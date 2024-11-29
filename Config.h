#ifndef CONFIG_H
#define CONFIG_H

// 定義 DEBUG_MODE 宏以啟用除錯模式
#define DEBUG_MODE

// GPIO PIN 配置
const int PIN_LED = 2;      // LED Pin
const int PIN_LED_R = 4;      // LED Pin

// GPIO 初始狀態
const bool INITIAL_LED_STATE = LOW;
const bool INITIAL_LED_R_STATE = HIGH;
//const bool INITIAL_BUTTON_MODE = INPUT_PULLUP;


// 裝置相關配置
const char DEVICE_MAC[] = "00000000aaBB0D838"; // 裝置 MAC 地址

// 訊息類型
const char LIGHT_MESSAGE_TYPE[] = "0265";      // 光度訊息類型

// WiFi 設定
const char* WIFI_SSID = "shunelle";
const char* WIFI_PASSWORD = "0967020204";

// MQTT 設定
const char* MQTT_SERVER = "mqtt-pbl.kits.tw";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "school";
const char* MQTT_PASSWORD = "nthu-hsnl";

// MQTT Topic 設定
const char* MQTT_UPLINK_TOPIC = "PBL/UL/";
const char* MQTT_DOWNLINK_TOPIC = "WAN-GW/DL/WetChicken";



#endif
