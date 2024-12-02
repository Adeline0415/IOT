#include "MQTTControl.h"
#include "timer.h"
#include "Utility.h"  // for debug

MessageResult extractMessage(String message) {
    DEBUG_PRINT("Debug: Starting to extract message...");
    MessageResult result;

    // 提取 "data" 欄位的值
    int dataStart = message.indexOf("\"data\": \"") + 9; // 找到 "data" 的起始位置
    int dataEnd = message.indexOf("\"", dataStart);      // 找到 "data" 的結尾位置
    if (dataStart < 9 || dataEnd == -1) {               // 檢查格式有效性
        DEBUG_PRINT("Error: Invalid message format.");
        result.isValid = false;
        return result;
    }

    // 提取數據
    result.data = message.substring(dataStart, dataEnd);
    result.isValid = true;

    DEBUG_PRINT_VAR("Debug: Extracted data: ", result.data);

    return result;
}




ControlParams parseControlParams(String data) {
    DEBUG_PRINT("Debug: Parsing control parameters...");
    ControlParams params;

    // 檢查訊息類型
    if (data.startsWith("1F") || data.startsWith("0F")) {
        params.type = data.substring(0, 2);             // 訊息類型 ("1F" 或 "0F")
        params.pin = strtol(data.substring(2, 4).c_str(), NULL, 16);  // 提取 GPIO pin
        params.isOn = (data.substring(4, 6) == "01");  // 狀態（開/關）

        // 如果是 "1F"，提取計時器資訊
        if (params.type == "1F" && data.length() >= 8) {
            params.delaySeconds = strtol(data.substring(6).c_str(), NULL, 16);
        } else {
            params.delaySeconds = 0; // 無計時器
        }

        params.isValid = true;

        // 列印解析後的參數
        DEBUG_PRINT_VAR("Debug: Parsed type: ", params.type);
        DEBUG_PRINT_VAR("Debug: Parsed pin: ", params.pin);
        DEBUG_PRINT_VAR("Debug: Parsed state: ", params.isOn ? "ON" : "OFF");
        DEBUG_PRINT_VAR("Debug: Parsed delaySeconds: ", params.delaySeconds);
    } else {
        DEBUG_PRINT("Error: Data does not match control format.");
        params.isValid = false;
    }

    return params;
}

void controlGPIO(ControlParams params) {
    if (!params.isValid) {
        DEBUG_PRINT("Error: Invalid control parameters. GPIO control skipped.");
        return;
    }

    DEBUG_PRINT("Debug: Controlling GPIO...");
    DEBUG_PRINT_VAR("Debug: Message type: ", params.type);
    DEBUG_PRINT_VAR("Debug: Setting pin ", params.pin);
    DEBUG_PRINT_VAR("Debug: Initial state: ", params.isOn ? "HIGH" : "LOW");

    pinMode(params.pin, OUTPUT);

    // 若為 "1F" 類型且設有計時器，加入計時器
    if (params.type == "1F" && params.delaySeconds > 0) {
        if (!addTimer(params.pin, params.delaySeconds)) {
            DEBUG_PRINT("Error: No available timer slots.");
        } else {
            DEBUG_PRINT_VAR("Debug: Timer successfully set for pin ", params.pin);
        }
    }
    else{
      digitalWrite(params.pin, params.isOn);
    }
}

