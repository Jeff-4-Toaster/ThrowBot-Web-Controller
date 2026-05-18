# ThrowBot S-3 使用手冊與硬體對接指南

這份手冊是寫給「負責開發 ESP32 硬體與馬達控制」的組員。透過這份指南，您可以快速將 ESP32 與手機的控制介面串接起來。

## 1. 系統架構簡介
目前的手機控制端是一個純前端網頁 (HTML/JS/CSS)。它的運作原理如下：
1. **影像部分**：手機網頁直接對 ESP32 發出 HTTP 請求，取得 MJPEG 串流。
2. **控制部分**：手機網頁會嘗試連線到 ESP32 建立的 **WebSocket Server (Port 80)**。連線成功後，每隔 50 毫秒會送出一次搖桿座標 JSON。

## 2. 搖桿訊號格式說明
當操作者滑動手機搖桿時，網頁會發送以下 JSON 格式字串給 ESP32：
```json
{
  "cmd": "move",
  "x": 50,
  "y": 80
}
```
*   `cmd`: 固定為 `"move"`。
*   `x`: 轉向軸，範圍從 `-100` (極左) 到 `100` (極右)。
*   `y`: 油門軸，範圍從 `-100` (全速後退) 到 `100` (全速前進)。

> [!IMPORTANT]
> **緊急煞車機制**：當操作者放開搖桿的瞬間，網頁會**無視發送冷卻時間**，瞬間發送 `{"cmd":"move", "x":0, "y":0}`。請務必在 ESP32 收到 `(0,0)` 時，立刻將馬達 PWM 設為 0 以保障安全。

## 3. ESP32 端 C++ 開發範例 (Arduino IDE)

請在您的 ESP32 影像控制板 (或主控板) 上，實作以下 WebSocket Server 功能。
建議使用 `WebSocketsServer` 庫 (作者：Markus Sattler) 與 `ArduinoJson` 庫。

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// 匯入自動產生的網頁介面標頭檔
#include "WebUI.h" 

const char *ssid = "ThrowBot_Wifi";
const char *password = "12345678"; 

// 設定 HTTP 伺服器在 Port 80
WebServer server(80);
// 設定 WebSocket 伺服器在 Port 81
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      int x = doc["x"];
      int y = doc["y"];
      
      // TODO: 差速控制 (Differential Drive) 運算放在這裡
      Serial.printf("X: %d, Y: %d\n", x, y);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // 開啟熱點
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // 設定 HTTP 網頁路由
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });
  server.on("/style.css", HTTP_GET, []() {
    server.send(200, "text/css", style_css);
  });
  server.on("/main.js", HTTP_GET, []() {
    server.send(200, "application/javascript", main_js);
  });
  
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();
}
```

## 4. 如何將網頁燒錄進 ESP32？(免裝外掛版)
業界有很多方式可以把網頁放進 ESP32 (例如 SPIFFS 或 LittleFS)。但為了減少 Arduino IDE 的外掛安裝麻煩，我推薦使用 **「C++ 標頭檔 (Header File)」** 法。

1. 我寫了一支 Python 腳本 `pack_to_header.py`。
2. 請在您的電腦上執行這支腳本： `python pack_to_header.py`。
3. 它會自動把 `index.html`, `style.css`, `main.js` 這三個檔案，轉碼成一個叫做 `WebUI.h` 的 C++ 檔案！
4. 您只需要把這個 `WebUI.h` 放到您的 Arduino 專案資料夾裡面。
5. 照著上面的程式碼寫 `#include "WebUI.h"`，然後一鍵燒錄，網頁介面就會順利被包進您的車子裡了！