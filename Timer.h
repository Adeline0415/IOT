#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

// 定義計時器結構
struct TimerTask {
    int pin;                     // GPIO pin
    unsigned long endTime;       // 計時器結束時間
    bool isActive;               // 計時器是否啟用
};

// 最大計時器數量
const int MAX_TIMERS = 10;

// 全域計時器陣列
extern TimerTask timerTasks[MAX_TIMERS];

// 初始化計時器
void initializeTimers();

// 新增計時器
bool addTimer(int pin, unsigned long delaySeconds);

// 更新計時器狀態（需要在 loop() 中呼叫）
void updateTimers();

#endif
