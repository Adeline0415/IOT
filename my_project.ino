#include "Config.h"       // 引入 WiFi 和 MQTT 設定
#include "Utility.h"  // for debug
#include <WiFi.h>
#include <PubSubClient.h>
#include "MQTTControl.h" // 引入模組化函式庫
#include "timer.h"
#include "SensorHandler.h"

#define ENABLEA 32
#define ENABLEB 14

WiFiClient espClient;
PubSubClient client(espClient);

// MQTT 回調函式
void callback(char* topic, byte* payload, unsigned int length) {
    // 列印接收到的主題
    DEBUG_PRINT("Received topic: ");
    DEBUG_PRINT(topic);

    // 將 payload 轉換為字串
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    // 列印接收到的完整消息
    DEBUG_PRINT("Received message: ");
    DEBUG_PRINT(message);

    // 提取並解析 MQTT 消息
    MessageResult result = extractMessage(message);
    if (result.isValid) {
        // 列印提取的 macAddr 和 data
        DEBUG_PRINT_VAR("Extracted macAddr: ", result.macAddr);
        DEBUG_PRINT_VAR("Extracted data: ", result.data );

        // 解析控制參數
        ControlParams params = parseControlParams(result.data);
        if (params.isValid) {
            // 控制 GPIO
            controlGPIO(params);

            // 列印控制結果
            DEBUG_PRINT("GPIO control executed.");
        } else {
            DEBUG_PRINT("Error: Invalid control parameters.");
        }
    } else {
        DEBUG_PRINT("Error: Failed to extract message.");
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
    pinMode(27, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(35, INPUT);
    pinMode(18, INPUT);
    pinMode(ENABLEB, OUTPUT);
    pinMode(ENABLEA, OUTPUT);
    digitalWrite(ENABLEA, HIGH);
    digitalWrite(ENABLEB, HIGH);
}


void setup() {
    Serial.begin(115200);
    initializePins();
    sensorSetup();
    setupWiFi();
    setupMQTT();
    initializeTimers(); // 初始化計時器
    analogReadResolution(10);
}

unsigned long lastMsg = 0;   // 上次消息發布時間
void loop() {
    reconnectMQTT(); // 確保 MQTT 連線

    unsigned long now = millis();

    // 更新速度
    if (now - lastMsg > UPDATE_PERIOD) {
        lastMsg = now;

        // 更新感測器數據
        updateSensorData();

        // 發送感測器數據
        sendLightData(client);
        sendTemperatureData(client);
        sendHumidityData(client);
        sendRainingData(client);
    }

    // 更新多計時器狀態
    updateTimers();

    // 確保 MQTT 客戶端正常運行
    client.loop();
}
