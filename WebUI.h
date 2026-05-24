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
    
    <script src="./nipplejs.min.js"></script>
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

const char nipplejs_min_js[] PROGMEM = R"=====(
!function(t,i){"object"==typeof exports&&"object"==typeof module?module.exports=i():"function"==typeof define&&define.amd?define("nipplejs",[],i):"object"==typeof exports?exports.nipplejs=i():t.nipplejs=i()}(window,function(){return e=[function(t,i,e){"use strict";e.r(i);function v(t,i){var e=i.x-t.x,i=i.y-t.y;return Math.sqrt(e*e+i*i)}function b(t){return t*(Math.PI/180)}function n(t){f.has(t)&&clearTimeout(f.get(t)),f.set(t,setTimeout(t,100))}function s(t,i,e){for(var o,n=i.split(/[ ,]+/g),s=0;s<n.length;s+=1)o=n[s],t.addEventListener?t.addEventListener(o,e,!1):t.attachEvent&&t.attachEvent(o,e)}function o(t,i,e){for(var o,n=i.split(/[ ,]+/g),s=0;s<n.length;s+=1)o=n[s],t.removeEventListener?t.removeEventListener(o,e):t.detachEvent&&t.detachEvent(o,e)}function r(t){return t.preventDefault(),t.type.match(/^touch/)?t.changedTouches:t}function d(){return{x:void 0!==window.pageXOffset?window.pageXOffset:(document.documentElement||document.body.parentNode||document.body).scrollLeft,y:void 0!==window.pageYOffset?window.pageYOffset:(document.documentElement||document.body.parentNode||document.body).scrollTop}}function p(t,i){i.top||i.right||i.bottom||i.left?(t.style.top=i.top,t.style.right=i.right,t.style.bottom=i.bottom,t.style.left=i.left):(t.style.left=i.x+"px",t.style.top=i.y+"px")}function a(t,i,e){var o,n=c(t);for(o in n)if(n.hasOwnProperty(o))if("string"==typeof i)n[o]=i+" "+e;else{for(var s="",r=0,d=i.length;r<d;r+=1)s+=i[r]+" "+e+", ";n[o]=s.slice(0,-2)}return n}function c(i){var e={};return e[i]="",["webkit","Moz","o"].forEach(function(t){e[t+i.charAt(0).toUpperCase()+i.slice(1)]=""}),e}function l(t,i){for(var e in i)i.hasOwnProperty(e)&&(t[e]=i[e])}function h(t,i){if(t.length)for(var e=0,o=t.length;e<o;e+=1)i(t[e]);else i(t)}var u,f=new Map,e=!!("ontouchstart"in window),y=!!window.PointerEvent,m=!!window.MSPointerEvent,g={start:"mousedown",move:"mousemove",end:"mouseup"},x={};function O(){}y?u={start:"pointerdown",move:"pointermove",end:"pointerup, pointercancel"}:m?u={start:"MSPointerDown",move:"MSPointerMove",end:"MSPointerUp"}:e?(u={start:"touchstart",move:"touchmove",end:"touchend, touchcancel"},x=g):u=g,O.prototype.on=function(t,i){var e,o=t.split(/[ ,]+/g);this._handlers_=this._handlers_||{};for(var n=0;n<o.length;n+=1)e=o[n],this._handlers_[e]=this._handlers_[e]||[],this._handlers_[e].push(i);return this},O.prototype.off=function(t,i){return this._handlers_=this._handlers_||{},void 0===t?this._handlers_={}:void 0===i?this._handlers_[t]=null:this._handlers_[t]&&0<=this._handlers_[t].indexOf(i)&&this._handlers_[t].splice(this._handlers_[t].indexOf(i),1),this},O.prototype.trigger=function(t,i){var e,o=this,n=t.split(/[ ,]+/g);o._handlers_=o._handlers_||{};for(var s=0;s<n.length;s+=1)e=n[s],o._handlers_[e]&&o._handlers_[e].length&&o._handlers_[e].forEach(function(t){t.call(o,{type:e,target:o},i)})},O.prototype.config=function(t){this.options=this.defaults||{},t&&(this.options=function(t,i){var e,o={};for(e in t)t.hasOwnProperty(e)&&i.hasOwnProperty(e)?o[e]=i[e]:t.hasOwnProperty(e)&&(o[e]=t[e]);return o}(this.options,t))},O.prototype.bindEvt=function(t,i){var e=this;return e._domHandlers_=e._domHandlers_||{},e._domHandlers_[i]=function(){"function"==typeof e["on"+i]?e["on"+i].apply(e,arguments):console.warn('[WARNING] : Missing "on'+i+'" handler.')},s(t,u[i],e._domHandlers_[i]),x[i]&&s(t,x[i],e._domHandlers_[i]),e},O.prototype.unbindEvt=function(t,i){return this._domHandlers_=this._domHandlers_||{},o(t,u[i],this._domHandlers_[i]),x[i]&&o(t,x[i],this._domHandlers_[i]),delete this._domHandlers_[i],this};y=O;function w(t,i){return this.identifier=i.identifier,this.position=i.position,this.frontPosition=i.frontPosition,this.collection=t,this.defaults={size:100,threshold:.1,color:"white",fadeTime:250,dataOnly:!1,restJoystick:!0,restOpacity:.5,mode:"dynamic",zone:document.body,lockX:!1,lockY:!1,shape:"circle"},this.config(i),"dynamic"===this.options.mode&&(this.options.restOpacity=0),this.id=w.id,w.id+=1,this.buildEl().stylize(),this.instance={el:this.ui.el,on:this.on.bind(this),off:this.off.bind(this),show:this.show.bind(this),hide:this.hide.bind(this),add:this.addToDom.bind(this),remove:this.removeFromDom.bind(this),destroy:this.destroy.bind(this),setPosition:this.setPosition.bind(this),resetDirection:this.resetDirection.bind(this),computeDirection:this.computeDirection.bind(this),trigger:this.trigger.bind(this),position:this.position,frontPosition:this.frontPosition,ui:this.ui,identifier:this.identifier,id:this.id,options:this.options},this.instance}w.prototype=new y,(w.constructor=w).id=0,w.prototype.buildEl=function(t){return this.ui={},this.options.dataOnly||(this.ui.el=document.createElement("div"),this.ui.back=document.createElement("div"),this.ui.front=document.createElement("div"),this.ui.el.className="nipple collection_"+this.collection.id,this.ui.back.className="back",this.ui.front.className="front",this.ui.el.setAttribute("id","nipple_"+this.collection.id+"_"+this.id),this.ui.el.appendChild(this.ui.back),this.ui.el.appendChild(this.ui.front)),this},w.prototype.stylize=function(){if(this.options.dataOnly)return this;var t=this.options.fadeTime+"ms",i=function(){var t,i=c("borderRadius");for(t in i)i.hasOwnProperty(t)&&(i[t]="50%");return i}(),t=a("transition","opacity",t),e={};return e.el={position:"absolute",opacity:this.options.restOpacity,display:"block",zIndex:999},e.back={position:"absolute",display:"block",width:this.options.size+"px",height:this.options.size+"px",marginLeft:-this.options.size/2+"px",marginTop:-this.options.size/2+"px",background:this.options.color,opacity:".5"},e.front={width:this.options.size/2+"px",height:this.options.size/2+"px",position:"absolute",display:"block",marginLeft:-this.options.size/4+"px",marginTop:-this.options.size/4+"px",background:this.options.color,opacity:".5",transform:"translate(0px, 0px)"},l(e.el,t),"circle"===this.options.shape&&l(e.back,i),l(e.front,i),this.applyStyles(e),this},w.prototype.applyStyles=function(t){for(var i in this.ui)if(this.ui.hasOwnProperty(i))for(var e in t[i])this.ui[i].style[e]=t[i][e];return this},w.prototype.addToDom=function(){return this.options.dataOnly||document.body.contains(this.ui.el)||this.options.zone.appendChild(this.ui.el),this},w.prototype.removeFromDom=function(){return!this.options.dataOnly&&document.body.contains(this.ui.el)&&this.options.zone.removeChild(this.ui.el),this},w.prototype.destroy=function(){clearTimeout(this.removeTimeout),clearTimeout(this.showTimeout),clearTimeout(this.restTimeout),this.trigger("destroyed",this.instance),this.removeFromDom(),this.off()},w.prototype.show=function(t){var i=this;return i.options.dataOnly||(clearTimeout(i.removeTimeout),clearTimeout(i.showTimeout),clearTimeout(i.restTimeout),i.addToDom(),i.restCallback(),setTimeout(function(){i.ui.el.style.opacity=1},0),i.showTimeout=setTimeout(function(){i.trigger("shown",i.instance),"function"==typeof t&&t.call(this)},i.options.fadeTime)),i},w.prototype.hide=function(i){var t,e,o=this;return o.options.dataOnly||(o.ui.el.style.opacity=o.options.restOpacity,clearTimeout(o.removeTimeout),clearTimeout(o.showTimeout),clearTimeout(o.restTimeout),o.removeTimeout=setTimeout(function(){var t="dynamic"===o.options.mode?"none":"block";o.ui.el.style.display=t,"function"==typeof i&&i.call(o),o.trigger("hidden",o.instance)},o.options.fadeTime),o.options.restJoystick&&(t=o.options.restJoystick,(e={}).x=!0===t||!1!==t.x?0:o.instance.frontPosition.x,e.y=!0===t||!1!==t.y?0:o.instance.frontPosition.y,o.setPosition(i,e))),o},w.prototype.setPosition=function(t,i){var e=this,i=(e.frontPosition={x:i.x,y:i.y},e.options.fadeTime+"ms"),o={},i=(o.front=a("transition",["transform"],i),{front:{}});i.front={transform:"translate("+e.frontPosition.x+"px,"+e.frontPosition.y+"px)"},e.applyStyles(o),e.applyStyles(i),e.restTimeout=setTimeout(function(){"function"==typeof t&&t.call(e),e.restCallback()},e.options.fadeTime)},w.prototype.restCallback=function(){var t={};t.front=a("transition","none",""),this.applyStyles(t),this.trigger("rested",this.instance)},w.prototype.resetDirection=function(){this.direction={x:!1,y:!1,angle:!1}},w.prototype.computeDirection=function(t){var i,e,o,n=t.angle.radian,s=Math.PI/4,r=Math.PI/2;if(s<n&&n<3*s&&!t.lockX?i="up":-s<n&&n<=s&&!t.lockY?i="left":3*-s<n&&n<=-s&&!t.lockX?i="down":t.lockY||(i="right"),t.lockY||(e=-r<n&&n<r?"left":"right"),t.lockX||(o=0<n?"up":"down"),t.force>this.options.threshold){var d,p={};for(d in this.direction)this.direction.hasOwnProperty(d)&&(p[d]=this.direction[d]);var a={};for(d in this.direction={x:e,y:o,angle:i},t.direction=this.direction,p)p[d]===this.direction[d]&&(a[d]=!0);if(a.x&&a.y&&a.angle)return t;a.x&&a.y||this.trigger("plain",t),a.x||this.trigger("plain:"+e,t),a.y||this.trigger("plain:"+o,t),a.angle||this.trigger("dir dir:"+i,t)}else this.resetDirection();return t};var _=w;function T(t,i){this.nipples=[],this.idles=[],this.actives=[],this.ids=[],this.pressureIntervals={},this.manager=t,this.id=T.id,T.id+=1,this.defaults={zone:document.body,multitouch:!1,maxNumberOfNipples:10,mode:"dynamic",position:{top:0,left:0},catchDistance:200,size:100,threshold:.1,color:"white",fadeTime:250,dataOnly:!1,restJoystick:!0,restOpacity:.5,lockX:!1,lockY:!1,shape:"circle",dynamicPage:!1,follow:!1},this.config(i),"static"!==this.options.mode&&"semi"!==this.options.mode||(this.options.multitouch=!1),this.options.multitouch||(this.options.maxNumberOfNipples=1);t=getComputedStyle(this.options.zone.parentElement);return t&&"flex"===t.display&&(this.parentIsFlex=!0),this.updateBox(),this.prepareNipples(),this.bindings(),this.begin(),this.nipples}T.prototype=new y,(T.constructor=T).id=0,T.prototype.prepareNipples=function(){var o=this.nipples;o.on=this.on.bind(this),o.off=this.off.bind(this),o.options=this.options,o.destroy=this.destroy.bind(this),o.ids=this.ids,o.id=this.id,o.processOnMove=this.processOnMove.bind(this),o.processOnEnd=this.processOnEnd.bind(this),o.get=function(t){if(void 0===t)return o[0];for(var i=0,e=o.length;i<e;i+=1)if(o[i].identifier===t)return o[i];return!1}},T.prototype.bindings=function(){this.bindEvt(this.options.zone,"start"),this.options.zone.style.touchAction="none",this.options.zone.style.msTouchAction="none"},T.prototype.begin=function(){var t=this.options;"static"===t.mode&&((t=this.createNipple(t.position,this.manager.getIdentifier())).add(),this.idles.push(t))},T.prototype.createNipple=function(t,i){var e=this.manager.scroll,o={},n=this.options,s=this.parentIsFlex?e.x:e.x+this.box.left,r=this.parentIsFlex?e.y:e.y+this.box.top,s=(t.x&&t.y?o={x:t.x-s,y:t.y-r}:(t.top||t.right||t.bottom||t.left)&&((s=document.createElement("DIV")).style.display="hidden",s.style.top=t.top,s.style.right=t.right,s.style.bottom=t.bottom,s.style.left=t.left,s.style.position="absolute",n.zone.appendChild(s),r=s.getBoundingClientRect(),n.zone.removeChild(s),o=t,t={x:r.left+e.x,y:r.top+e.y}),new _(this,{color:n.color,size:n.size,threshold:n.threshold,fadeTime:n.fadeTime,dataOnly:n.dataOnly,restJoystick:n.restJoystick,restOpacity:n.restOpacity,mode:n.mode,identifier:i,position:t,zone:n.zone,frontPosition:{x:0,y:0},shape:n.shape}));return n.dataOnly||(p(s.ui.el,o),p(s.ui.front,s.frontPosition)),this.nipples.push(s),this.trigger("added "+s.identifier+":added",s),this.manager.trigger("added "+s.identifier+":added",s),this.bindNipple(s),s},T.prototype.updateBox=function(){this.box=this.options.zone.getBoundingClientRect()},T.prototype.bindNipple=function(t){function i(t,i){e=t.type+" "+i.id+":"+t.type,o.trigger(e,i)}var e,o=this;t.on("destroyed",o.onDestroyed.bind(o)),t.on("shown hidden rested dir plain",i),t.on("dir:up dir:right dir:down dir:left",i),t.on("plain:up plain:right plain:down plain:left",i)},T.prototype.pressureFn=function(i,e,t){var o=this,n=0;clearInterval(o.pressureIntervals[t]),o.pressureIntervals[t]=setInterval(function(){var t=i.force||i.pressure||i.webkitForce||0;t!==n&&(e.trigger("pressure",t),o.trigger("pressure "+e.identifier+":pressure",t),n=t)}.bind(o),100)},T.prototype.onstart=function(e){var o=this,i=o.options,n=e;return e=r(e),o.updateBox(),h(e,function(t){o.actives.length<i.maxNumberOfNipples?o.processOnStart(t):n.type.match(/^touch/)&&(Object.keys(o.manager.ids).forEach(function(i){var t;Object.values(n.touches).findIndex(function(t){return t.identifier===i})<0&&((t=[e[0]]).identifier=i,o.processOnEnd(t))}),o.actives.length<i.maxNumberOfNipples&&o.processOnStart(t))}),o.manager.bindDocument(),!1},T.prototype.processOnStart=function(i){function t(t){t.trigger("start",t),e.trigger("start "+t.id+":start",t),t.show(),0<s&&e.pressureFn(i,t,t.identifier),e.processOnMove(i)}var e=this,o=e.options,n=e.manager.getIdentifier(i),s=i.force||i.pressure||i.webkitForce||0,r={x:i.pageX,y:i.pageY},d=e.getOrCreate(n,r);d.identifier!==n&&e.manager.removeIdentifier(d.identifier),d.identifier=n;if(0<=(n=e.idles.indexOf(d))&&e.idles.splice(n,1),e.actives.push(d),e.ids.push(d.identifier),"semi"!==o.mode)t(d);else{if(!(v(r,d.position)<=o.catchDistance))return d.destroy(),void e.processOnStart(i);t(d)}return d},T.prototype.getOrCreate=function(t,i){var e,o=this.options;return/(semi|static)/.test(o.mode)?(e=this.idles[0])?(this.idles.splice(0,1),e):"semi"===o.mode?this.createNipple(i,t):(console.warn("Coudln't find the needed nipple."),!1):this.createNipple(i,t)},T.prototype.processOnMove=function(t){var i=this.options,e=this.manager.getIdentifier(t),o=this.nipples.get(e),n=this.manager.scroll;if(s=t,isNaN(s.buttons)?0!==s.pressure:0!==s.buttons){if(!o)return console.error("Found zombie joystick with ID "+e),void this.manager.removeIdentifier(e);i.dynamicPage&&(s=o.el.getBoundingClientRect(),o.position={x:n.x+s.left,y:n.y+s.top}),o.identifier=e;var s=o.options.size/2,e={x:t.pageX,y:t.pageY};i.lockX&&(e.y=o.position.y),i.lockY&&(e.x=o.position.x);var r,d,p,a,c,l=v(e,o.position),h=(h=e,u=o.position,f=u.x-h.x,u=u.y-h.y,Math.atan2(u,f)*(180/Math.PI)),u=b(h),f=l/s,y={distance:l,position:e},m=("circle"===o.options.shape?(r=Math.min(l,s),d=o.position,p=r,c={x:0,y:0},a=b(h),c.x=d.x-p*Math.cos(a),c.y=d.y-p*Math.sin(a),d=c):(p=e,a=o.position,c=s,d={x:Math.min(Math.max(p.x,a.x-c),a.x+c),y:Math.min(Math.max(p.y,a.y-c),a.y+c)},r=v(d,o.position)),i.follow?s<l&&(m=e.x-d.x,g=e.y-d.y,o.position.x+=m,o.position.y+=g,o.el.style.top=o.position.y-(this.box.top+n.y)+"px",o.el.style.left=o.position.x-(this.box.left+n.x)+"px",l=v(e,o.position)):(e=d,l=r),e.x-o.position.x),g=e.y-o.position.y,n=(o.frontPosition={x:m,y:g},i.dataOnly||(o.ui.front.style.transform="translate("+m+"px,"+g+"px)"),{identifier:o.identifier,position:e,force:f,pressure:t.force||t.pressure||t.webkitForce||0,distance:l,angle:{radian:u,degree:h},vector:{x:m/s,y:-g/s},raw:y,instance:o,lockX:i.lockX,lockY:i.lockY});(n=o.computeDirection(n)).angle={radian:b(180-h),degree:180-h},o.trigger("move",n),this.trigger("move "+o.id+":move",n)}else this.processOnEnd(t)},T.prototype.processOnEnd=function(t){var i=this,e=i.options,t=i.manager.getIdentifier(t),o=i.nipples.get(t),t=i.manager.removeIdentifier(o.identifier);o&&(e.dataOnly||o.hide(function(){"dynamic"===e.mode&&(o.trigger("removed",o),i.trigger("removed "+o.id+":removed",o),i.manager.trigger("removed "+o.id+":removed",o),o.destroy())}),clearInterval(i.pressureIntervals[o.identifier]),o.resetDirection(),o.trigger("end",o),i.trigger("end "+o.id+":end",o),0<=i.ids.indexOf(o.identifier)&&i.ids.splice(i.ids.indexOf(o.identifier),1),0<=i.actives.indexOf(o)&&i.actives.splice(i.actives.indexOf(o),1),/(semi|static)/.test(e.mode)?i.idles.push(o):0<=i.nipples.indexOf(o)&&i.nipples.splice(i.nipples.indexOf(o),1),i.manager.unbindDocument(),/(semi|static)/.test(e.mode)&&(i.manager.ids[t.id]=t.identifier))},T.prototype.onDestroyed=function(t,i){0<=this.nipples.indexOf(i)&&this.nipples.splice(this.nipples.indexOf(i),1),0<=this.actives.indexOf(i)&&this.actives.splice(this.actives.indexOf(i),1),0<=this.idles.indexOf(i)&&this.idles.splice(this.idles.indexOf(i),1),0<=this.ids.indexOf(i.identifier)&&this.ids.splice(this.ids.indexOf(i.identifier),1),this.manager.removeIdentifier(i.identifier),this.manager.unbindDocument()},T.prototype.destroy=function(){for(var t in this.unbindEvt(this.options.zone,"start"),this.nipples.forEach(function(t){t.destroy()}),this.pressureIntervals)this.pressureIntervals.hasOwnProperty(t)&&clearInterval(this.pressureIntervals[t]);this.trigger("destroyed",this.nipples),this.manager.unbindDocument(),this.off()};var k=T;function P(t){function i(){var i;o.collections.forEach(function(t){t.forEach(function(t){i=t.el.getBoundingClientRect(),t.position={x:o.scroll.x+i.left,y:o.scroll.y+i.top}})})}function e(){o.scroll=d()}var o=this;o.ids={},o.index=0,o.collections=[],o.scroll=d(),o.config(t),o.prepareCollections(),s(window,"resize",function(){n(i)});return s(window,"scroll",function(){n(e)}),o.collections}P.prototype=new y,(P.constructor=P).prototype.prepareCollections=function(){var t=this;t.collections.create=t.create.bind(t),t.collections.on=t.on.bind(t),t.collections.off=t.off.bind(t),t.collections.destroy=t.destroy.bind(t),t.collections.get=function(i){var e;return t.collections.every(function(t){return!(e=t.get(i))}),e}},P.prototype.create=function(t){return this.createCollection(t)},P.prototype.createCollection=function(t){t=new k(this,t);return this.bindCollection(t),this.collections.push(t),t},P.prototype.bindCollection=function(t){function i(t,i){e=t.type+" "+i.id+":"+t.type,o.trigger(e,i)}var e,o=this;t.on("destroyed",o.onDestroyed.bind(o)),t.on("shown hidden rested dir plain",i),t.on("dir:up dir:right dir:down dir:left",i),t.on("plain:up plain:right plain:down plain:left",i)},P.prototype.bindDocument=function(){this.binded||(this.bindEvt(document,"move").bindEvt(document,"end"),this.binded=!0)},P.prototype.unbindDocument=function(t){Object.keys(this.ids).length&&!0!==t||(this.unbindEvt(document,"move").unbindEvt(document,"end"),this.binded=!1)},P.prototype.getIdentifier=function(t){var i;return t?void 0===(i=void 0===t.identifier?t.pointerId:t.identifier)&&(i=this.latest||0):i=this.index,void 0===this.ids[i]&&(this.ids[i]=this.index,this.index+=1),this.latest=i,this.ids[i]},P.prototype.removeIdentifier=function(t){var i,e={};for(i in this.ids)if(this.ids[i]===t){e.id=i,e.identifier=this.ids[i],delete this.ids[i];break}return e},P.prototype.onmove=function(t){return this.onAny("move",t),!1},P.prototype.onend=function(t){return this.onAny("end",t),!1},P.prototype.oncancel=function(t){return this.onAny("end",t),!1},P.prototype.onAny=function(t,i){var e,o=this,n="processOn"+t.charAt(0).toUpperCase()+t.slice(1);i=r(i);return h(i,function(t){e=o.getIdentifier(t),h(o.collections,function(t,i,e){0<=e.ids.indexOf(i)&&(e[n](t),t._found_=!0)}.bind(null,t,e)),t._found_||o.removeIdentifier(e)}),!1},P.prototype.destroy=function(){this.unbindDocument(!0),this.ids={},this.index=0,this.collections.forEach(function(t){t.destroy()}),this.off()},P.prototype.onDestroyed=function(t,i){if(this.collections.indexOf(i)<0)return!1;this.collections.splice(this.collections.indexOf(i),1)};var E=new P;i.default={create:function(t){return E.create(t)},factory:E}}],o={},n.m=e,n.c=o,n.d=function(t,i,e){n.o(t,i)||Object.defineProperty(t,i,{enumerable:!0,get:e})},n.r=function(t){"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(t,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(t,"__esModule",{value:!0})},n.t=function(i,t){if(1&t&&(i=n(i)),8&t)return i;if(4&t&&"object"==typeof i&&i&&i.__esModule)return i;var e=Object.create(null);if(n.r(e),Object.defineProperty(e,"default",{enumerable:!0,value:i}),2&t&&"string"!=typeof i)for(var o in i)n.d(e,o,function(t){return i[t]}.bind(null,o));return e},n.n=function(t){var i=t&&t.__esModule?function(){return t.default}:function(){return t};return n.d(i,"a",i),i},n.o=function(t,i){return Object.prototype.hasOwnProperty.call(t,i)},n.p="",n(n.s=0).default;function n(t){if(o[t])return o[t].exports;var i=o[t]={i:t,l:!1,exports:{}};return e[t].call(i.exports,i,i.exports,n),i.l=!0,i.exports}var e,o});
)=====";

#endif // WEBUI_H