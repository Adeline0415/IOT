#include "Timer.h"

// 全域計時器陣列
TimerTask timerTasks[MAX_TIMERS];

// 初始化計時器列表
void initializeTimers() {
    for (int i = 0; i < MAX_TIMERS; i++) {
        timerTasks[i].isActive = false; // 初始化為未啟用
    }
}

// 新增計時器
bool addTimer(int pin, unsigned long delaySeconds) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!timerTasks[i].isActive) { // 找到空閒計時器槽位
            timerTasks[i].pin = pin;
            timerTasks[i].endTime = millis() + delaySeconds * 1000; // 計算結束時間
            timerTasks[i].isActive = true;
            pinMode(pin, OUTPUT); // 設置為輸出模式
            digitalWrite(pin, HIGH); // 啟動引腳
            return true; // 計時器成功加入
        }
        else{
          if(timerTasks[i].pin == pin){
            break;
          }
        }
    }
    return false; // 無可用計時器槽位
}

// 更新計時器狀態
void updateTimers() {
    unsigned long currentTime = millis();
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timerTasks[i].isActive && currentTime >= timerTasks[i].endTime) {
            digitalWrite(timerTasks[i].pin, LOW); // 計時結束後關閉引腳
            timerTasks[i].isActive = false;       // 停用計時器
        }
    }
}
