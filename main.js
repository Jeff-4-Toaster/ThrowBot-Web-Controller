// App State
let ws = null;
let isConnected = false;
let joystickManager = null;
let lastSendTime = 0;
const SEND_INTERVAL_MS = 50; // 20Hz refresh rate

// DOM Elements
const statusIndicator = document.getElementById('status-indicator');
const statusText = document.getElementById('status-text'); // This is now also the disconnect button
const videoStream = document.getElementById('video-stream');
const videoPlaceholder = document.querySelector('.video-placeholder');
const btnConnect = document.getElementById('btn-connect');
const btnEStop = document.getElementById('btn-estop');
const inputWsUrl = document.getElementById('ws-url');
const inputVideoUrl = document.getElementById('video-url');
const valX = document.getElementById('val-x');
const valY = document.getElementById('val-y');
const valRate = document.getElementById('val-rate');

// Screens
const setupScreen = document.getElementById('setup-screen');
const driveScreen = document.getElementById('drive-screen');

// FPS counter for telemetry
let packetsSent = 0;
setInterval(() => {
  valRate.textContent = `${packetsSent} Hz`;
  packetsSent = 0;
}, 1000);

// Initialize Joystick
function initJoystick() {
  const zone = document.getElementById('joystick-zone');
  
  joystickManager = nipplejs.create({
    zone: zone,
    mode: 'static',
    position: { left: '50%', top: '50%' },
    color: '#66fcf1',
    size: 150,
    restOpacity: 0.5
  });

  joystickManager.on('move', (evt, data) => {
    const angle = data.angle.radian;
    const distance = Math.min(data.distance, 75); // max radius is 75
    const normalizedDist = distance / 75;

    let x = Math.round(Math.cos(angle) * normalizedDist * 100);
    let y = Math.round(Math.sin(angle) * normalizedDist * 100);

    updateTelemetry(x, y);
    sendControlData(x, y, false);
  });

  joystickManager.on('end', () => {
    // Force sending (0,0) by bypassing throttle
    updateTelemetry(0, 0);
    sendControlData(0, 0, true); 
  });
}

// Update UI Text
function updateTelemetry(x, y) {
  valX.textContent = x;
  valY.textContent = y;
}

// WebSocket Connection
function connectWebSocket() {
  const url = inputWsUrl.value;
  btnConnect.textContent = "CONNECTING...";
  
  try {
    ws = new WebSocket(url);
    
    ws.onopen = () => {
      isConnected = true;
      statusIndicator.className = 'status-dot connected';
      statusText.textContent = "Disconnect";
      
      // Switch screens
      setupScreen.style.display = 'none';
      driveScreen.style.display = 'flex';
      
      // Video
      const vUrl = inputVideoUrl.value;
      if (vUrl) {
        videoStream.src = vUrl;
        videoStream.style.display = 'block';
        videoPlaceholder.style.display = 'none';
      }

      // Re-init joystick if needed (nipplejs can sometimes glitch if container was display:none)
      if (joystickManager) {
        joystickManager.destroy();
      }
      setTimeout(initJoystick, 100);
    };
    
    ws.onclose = () => {
      handleDisconnect();
    };
    
    ws.onerror = (err) => {
      console.error("WebSocket Error:", err);
      btnConnect.textContent = "START CONNECTION";
      // Let onclose handle the rest
    };
    
  } catch (err) {
    console.error("Invalid WS URL:", err);
    btnConnect.textContent = "START CONNECTION";
    alert("Invalid URL Format");
  }
}

function handleDisconnect() {
  isConnected = false;
  statusIndicator.className = 'status-dot disconnected';
  statusText.textContent = "Disconnected";
  btnConnect.textContent = "START CONNECTION";
  
  // Switch back to setup screen
  driveScreen.style.display = 'none';
  setupScreen.style.display = 'flex';
  
  // Stop video stream to save bandwidth
  videoStream.src = "";
  
  if (ws) {
    ws.close();
    ws = null;
  }
}

// Send Data
function sendControlData(x, y, force = false) {
  if (!isConnected || !ws || ws.readyState !== WebSocket.OPEN) return;
  
  const now = Date.now();
  if (!force && (now - lastSendTime < SEND_INTERVAL_MS)) return;
  
  const payload = JSON.stringify({ cmd: "move", x: x, y: y });
  ws.send(payload);
  lastSendTime = now;
  packetsSent++;
}

// Emergency Stop Handler
function handleEmergencyStop() {
  updateTelemetry(0, 0);
  btnEStop.classList.add('active');
  
  // Force bypass throttle
  sendControlData(0, 0, true);
  
  setTimeout(() => {
    btnEStop.classList.remove('active');
  }, 200);
}

// Event Listeners
btnConnect.addEventListener('click', connectWebSocket);

statusText.addEventListener('click', () => {
  if (isConnected) {
    handleDisconnect();
  }
});

btnEStop.addEventListener('click', handleEmergencyStop);
btnEStop.addEventListener('touchstart', (e) => {
  e.preventDefault();
  handleEmergencyStop();
});