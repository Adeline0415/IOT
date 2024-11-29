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

#if defined(ESP32)
const int ADC_MAX = 4095;     // ESP32 為 12 位 ADC
#else
const int ADC_MAX = 1023;     // Arduino 為 10 位 ADC
#endif

// 計算 Lux 的函數
int calculateLux(int adcValue) {
    if (adcValue <= 0) {  // 避免 ADC 值為 0
        return 0;
    }

    // 計算輸出電壓
    float vOut = (adcValue / (float)ADC_MAX) * V_IN;
    if (vOut < 0.1) {  // 避免 vOut 過小
        return 0;
    }

    // 計算光敏電阻的電阻值
    float rLDR = R_FIXED * (V_IN / vOut - 1.0);
    if (rLDR <= 0) {  // 避免電阻值異常
        return 0;
    }

    // 計算 Lux
    float lux = pow(rLDR / R10, -1.0 / GAMMA);

    // 返回整數 Lux
    return round(lux);
}


void updateSensorData() {
    lightIntensity = calculateLux( analogRead(LDRPIN) );
    //lightIntensity = analogRead(LDRPIN);
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