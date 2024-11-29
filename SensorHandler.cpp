#include <Arduino.h>
#include "Config.h"       // 引入 WiFi 和 MQTT 設定
#include "SensorHandler.h"
#include "DHT.h"

 
DHT dht(DHT_PIN, DHT_TYPE);

// 全域變數初始化
int lightIntensity = 0;
int humidity = 0;
int temperature = 0;


void sensorSetup() {
    dht.begin();
}

void updateSensorData() {
    lightIntensity = analogRead(LDRPIN);
    humidity = dht.readHumidity() * 2;
    temperature = dht.readTemperature() * 10;
    //lightIntensity = 50;
    //humidity = 100;
    //temperature = 300;
}

void sendLightData(PubSubClient &client) {
    char hexLight[7];
    sprintf(hexLight, "%06x", lightIntensity); // 格式化光敏數據為 6 位 HEX

    String message = String("[\n") +
                     "  {\n" +
                     "    \"macAddr\": \"" + String(DEVICE_MAC) + "\",\n" +
                     "    \"data\": \"" + String(LIGHT_MESSAGE_TYPE) + String(hexLight) + "\"\n" +
                     "  }\n" +
                     "]";

    if (client.publish(MQTT_UPLINK_TOPIC, message.c_str())) {
        Serial.println("Light data sent: " + message);
    } else {
        Serial.println("Failed to send light data");
    }
}


void sendTemperatureData(PubSubClient &client) {
    char hexTemp[5];
    sprintf(hexTemp, "%04x", temperature); // 格式化溫度數據為 4 位 HEX

    String message = String("[\n") +
                     "  {\n" +
                     "    \"macAddr\": \"" + String(DEVICE_MAC) + "\",\n" +
                     "    \"data\": \"" + String(TEMPERATURE_MESSAGE_TYPE) + String(hexTemp) + "\"\n" +
                     "  }\n" +
                     "]";

    if (client.publish(MQTT_UPLINK_TOPIC, message.c_str())) {
        Serial.println("Temperature data sent: " + message);
    } else {
        Serial.println("Failed to send temperature data");
    }
}



void sendHumidityData(PubSubClient &client) {
    char hexHumi[3];
    sprintf(hexHumi, "%02x", humidity); // 格式化濕度數據為 2 位 HEX

    String message = String("[\n") +
                     "  {\n" +
                     "    \"macAddr\": \"" + String(DEVICE_MAC) + "\",\n" +
                     "    \"data\": \"" + String(HUMIDITY_MESSAGE_TYPE) + String(hexHumi) + "\"\n" +
                     "  }\n" +
                     "]";

    if (client.publish(MQTT_UPLINK_TOPIC, message.c_str())) {
        Serial.println("Humidity data sent: " + message);
    } else {
        Serial.println("Failed to send humidity data");
    }
}