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
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char *ssid = "ThrowBot_Wifi";
const char *password = "12345678"; 

WebSocketsServer webSocket = WebSocketsServer(80);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      int x = doc["x"];
      int y = doc["y"];
      
      // 在此實作差速控制 (Differential Drive)
      int left_motor_speed = y + x;
      int right_motor_speed = y - x;
      
      // 將速度限制在 -100 到 100 之間
      left_motor_speed = constrain(left_motor_speed, -100, 100);
      right_motor_speed = constrain(right_motor_speed, -100, 100);
      
      Serial.printf("X: %d, Y: %d | Left: %d, Right: %d\n", x, y, left_motor_speed, right_motor_speed);
      
      // TODO: 輸出 PWM 到 L298N 或 TB6612 馬達驅動板
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // 開啟熱點
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
```

## 4. 如何部署這個控制網頁？
您有三個選項可以讓手機顯示這個控制介面：
1. **開發測試期**：在電腦端使用 `python3 -m http.server 8080` 開啟，手機與電腦連同一個 Wi-Fi，手機瀏覽器輸入 `http://電腦IP:8080`。
2. **免安裝 APP (離線檔案)**：將 `index.html`、`style.css`、`main.js` 傳到手機裡，直接用手機瀏覽器開啟本機檔案。
3. **最專業作法 (燒入 ESP32)**：使用 `SPIFFS` 或 `LittleFS` 技術，把這三個檔案直接燒錄進 ESP32 內部記憶體。並搭配 `ESPAsyncWebServer` 庫，當手機連上 ESP32 的 Wi-Fi 時，直接打開 `http://192.168.4.1` 就能載入介面！