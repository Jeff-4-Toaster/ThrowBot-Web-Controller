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
  
  // 必須設定為 STA 模式並啟動，才能讀取到正確的 MAC Address
  WiFi.mode(WIFI_STA);
  WiFi.begin(); // 確保 WiFi 晶片已喚醒
  delay(100);
  
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

// 顯式設定固定的 IP 網段，加速 DHCP 握手並防止手機連線失敗
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

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
  delay(1000); // 讓序列埠輸出穩定
  
  // 1. 改用 AP_STA 模式，載入完整的雙界面狀態機，避免單獨 AP 模式與 ESP-NOW 衝突
  WiFi.mode(WIFI_AP_STA);
  
  // 2. 顯式設定網段，保證手機連線時能穩定且快速獲得 IP 派發
  WiFi.softAPConfig(local_IP, gateway, subnet);
  
  // 3. 強制將熱點固定在 Channel 1
  if (WiFi.softAP(ssid, password, 1)) {
    Serial.println("Wi-Fi 熱點 (SoftAP) 啟動成功！");
    Serial.print("熱點 IP 位置: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Wi-Fi 熱點啟動失敗！");
  }
  
  // 4. 初始化 ESP-NOW 並註冊副板
  if (esp_now_init() == ESP_OK) {
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 1; // 必須與 softAP 頻道 (Channel 1) 一致
    peerInfo.encrypt = false;
    peerInfo.ifidx = WIFI_IF_AP; // 明確指定為 AP 介面，避免 3.x 版本崩潰
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
  
  Serial.println("主板初始化完成，Wi-Fi 伺服器已啟動！");
}

void loop() {
  server.handleClient();
  webSocket.loop();
  
  // 給系統留有 1 毫秒處理背景 Wi-Fi 任務與握手，防止 CPU 被完全霸佔而導致連線不穩
  delay(1);
}
```

### 【副板】馬達控制中心 (純 ESP-NOW)
副板**不需要**開啟 Wi-Fi 基地台，也不需要連上主板的 Wi-Fi，專心用 ESP-NOW 收指令即可。
```cpp
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // 需要引入此標頭檔來強制鎖定頻道

#ifndef LED_BUILTIN
// 若您的板子編譯時找不到 LED_BUILTIN，請手動將它改為對應的腳位（通常為 2）
#define LED_BUILTIN 2 
#endif

typedef struct struct_message {
    int x;
    int y;
} struct_message;

struct_message myData;
unsigned long lastRecvTime = 0; // 用於追蹤最後一次收到訊號的時間

// ESP-NOW 接收回呼函數 (自動觸發)
// 注意：以下寫法為 ESP32 Core 3.x 以上專用。
void OnDataRecv(const esp_now_recv_info_t * esp_now_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // 紀錄收到資料的時間
  lastRecvTime = millis();
  
  // 點亮內建藍燈表示通訊中
  digitalWrite(LED_BUILTIN, HIGH); 
  
  // 印出偏移座標讓您在 Serial Monitor 看到變化
  Serial.printf("收到搖桿指令 -> X: %4d, Y: %4d\n", myData.x, myData.y);
  
  // TODO: 在這裡或者在 Core 1 的 Task 裡面更新馬達 PWM
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // 初始狀態熄滅
  
  // 必須設定為 STA 模式才能使用 ESP-NOW (但不連線)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // 確保沒有在背景連線
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // 強制鎖定在 Channel 1 與主板同步

  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("ESP-NOW 初始化成功，等待主板訊號...");
  } else {
    Serial.println("ESP-NOW 初始化失敗");
  }
}

void loop() {
  // 如果超過 500 毫秒沒有收到任何主板訊號，視為斷線並熄滅 LED
  if (millis() - lastRecvTime > 500 && lastRecvTime != 0) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
```

## 5. 如何將網頁燒錄進 ESP32？
1. 請在您的電腦上執行 Python 腳本：`python pack_to_header.py`。
2. 它會自動下載離線版的 `nipplejs.min.js`，並將所有網頁檔案轉碼成一個 `WebUI.h` 的 C++ 檔案。
3. 將這個 `WebUI.h` 放到您的 Arduino 專案資料夾裡面。
4. 照著上面的程式碼寫 `#include "WebUI.h"`，然後編譯燒錄到**主板**即可！
