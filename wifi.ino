#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi 設定
const char* ssid = "SHUN iPhone";
const char* password = "0967020204";

// MQTT Broker 設定
const char* mqtt_server = "mqtt-pbl.kits.tw";
const int mqtt_port = 1883;
const char* mqtt_user = "school";
const char* mqtt_password = "nthu-hsnl";

// MQTT Topic 設定
const char* topic = "PBL/UL/";       // 上傳的 Topic
const char* downlinkTopic = "WAN-GW/DL/WetChicken"; // 接收下行訊息的 Topic

WiFiClient espClient;
PubSubClient client(espClient);
#define DHTPIN 4    
#define DHTTYPE DHT22   
const int ldrPin = 34; // 光敏電阻接到 GPIO34 (ADC1)
const int redPin = 2; // 紅色 LED
const int greenPin = 15; // 紅色 LED
// 設定上次傳送的時間
unsigned long lastMsg = 0;
DHT dht(DHTPIN, DHTTYPE);

struct MessageResult {
    String TT; // GPIO pin number
    String CC; // Control Type
    bool isValid; // 是否成功解析
};

// 增加 GPIO 控制相關函數
void controlGPIO(int pin, bool state, unsigned long duration = 0) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
    
    // 發送 ACK2
    char ack2[9];
    sprintf(ack2, "002F%02x%02x", pin, state ? 0x01 : 0x00);
    
    String ack2Message = "[\n"
                        "  {\n"
                        "    \"macAddr\": \"chickchick\",\n"
                        "    \"data\": \"" + String(ack2) + "\"\n"
                        "  }\n"
                        "]";
                        
    client.publish(downlinkTopic, ack2Message.c_str());
    Serial.println("ACK2 sent: " + ack2Message);
}

// 回調函數，用於處理下行訊息
void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Received message: " + message);

    // 提取 JSON "data" 欄位的值
    int dataStart = message.indexOf("\"data\": \"") + 9;
    int dataEnd = message.indexOf("\"", dataStart);
    if (dataStart < 9 || dataEnd == -1) {
        Serial.println("Invalid message format");
        return;
    }

    String data = message.substring(dataStart, dataEnd);
    Serial.println("Extracted data: " + data);

    // 回傳 ACK1
    String ack1 = "00" + data;
    String ack1Message = "[\n"
                         "  {\n"
                         "    \"macAddr\": \"chickchick\",\n"
                         "    \"data\": \"" + ack1 + "\"\n"
                         "  }\n"
                         "]";
    client.publish(downlinkTopic, ack1Message.c_str());
    Serial.println("ACK1 sent: " + ack1Message);

    // 判斷訊息類型並執行控制邏輯
    if (data.startsWith("1F") || data.startsWith("0F")) {
        int pin = strtol(data.substring(2, 4).c_str(), NULL, 16);  // 提取 GPIO pin
        bool state = (data.substring(4, 6) == "01");              // 開啟或關閉
        unsigned long duration = 0;

        if (data.startsWith("1F")) {  // 帶時間參數的控制
            duration = strtol(data.substring(6).c_str(), NULL, 16);
        }
        controlGPIO(pin, state, duration);  // 控制 GPIO 在此function 回傳 ACK2
    }
}


void setup() {
    Serial.begin(115200);
    pinMode(redPin, OUTPUT);

    // 連接 WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected");

    // 設定 MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback); // 設定回調函數

    // 連接 MQTT
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
            Serial.println("MQTT connected");

            // 訂閱下行訊息的 Topic
            client.subscribe(downlinkTopic);
            Serial.println("Subscribed to topic: " + String(downlinkTopic));
        } else {
            Serial.print("Failed to connect, state: ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void loop() {
    client.loop();  // 保持 MQTT 連線

    // 每隔 5 秒上傳一次光敏電阻數據
    unsigned long now = millis();
    if (now - lastMsg > 5000) {
        lastMsg = now;

        // 讀取光敏電阻的 ADC 值
        int lightIntensity = analogRead(ldrPin); // 0~4095
        // 讀取溫度與濕度
        int humidity = dht.readHumidity();
        int temperature = dht.readTemperature();
        float f = dht.readTemperature(true);

        Serial.print("LDR Value: ");
        Serial.println(lightIntensity);
        Serial.print(F("Humidity: "));
        Serial.print(humidity);
        Serial.print(F("%  Temperature: "));
        Serial.print(temperature);

        // 將光強值轉換為大端格式的十六進位字串
        char hexLight[7];
        sprintf(hexLight, "%06x", lightIntensity); // 格式化為 6 位大端 HEX 值
        // 格式化 MQTT 訊息
        String lightMessage = "[\n"
                         "  {\n"
                         "    \"macAddr\": \"00000000aaBB0D838\",\n"
                         "    \"data\": \"0265" + String(hexLight) + "\"\n"
                         "  }\n"
                         "]";
        // 發佈到 MQTT 平台
        if (client.publish(topic, lightMessage.c_str())) {
            Serial.println("lightMessage sent: " + lightMessage);
        } else {
            Serial.println("Failed to send lightMessage");
        }

        // 將溫度轉換為大端格式（big-endian）的十六進位字串
        char hexTemp[5];
        sprintf(hexTemp, "%04x", temperature);  // 轉為 4 位小端 HEX 格式
        // 格式化 JSON 訊息
        String tempMessage = "[\n"
                         "  {\n"
                         "    \"macAddr\": \"00000000aaBB0D838\",\n"
                         "    \"data\": \"0267" + String(hexTemp) + "\"\n"
                         "  }\n"
                         "]";

        // 發佈到 MQTT 平台
        if (client.publish(topic, tempMessage.c_str())) {
            Serial.println("tempMessage sent: " + tempMessage);
        } else {
            Serial.println("Failed to send tempMessage");
        }

        // 將濕度轉換為大端格式（big-endian）的十六進位字串
        char hexHumi[3];
        sprintf(hexHumi, "%02x", humidity);  // 轉為 4 位小端 HEX 格式
        // 格式化 JSON 訊息
        String humiMessage = "[\n"
                         "  {\n"
                         "    \"macAddr\": \"00000000aaBB0D838\",\n"
                         "    \"data\": \"0268" + String(hexHumi) + "\"\n"
                         "  }\n"
                         "]";

        // 發佈到 MQTT 平台
        if (client.publish(topic, humiMessage.c_str())) {
            Serial.println("humiMessage sent: " + humiMessage);
        } else {
            Serial.println("Failed to send humiMessage");
        }



    }
}



MessageResult processDownlinkMessage(String message, String downlinkTopic, const char* topic) {
    MessageResult result = {"", "", false}; // 初始化返回結果

    if (String(topic) == downlinkTopic) {
        Serial.println("Downlink message received.");
        
        // 簡單解析 JSON 格式的 "data" 值
        int dataStart = message.indexOf("\"data\": \"") + 9; // 找到 "data": 的開始
        int dataEnd = message.indexOf("\"", dataStart);     // 找到結束引號
        if (dataStart < 9 || dataEnd == -1) {
            Serial.println("Invalid message format");
            return result; // 返回無效結果
        }

        String data = message.substring(dataStart, dataEnd);

        // 驗證標頭 "0F"
        if (data.substring(0, 2) == "0F") {
            // 取得 TT 和 CC
            result.TT = data.substring(2, 4); // GPIO pin number
            result.CC = data.substring(4, 6); // Control Type
            result.isValid = true; // 設置為有效結果
        } else {
            Serial.println("Invalid header in data.");
        }
    }

    return result;
}

