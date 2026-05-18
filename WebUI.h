// Auto-generated file containing Web UI
#ifndef WEBUI_H
#define WEBUI_H

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <title>ThrowBot Control Panel</title>
    <link rel="stylesheet" href="./style.css">
    <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;500;700&family=Inter:wght@400;500&display=swap" rel="stylesheet">
  </head>
  <body>
    <div id="app">
      
      <!-- Top Status Bar -->
      <div class="status-bar">
        <div class="brand">
          <span class="logo">TB</span>
          <span class="title">THROWBOT S-3</span>
        </div>
        <div class="connection-status">
          <div class="status-dot disconnected" id="status-indicator"></div>
          <span id="status-text" id="btn-disconnect" style="cursor:pointer;">Disconnected</span>
        </div>
      </div>

      <!-- SETUP SCREEN -->
      <div id="setup-screen" class="setup-screen">
        <div class="setup-card">
          <h2>SYSTEM SETUP</h2>
          <div class="input-group">
            <label for="ws-url">ESP32 WebSocket URL</label>
            <input type="text" id="ws-url" value="ws://192.168.4.1:81/" />
          </div>
          <div class="input-group">
            <label for="video-url">Video Stream URL</label>
            <input type="text" id="video-url" value="http://192.168.4.1:81/stream" />
          </div>
          <button id="btn-connect" class="btn primary-btn" style="margin-top: 15px;">START CONNECTION</button>
        </div>
      </div>

      <!-- DRIVE SCREEN -->
      <div id="drive-screen" class="drive-screen" style="display: none;">
        
        <!-- Video Feed Section -->
        <div class="video-container">
          <div class="video-placeholder">
            <span class="video-icon">📷</span>
            <p>Waiting for video stream...</p>
          </div>
          <img id="video-stream" src="" alt="Live Video Feed" style="display: none;" />
          
          <div class="telemetry-overlay">
            <div class="telemetry-item"><span class="label">X</span><span class="value" id="val-x">0</span></div>
            <div class="telemetry-item"><span class="label">Y</span><span class="value" id="val-y">0</span></div>
            <div class="telemetry-item"><span class="label">RATE</span><span class="value" id="val-rate">0 Hz</span></div>
          </div>
        </div>

        <!-- Controls Section -->
        <div class="controls-container">
          <!-- Action Bar -->
          <div class="action-bar">
            <button id="btn-estop" class="btn estop-btn">
              <span class="icon">🛑</span> EMERGENCY STOP
            </button>
          </div>
          
          <!-- Virtual Joystick Area -->
          <div class="joystick-wrapper">
            <div id="joystick-zone"></div>
          </div>
        </div>

      </div>

    </div>
    
    <script src="https://cdnjs.cloudflare.com/ajax/libs/nipplejs/0.10.1/nipplejs.min.js"></script>
    <script type="module" src="./main.js"></script>
  </body>
</html>

)=====";

const char style_css[] PROGMEM = R"=====(
:root {
  --bg-color: #0b0c10;
  --surface-color: #1f2833;
  --primary-color: #66fcf1;
  --secondary-color: #45a29e;
  --text-primary: #ffffff;
  --text-secondary: #c5c6c7;
  --danger-color: #ff4136;
  --success-color: #2ecc40;
}

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  user-select: none;
  -webkit-touch-callout: none;
}

body {
  font-family: 'Inter', sans-serif;
  background-color: var(--bg-color);
  color: var(--text-primary);
  height: 100vh;
  width: 100vw;
  overflow: hidden;
  display: flex;
  flex-direction: column;
}

#app {
  display: flex;
  flex-direction: column;
  height: 100%;
  width: 100%;
}

/* Status Bar */
.status-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 10px 20px;
  background: rgba(31, 40, 51, 0.8);
  backdrop-filter: blur(10px);
  border-bottom: 1px solid rgba(102, 252, 241, 0.2);
  z-index: 100;
}

.brand {
  display: flex;
  align-items: center;
  gap: 10px;
}

.logo {
  font-family: 'Orbitron', sans-serif;
  font-weight: 700;
  font-size: 1.2rem;
  color: var(--bg-color);
  background: var(--primary-color);
  padding: 2px 8px;
  border-radius: 4px;
}

.title {
  font-family: 'Orbitron', sans-serif;
  font-size: 1rem;
  letter-spacing: 2px;
  color: var(--text-primary);
}

.connection-status {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 0.85rem;
  color: var(--text-secondary);
}

.status-dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
  box-shadow: 0 0 8px currentColor;
}

.status-dot.disconnected {
  color: var(--danger-color);
  background-color: var(--danger-color);
}

.status-dot.connected {
  color: var(--success-color);
  background-color: var(--success-color);
}

/* Forms & Inputs */
.input-group {
  display: flex;
  flex-direction: column;
  gap: 5px;
  width: 100%;
}

.input-group label {
  font-size: 0.75rem;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 1px;
}

.input-group input {
  background: rgba(11, 12, 16, 0.5);
  border: 1px solid rgba(102, 252, 241, 0.3);
  padding: 12px;
  color: white;
  border-radius: 4px;
  font-family: monospace;
  font-size: 1rem;
  outline: none;
  transition: border-color 0.3s;
  width: 100%;
}

.input-group input:focus {
  border-color: var(--primary-color);
}

/* Buttons */
.btn {
  padding: 15px 20px;
  border: none;
  border-radius: 6px;
  font-family: 'Orbitron', sans-serif;
  font-weight: 700;
  cursor: pointer;
  text-transform: uppercase;
  letter-spacing: 1px;
  transition: all 0.2s;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  width: 100%;
  font-size: 1rem;
}

.btn:active {
  transform: scale(0.95);
}

.primary-btn {
  background-color: var(--primary-color);
  color: var(--bg-color);
  box-shadow: 0 0 15px rgba(102, 252, 241, 0.4);
}

/* --- SETUP SCREEN --- */
.setup-screen {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 20px;
  background: radial-gradient(circle at center, rgba(31, 40, 51, 0.8) 0%, var(--bg-color) 100%);
}

.setup-card {
  width: 100%;
  max-width: 400px;
  background: rgba(11, 12, 16, 0.6);
  padding: 30px 20px;
  border-radius: 12px;
  border: 1px solid rgba(102, 252, 241, 0.2);
  display: flex;
  flex-direction: column;
  gap: 20px;
  box-shadow: 0 10px 30px rgba(0,0,0,0.5);
}

.setup-card h2 {
  font-family: 'Orbitron', sans-serif;
  text-align: center;
  color: var(--primary-color);
  font-size: 1.2rem;
  margin-bottom: 10px;
}

/* --- DRIVE SCREEN --- */
.drive-screen {
  flex: 1;
  display: flex;
  flex-direction: column;
}

.video-container {
  flex: none;
  height: 45vh;
  position: relative;
  background-color: #000;
  display: flex;
  justify-content: center;
  align-items: center;
  overflow: hidden;
  border-bottom: 2px solid var(--primary-color);
}

.video-placeholder {
  text-align: center;
  color: var(--text-secondary);
}

.video-icon {
  font-size: 3rem;
  margin-bottom: 10px;
  display: block;
}

#video-stream {
  width: 100%;
  height: 100%;
  object-fit: cover;
}

.telemetry-overlay {
  position: absolute;
  bottom: 15px;
  left: 15px;
  display: flex;
  gap: 15px;
  background: rgba(11, 12, 16, 0.6);
  backdrop-filter: blur(5px);
  padding: 8px 15px;
  border-radius: 8px;
  border: 1px solid rgba(102, 252, 241, 0.3);
}

.telemetry-item {
  display: flex;
  flex-direction: column;
}

.telemetry-item .label {
  font-size: 0.65rem;
  color: var(--primary-color);
  font-family: 'Orbitron', sans-serif;
}

.telemetry-item .value {
  font-size: 1.1rem;
  font-weight: bold;
  font-family: monospace;
}

.controls-container {
  flex: 1;
  background: var(--surface-color);
  display: flex;
  flex-direction: column;
  position: relative;
}

.action-bar {
  padding: 15px 20px 0 20px;
  flex: none;
}

.estop-btn {
  background-color: transparent;
  color: var(--danger-color);
  border: 2px solid var(--danger-color);
}

.estop-btn:active, .estop-btn.active {
  background-color: var(--danger-color);
  color: white;
  box-shadow: 0 0 20px var(--danger-color);
}

.joystick-wrapper {
  flex: 1;
  position: relative;
  display: flex;
  justify-content: center;
  align-items: center;
  background: radial-gradient(circle at center, rgba(102, 252, 241, 0.1) 0%, transparent 60%);
}

#joystick-zone {
  width: 100%;
  height: 100%;
  position: relative;
}

/* Landscape Mode overrides */
@media (max-width: 800px) and (orientation: landscape) {
  .drive-screen {
    flex-direction: row;
  }
  .video-container {
    height: 100%;
    flex: 1;
    border-bottom: none;
    border-right: 2px solid var(--primary-color);
  }
  .controls-container {
    width: 250px;
    flex: none;
  }
}

)=====";

const char main_js[] PROGMEM = R"=====(
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


)=====";

#endif // WEBUI_H
