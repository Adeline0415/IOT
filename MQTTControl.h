#ifndef MQTTCONTROL_H
#define MQTTCONTROL_H

#include <Arduino.h>

// Data structure for MQTT message parsing
struct MessageResult {
    String macAddr;
    String data;
    bool isValid;
};

// Data structure for control parameters
struct ControlParams {
    int pin;               // GPIO 引腳號碼
    bool isOn;             // 狀態：是否開啟
    unsigned long delaySeconds; // 延遲關閉時間（秒）
    String type;           // 訊息類型 ("1F" 或 "0F")
    bool isValid;          // 是否為有效參數
};


// Function declarations
MessageResult extractMessage(String message);
ControlParams parseControlParams(String data);
void controlGPIO(ControlParams params);

#endif
