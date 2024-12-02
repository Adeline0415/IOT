#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include <PubSubClient.h>

// 全域變數宣告
extern int lightIntensity;
extern int humidity;
extern int temperature;

// 初始化感測器和全域變數
void sensorSetup();

// 更新感測器數據
void updateSensorData();

// 發送光敏電阻數據
void sendLightData(PubSubClient &client);
int calculateLux(int adcValue);

// 發送溫度數據
void sendTemperatureData(PubSubClient &client);

// 發送濕度數據
void sendHumidityData(PubSubClient &client);

// 發送下雨判斷
void sendRainingData(PubSubClient &client);


#endif
