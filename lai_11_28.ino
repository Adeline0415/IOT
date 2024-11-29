#include "Config.h"       // 引入 WiFi 和 MQTT 設定
#include <WiFi.h>
#include <PubSubClient.h>
#include "MQTTControl.h" // 引入模組化函式庫
#include "timer.h"


WiFiClient espClient;
PubSubClient client(espClient);

// MQTT 回調函式
void callback(char* topic, byte* payload, unsigned int length) {
    // 列印接收到的主題
    Serial.print("Received topic: ");
    Serial.println(topic);

    // 將 payload 轉換為字串
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    // 列印接收到的完整消息
    Serial.print("Received message: ");
    Serial.println(message);

    // 提取並解析 MQTT 消息
    MessageResult result = extractMessage(message);
    if (result.isValid) {
        // 列印提取的 macAddr 和 data
        Serial.print("Extracted macAddr: ");
        Serial.println(result.macAddr);
        Serial.print("Extracted data: ");
        Serial.println(result.data);

        // 解析控制參數
        ControlParams params = parseControlParams(result.data);
        if (params.isValid) {
            // 控制 GPIO
            controlGPIO(params);

            // 列印控制結果
            Serial.println("GPIO control executed.");
        } else {
            Serial.println("Error: Invalid control parameters.");
        }
    } else {
        Serial.println("Error: Failed to extract message.");
    }
}


// 設定 WiFi 連線
void setupWiFi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
}

// 設定 MQTT 客戶端
void setupMQTT() {
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
    reconnectMQTT();
}

// 連線到 MQTT Broker
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Connected to MQTT broker");
            client.subscribe(MQTT_DOWNLINK_TOPIC); // 訂閱下行訊息的 Topic
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}

void initializePins() {
    // 設置 LED 引腳
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, INITIAL_LED_STATE);
    pinMode(PIN_LED_R, OUTPUT);
    digitalWrite(PIN_LED_R, INITIAL_LED_R_STATE);
    
/*
    // 設置按鈕引腳
    pinMode(PIN_BUTTON, INITIAL_BUTTON_MODE);
    DEBUG_PRINT("Button pin initialized.");

    // 設置繼電器引腳
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_RELAY, INITIAL_RELAY_STATE);
    DEBUG_PRINT("Relay pin initialized.");
*/
}


void setup() {
    Serial.begin(115200);
    initializePins();
    setupWiFi();
    setupMQTT();
    initializeTimers(); // 初始化計時器
}

unsigned long lastMsg = 0;   // 上次消息發布時間
bool highLight = true;       // 模擬光度狀態

// LED 閃爍相關設定
const unsigned long BLINK_INTERVAL = 500; // 閃爍間隔（毫秒）
bool ledState = false;          // LED 當前狀態
unsigned long lastBlinkTime = 0; // 上次閃爍時間

void loop() {
    reconnectMQTT(); // 確保 MQTT 連線

    unsigned long now = millis();

    // 定期模擬光度值並發佈
    if (now - lastMsg > 5000) {
        lastMsg = now;

        // 模擬光度值
        int lightIntensity = highLight ? 1200 : 1200;
        highLight = !highLight;

        // 將光強值轉換為大端格式的十六進位字串
        char hexLight[7];
        sprintf(hexLight, "%06x", lightIntensity);

        // 格式化 MQTT 訊息
        String lightMessage = "[\n"
                              "  {\n"
                              "    \"macAddr\": \"" + String(DEVICE_MAC) + "\",\n"
                              "    \"data\": \"" + String(LIGHT_MESSAGE_TYPE) + String(hexLight) + "\"\n"
                              "  }\n"
                              "]";

        // 發佈到 MQTT 平台
        if (client.publish(MQTT_UPLINK_TOPIC, lightMessage.c_str())) {
            Serial.println("lightMessage sent: " + lightMessage);
        } else {
            Serial.println("Failed to send lightMessage");
        }
    }

    // LED 閃爍邏輯
    if (now - lastBlinkTime > BLINK_INTERVAL) {
        lastBlinkTime = now;       // 更新上次閃爍時間
        ledState = !ledState;      // 切換 LED 狀態
        digitalWrite(PIN_LED_R, ledState ? HIGH : LOW); // 更新 LED 狀態
    }

    // 更新多計時器狀態
    updateTimers();

    // 確保 MQTT 客戶端正常運行
    client.loop();
}


