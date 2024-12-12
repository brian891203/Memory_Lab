/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * main.c
 *
 * Out of Box Demo for the MSP-EXP430FR4133
 * Main loop, initialization, and interrupt service routines
 *
 * September 2014
 * E. Chen
 *
 ******************************************************************************/

#include "main.h"
#include "hal_LCD.h"
#include "TempSensorMode.h"

// Backup Memory variables to track states through LPM3.5
volatile unsigned char * S1buttonDebounce = &BAKMEM2_L;       // S1 button debounce flag
volatile unsigned char * S2buttonDebounce = &BAKMEM2_H;       // S2 button debounce flag
volatile unsigned char * stopWatchRunning = &BAKMEM3_L;       // Stopwatch running flag
volatile unsigned char * tempSensorRunning = &BAKMEM3_H;      // Temp Sensor running flag
volatile unsigned char * mode = &BAKMEM4_L;                   // mode flag
volatile unsigned int holdCount = 0;

// TimerA0 UpMode Configuration Parameter
Timer_A_initUpModeParam initUpParam_A0 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/1 = 2MHz
        30000,                                  // 15ms debounce period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR,                       // Clear value
        true                                    // Start Timer
};

/*
 * main.c
 */
int main(void) {
    // Stop Watchdog timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);     // Stop WDT

    // Check if a wakeup from LPMx.5
    if (SYSRSTIV == SYSRSTIV_LPM5WU)
    {
        Init_GPIO();
        __enable_interrupt();

        tempSensor();
    }
    else
    {
        // Initializations
        Init_GPIO();
        Init_Clock();
        Init_RTC();
        Init_LCD();

        GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
        GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6);

        *S1buttonDebounce = *S2buttonDebounce = *stopWatchRunning = *tempSensorRunning = *mode = 0;

        __enable_interrupt();
    }

    while(1)
    {
        LCD_E_selectDisplayMemory(LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY);

        //RTC_setModulo(RTC_BASE, 8191);
        //RTC_enableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
        //RTC_start(RTC_BASE, RTC_CLOCKSOURCE_XT1CLK);
        if (*mode == STARTUP_MODE)
        {
            RTC_setModulo(RTC_BASE, 8191);
            RTC_enableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
            RTC_start(RTC_BASE, RTC_CLOCKSOURCE_XT1CLK);
            clearLCD();
            displayScrollText("112526011 LAB 1");
        }
        else
        {
            clearLCD();
            tempSensorModeInit();
            tempSensor();
        }

        // if (*mode == SAFE_SHUTDOWN_MODE)
        // {
        //     // 停止進一步操作，並顯示安全模式提示
        //     safeShutdownModeInit();
        // }

        __bis_SR_register(LPM3_bits | GIE);         // enter LPM3
        __no_operation();
    }
}

/*
 * GPIO Initialization
 */
void Init_GPIO()
{
    // Set all GPIO pins to output low to prevent floating input and reduce power consumption
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN1);

    // Configure button S1 (P1.2) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    // Configure button S2 (P2.6) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6);

    // Set P4.1 and P4.2 as Secondary Module Function Input, LFXT.
    GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_P4,
           GPIO_PIN1 + GPIO_PIN2,
           GPIO_PRIMARY_MODULE_FUNCTION
           );

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PMM_unlockLPM5();
}

/*
 * Clock System Initialization
 */
void Init_Clock()
{
    // Intializes the XT1 crystal oscillator
    CS_turnOnXT1(CS_XT1_DRIVE_1);
}

/*
 * Real Time Clock counter Initialization
 */
void Init_RTC()
{
    // Set RTC modulo to 327-1 to trigger interrupt every ~10 ms
    RTC_setModulo(RTC_BASE, 326);
    RTC_enableInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT);
}

/*
 * PORT1 Interrupt Service Routine
 * Handles S1 button press interrupt
 */
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    P1OUT |= BIT0;    // Turn LED1 On
    switch(__even_in_range(P1IV, P1IV_P1IFG7))
    {
        case P1IV_NONE : break;
        case P1IV_P1IFG0 : break;
        case P1IV_P1IFG1 : break;
        case P1IV_P1IFG2 :
            if ((*S1buttonDebounce) == 0)
            {
                *S1buttonDebounce = 1;                        // First high to low transition
                holdCount = 0;

                *flashWUnit ^= 0x01;

                // Start debounce timer
                Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0);
            }
        case P1IV_P1IFG3 : break;
        case P1IV_P1IFG4 : break;
        case P1IV_P1IFG5 : break;
        case P1IV_P1IFG6 : break;
        case P1IV_P1IFG7 : break;
    }
}

/*
 * PORT2 Interrupt Service Routine
 * Handles S2 button press interrupt
 */
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    P4OUT |= BIT0;    // Turn LED2 On
    switch(__even_in_range(P2IV, P2IV_P2IFG7))
    {
        case P2IV_NONE : break;
        case P2IV_P2IFG0 : break;
        case P2IV_P2IFG1 : break;
        case P2IV_P2IFG2 : break;
        case P2IV_P2IFG3 : break;
        case P2IV_P2IFG4 : break;
        case P2IV_P2IFG5 : break;
        case P2IV_P2IFG6 :
            if ((*S2buttonDebounce) == 0)
            {
                *S2buttonDebounce = 1;                        // First high to low transition
                holdCount = 0;
                *tempUnit ^= 0x01;

                // Start debounce timer
                Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0);
            }

        case P2IV_P2IFG7 : break;
    }
}

/*
 * Timer A0 Interrupt Service Routine
 * Used as button debounce timer
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    // Both button S1 & S2 held down
    if (!(P1IN & BIT2) && !(P2IN & BIT6))
    {
        holdCount++;
        if (holdCount == 40)
        {
            // Timer_A_stop(TIMER_A0_BASE);
            if (*mode == STARTUP_MODE)
            {
                displayScrollText("Q1");
                (*mode) = Q1_MODE;
                *flashWUnit =0;
            }
            else if (*mode == Q1_MODE)
            {
                displayScrollText("Q2");
                (*mode) = Q2_MODE;
            }
            else if (*mode == Q2_MODE)
            {
                displayScrollText("Q3");
                (*mode) = Q3_MODE;
            }
            else if (*mode == Q3_MODE)
            {
                displayScrollText("SAFE");
                (*mode) = SAFE_SHUTDOWN_MODE;
                safeShutdownModeInit();
            }
            // else if (*mode == Q3_MODE)
            // {
            //     displayScrollText("Q1");
            //     (*mode) = Q1_MODE;
            // }

            __bic_SR_register_on_exit(LPM3_bits);

        }
    }

    // Button S1 released
    if (P1IN & BIT2)
    {
        *S1buttonDebounce = 0;                                   // Clear button debounce
        P1OUT &= ~BIT0;
    }

    // Button S2 released
    if (P2IN & BIT6)
    {
        *S2buttonDebounce = 0;                                   // Clear button debounce
        P4OUT &= ~BIT0;
    }

    // Both button S1 & S2 released
    if ((P1IN & BIT2) && (P2IN & BIT6))
    {
        // Stop timer A0
        Timer_A_stop(TIMER_A0_BASE);
        if (*mode == Q1_MODE)
                __bic_SR_register_on_exit(LPM3_bits);            // exit LPM3
        if (*mode == Q2_MODE)
                __bic_SR_register_on_exit(LPM3_bits);            // exit LPM3
    }

    // Button S1 被按下而 S2 沒有
    if (!(P1IN & BIT2) && (P2IN & BIT6)) {
        *S1buttonDebounce = 1;  // S1 被按下，設置防抖標誌
        // 在此處執行 S1 的處理邏輯
        // *flashWUnit ^= 0x01;
        // *flashWUnit == 1;
    }

    // if ((P1IN & BIT2) && !(P2IN & BIT6)) {
    //     *S2buttonDebounce = 1;  // S1 被按下，設置防抖標誌
    //     displayScrollText("SAFE");
    //     safeShutdownModeInit();
    // }
}

/*
 * RTC Interrupt Service Routine
 * Wakes up every ~10 milliseconds to update stowatch
 */
#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF))
    {
        case RTCIV_NONE : break;
        case RTCIV_RTCIF:
            __bic_SR_register_on_exit(LPM3_bits);
            break;
    }
}

/*
 * ADC Interrupt Service Routine
 * Wake up from LPM3 when ADC conversion completes
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            // Clear interrupt flag
            ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT_FLAG);
            __bic_SR_register_on_exit(LPM3_bits);                // Exit LPM3
            break;
        default:
            break;
    }
}

void safeShutdownModeInit()
{
    // 停止所有計時器
    Timer_A_stop(TIMER_A0_BASE);
    Timer_A_stop(TIMER_A1_BASE);

    // 停止 ADC 和溫度傳感器
    ADC_disableConversions(ADC_BASE, ADC_COMPLETECONVERSION);
    ADC_disable(ADC_BASE);
    PMM_disableInternalReference();
    PMM_disableTempSensor();

    // // 清除相關狀態標誌
    // *tempSensorRunning = 0;
    // *flashWUnit = 0;
    // *S1buttonDebounce = 0;
    // *S2buttonDebounce = 0;

    // 顯示安全斷電提示
    clearLCD();
    displayScrollText("SAFE TO SHUTDOWN");
     __bis_SR_register(LPM4_bits | GIE);  // 進入最低功耗模式
    __no_operation();
}