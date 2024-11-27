#include "DHT.h"

#define DHTPIN 4    
#define DHTTYPE DHT22   
#define LDRPIN 34 // 定義光敏電阻連接的引腳
#define redLEDPIN 2 
#define greenLEDPIN 15 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx and LDR test!"));

  dht.begin();


  // 設定 LED 引腳為輸出模式
  pinMode(redLEDPIN, OUTPUT);
  pinMode(greenLEDPIN, OUTPUT);

  // 初始化 LED 狀態
  digitalWrite(redLEDPIN, LOW);
  digitalWrite(greenLEDPIN, LOW);
}

void loop() {
  // 等待兩秒進行下一次測量
  delay(2000);

  // 讀取溫度與濕度
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // 檢查讀取是否成功
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // 讀取光敏電阻值
  int ldrValue = analogRead(LDRPIN);
  // 將光敏電阻值轉換為0-100的百分比
  float ldrPercentage = map(ldrValue, 0, 1023, 0, 100);

  // 計算總和
  float total = h + t + ldrPercentage;

  // 輸出讀取的數值與總和
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  LDR: "));
  Serial.print(ldrPercentage);
  Serial.print(F("%  Total: "));
  Serial.println(total);
  //
  if(total > 90){
    digitalWrite(redLEDPIN, HIGH);
    digitalWrite(greenLEDPIN, LOW);
  }
  else {
    digitalWrite(redLEDPIN, LOW);
    digitalWrite(greenLEDPIN, HIGH);
  }
}
