// #include "main.h"
// #include "hal_LCD.h"
// #include "TempSensorMode.h"

// // Backup Memory variables to track states through LPM3.5
// volatile unsigned char * S1buttonDebounce = &BAKMEM2_L;       // S1 button debounce flag
// volatile unsigned char * S2buttonDebounce = &BAKMEM2_H;       // S2 button debounce flag
// volatile unsigned char * stopWatchRunning = &BAKMEM3_L;       // Stopwatch running flag
// volatile unsigned char * tempSensorRunning = &BAKMEM3_H;      // Temp Sensor running flag
// volatile unsigned char * mode = &BAKMEM4_L;                   // mode flag
// volatile unsigned int holdCount = 0;
// // FRAM 地址，用於存儲溫度數據
// volatile unsigned int *framTemperature = (unsigned int *)FRAM_TEMP_ADDRESS; // FRAM address for temperature storage
// #define TEMP_THRESHOLD 30  // 溫度閾值，單位為攝氏度
// #define DEFAULT_TEMPERATURE 250  // 預設溫度值 (25.0°C)

// /*
//  * main.c
//  */
// #include "main.h"
// #include "hal_LCD.h"
// #include "TempSensorMode.h"

// int main(void) {
//     // 停止 Watchdog 計時器
//     WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);

//     // 初始化硬體資源
//     Init_GPIO();   // 初始化 GPIO
//     Init_LCD();    // 初始化 LCD

//     // 啟動溫度感測模式
//     tempSensorModeInit();

//     while (1) {
//         // 偵測並計算目前的溫度
//         tempSensor();

//         // 更新 LCD 顯示目前溫度
//         displayTemp();

//         // 延遲一段時間以減少頻繁刷新 (例如每秒更新一次)
//         __delay_cycles(1000000); // 假設系統頻率為 1 MHz
//     }

//     return 0; // 這行通常不會被執行，僅作為保險


//     // // 初始化外圍設備
//     // Init_GPIO();
//     // Init_LCD();
//     // displayScrollText("112526011 LAB 1");

//     // // 初始化溫度感測模式
//     // clearLCD(); 
//     // tempSensorModeInit();
//     // tempSensor();

//     // // 檢查並從 FRAM 中檢索上次存儲的溫度數據
//     // if (*framTemperature > 1000 || *framTemperature <= 0) {
//     //     *framTemperature = DEFAULT_TEMPERATURE;  // 設置預設溫度
//     // }
//     // unsigned int lastTemperature = *framTemperature;
//     // *degC = lastTemperature;  // 設定上次的攝氏溫度

//     // displayScrollText("LAST");
//     // displayTemp();

//     // displayScrollText("C111");
//     // displayScrollText("CURRENT TEMP");
    
//     // while (1) {
//     //     // displayScrollText("C222");
//     //     // 啟動溫度感測
//     //     tempSensor();

//     //     // 儲存目前的溫度到 FRAM
//     //     unsigned int currentTemp = *degC;  // 使用 TempSensorMode 的 degC
//     //     *framTemperature = currentTemp;

//     //     // 比較目前溫度是否超過閾值
//     //     if (currentTemp > TEMP_THRESHOLD * 10) {  // 攝氏溫度以 0.1 度為單位
//     //         GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); // 開啟 LED
//     //     } else {
//     //         GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  // 關閉 LED
//     //     }

//     //     // 顯示目前溫度
//     //     // displayScrollText("CURRENT TEMP:");
//     //     displayTemp();

//     //     // 進入低功耗模式
//     //     __bis_SR_register(LPM3_bits | GIE);
//     //     __no_operation();
//     // }
// }

// /*
//  * GPIO 初始化
//  */
// void Init_GPIO() {
//     // 配置 LED 腳位
//     GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // P1.0 作為輸出 (LED)
//     GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // 初始關閉 LED

//     // 禁用高阻抗模式以啟用配置
//     PMM_unlockLPM5();
// }

// /*
//  * Real Time Clock counter Initialization
//  */
// void Init_RTC()
// {
//     // Set RTC modulo to 327-1 to trigger interrupt every ~10 ms
//     RTC_setModulo(RTC_BASE, 326);
//     RTC_enableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
// }


