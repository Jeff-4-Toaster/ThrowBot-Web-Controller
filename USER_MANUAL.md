# ThrowBot S-3 使用手冊與硬體對接指南

這份手冊是寫給「負責開發 ESP32 硬體與馬達控制」的組員。透過這份指南，您可以快速將 ESP32 與手機的控制介面串接起來。

## 1. 系統架構簡介
目前的手機控制端是一個純前端網頁 (HTML/JS/CSS)。它的運作原理如下：
1. **影像部分**：手機網頁直接對 ESP32 主板發出 HTTP 請求，取得 MJPEG 串流。
2. **控制部分**：手機網頁連線到 ESP32 主板建立的 **WebSocket Server (Port 81)**。連線成功後，每隔 50 毫秒送出搖桿座標 JSON。

### 終極雙板通訊架構 (Wi-Fi + ESP-NOW)
為了達到最低延遲與完全不卡頓的馬達控制，我們採用**主副板架構**：
*   **主板 (攝影機/網路板)**：負責開啟 Wi-Fi 給手機連線，接收手機的 WebSocket JSON 訊號，並「瞬間」轉成 ESP-NOW 封包射給副板。
*   **副板 (馬達控制板)**：**不開啟 Wi-Fi 連線**，純粹使用 ESP-NOW 監聽主板射過來的訊號。這樣副板可以把 100% 的雙核心算力用在 PID 與馬達控制上。

## 2. 搖桿訊號格式說明
當操作者滑動手機搖桿時，網頁會發送以下 JSON 格式字串給 ESP32 主板：
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
> **緊急煞車機制**：當操作者放開搖桿的瞬間，網頁會**無視發送冷卻時間**，瞬間發送 `{"cmd":"move", "x":0, "y":0}`。

## 3. 如何取得副板的真實 MAC Address？
ESP-NOW 必須要知道副板的 MAC Address 才能精準投遞控制訊號。
請先將以下這段極短的程式碼，**燒錄到您的「副板」上**：

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000); // 等待終端機準備好
  
  // 必須設定為 STA 模式才能讀取到正確的 MAC Address
  WiFi.mode(WIFI_STA);
  
  Serial.println();
  Serial.print("請複製這串 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() { }
```
**操作步驟：**
1. 燒錄上述程式到副板。
2. 打開 Arduino IDE 的 **序列埠監控視窗 (Serial Monitor)**，波特率設為 `115200`。
3. 按下板子上的 `EN` 或 `RST` (重置) 按鈕。
4. 您會看到類似 `24:6F:28:AE:9D:14` 的字串。請將它轉換成 `0x24, 0x6F, 0x28, 0xAE, 0x9D, 0x14` 格式，並填入下方主板程式碼的 `broadcastAddress` 中。

## 4. 雙板 C++ 開發範例 (Arduino IDE)

### 【共用結構】
兩塊板子都必須宣告這個相同的資料結構：
```cpp
// 定義傳輸的資料格式 (大小不可超過 250 bytes)
typedef struct struct_message {
    int x; // 轉向 (-100 ~ 100)
    int y; // 油門 (-100 ~ 100)
} struct_message;

struct_message motorCmd;
```

### 【主板】網路與轉發中心 (含網頁介面)
主板負責開啟熱點、提供網頁，並把收到的 WebSocket 轉成 ESP-NOW。
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <esp_now.h>
#include "WebUI.h" 

const char *ssid = "ThrowBot_Wifi";
const char *password = "12345678"; 

// 替換成您【副板】的真實 MAC Address (請務必修改！)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
esp_now_peer_info_t peerInfo;

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    StaticJsonDocument<200> doc;
    if (!deserializeJson(doc, payload)) {
      motorCmd.x = doc["x"];
      motorCmd.y = doc["y"];
      
      // 收到手機訊號，瞬間透過 ESP-NOW 發送給副板！
      esp_now_send(broadcastAddress, (uint8_t *) &motorCmd, sizeof(motorCmd));
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  
  // 初始化 ESP-NOW 並註冊副板
  if (esp_now_init() == ESP_OK) {
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
  }

  // 網頁路由 (必須加入 nipplejs.min.js 以支援離線搖桿)
  server.on("/", HTTP_GET, []() { server.send(200, "text/html", index_html); });
  server.on("/style.css", HTTP_GET, []() { server.send(200, "text/css", style_css); });
  server.on("/main.js", HTTP_GET, []() { server.send(200, "application/javascript", main_js); });
  server.on("/nipplejs.min.js", HTTP_GET, []() { server.send(200, "application/javascript", nipplejs_min_js); });
  
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();
}
```

### 【副板】馬達控制中心 (純 ESP-NOW)
副板**不需要**開啟 Wi-Fi 基地台，也不需要連上主板的 Wi-Fi，專心用 ESP-NOW 收指令即可。
```cpp
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    int x;
    int y;
} struct_message;

struct_message myData;

// ESP-NOW 接收回呼函數 (自動觸發)
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("X: %d, Y: %d\n", myData.x, myData.y);
  // TODO: 在這裡或者在 Core 1 的 Task 裡面更新馬達 PWM
}

void setup() {
  Serial.begin(115200);
  
  // 必須設定為 STA 模式才能使用 ESP-NOW (但不連線)
  WiFi.mode(WIFI_STA);

  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(OnDataRecv);
  }
  
  // (進階) 在這裡可以呼叫 xTaskCreatePinnedToCore 來啟動專屬的控制迴圈
}

void loop() {
  // 保持淨空，不處理網路
}
```

## 5. 如何將網頁燒錄進 ESP32？
1. 請在您的電腦上執行 Python 腳本：`python pack_to_header.py`。
2. 它會自動下載離線版的 `nipplejs.min.js`，並將所有網頁檔案轉碼成一個 `WebUI.h` 的 C++ 檔案。
3. 將這個 `WebUI.h` 放到您的 Arduino 專案資料夾裡面。
4. 照著上面的程式碼寫 `#include "WebUI.h"`，然後編譯燒錄到**主板**即可！