#ifndef CONFIG_H
#define CONFIG_H

// 定義 DEBUG_MODE 宏以啟用除錯模式
#define DEBUG_MODE

// 更新週期
extern const unsigned long UPDATE_PERIOD;

// DHT 配置
extern const int DHT_PIN;
extern const int DHT_TYPE;  // DHT22 的值定義為 22，與 DHT 庫一致

// 光敏
extern const int LDRPIN;

// GPIO PIN 配置
extern const int PIN_LED;
extern const int PIN_LED_R;

// GPIO 初始狀態
extern const bool INITIAL_LED_STATE;
extern const bool INITIAL_LED_R_STATE;

// 裝置相關配置
extern const char DEVICE_MAC[];

// 訊息類型
extern const char LIGHT_MESSAGE_TYPE[];
extern const char TEMPERATURE_MESSAGE_TYPE[];
extern const char HUMIDITY_MESSAGE_TYPE[];

// WiFi 設定
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

// MQTT 設定
extern const char* MQTT_SERVER;
extern const int MQTT_PORT;
extern const char* MQTT_USER;
extern const char* MQTT_PASSWORD;

// MQTT Topic 設定
extern const char* MQTT_UPLINK_TOPIC;
extern const char* MQTT_DOWNLINK_TOPIC;

#endif
