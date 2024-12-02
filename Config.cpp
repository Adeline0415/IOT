#include "Config.h"
#include <Arduino.h>
// 更新週期
const unsigned long UPDATE_PERIOD = 5000;

// DHT 配置
const int DHT_PIN = 4;
const int DHT_TYPE = 22;  // DHT22 的值定義為 22，與 DHT 庫一致

// 光敏
const int LDRPIN = 34;
const int R_FIXED = 1000;     // 固定電阻，1 kΩ (棕黑紅金)

const float V_IN = 3.3;

// 光敏電阻特性參數
const float R10 = 5000.0;     // 10 Lux 下的電阻值 (5kΩ，亮電阻中值)
const float GAMMA = 0.6;      // 伽馬值 (γ)

const int RAIN_DIGITAL = 18;
const int RAIN_ANALOG = 35;

// 裝置相關配置
const char DEVICE_MAC[] = "00000000aaBB0D838";

// 訊息類型
const char LIGHT_MESSAGE_TYPE[] = "0265";
const char TEMPERATURE_MESSAGE_TYPE[] = "0267";
const char HUMIDITY_MESSAGE_TYPE[] = "0268";
const char RAINING_MESSAGE_TYPE[] = "028e";

// WiFi 設定
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// MQTT 設定
const char* MQTT_SERVER = "mqtt-pbl.kits.tw";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "school";
const char* MQTT_PASSWORD = "nthu-hsnl";

// MQTT Topic 設定
const char* MQTT_UPLINK_TOPIC = "PBL/UL/";
const char* MQTT_DOWNLINK_TOPIC = "WAN-GW/DL/WetChicken";
