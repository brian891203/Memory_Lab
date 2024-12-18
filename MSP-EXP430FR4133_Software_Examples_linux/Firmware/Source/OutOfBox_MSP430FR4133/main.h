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
 * main.h
 *
 * Out of Box Demo for the MSP-EXP430FR4133
 * Main loop, initialization, and interrupt service routines
 *
 * Updated for Temperature Sensor and FRAM Usage
 *
 ******************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

#include <driverlib.h>

// Modes for system state
#define STARTUP_MODE 0
#define TEMPSENSOR_MODE 1

// Memory address for FRAM storage
#define FRAM_TEMP_ADDRESS &BAKMEM7

// Temperature threshold in Celsius
#define TEMP_THRESHOLD 30

// External variable declarations
extern volatile unsigned char * tempSensorRunning;
extern volatile unsigned char * mode;
extern volatile unsigned short *degC;  // 添加對 degC 的外部宣告

// Timer configuration parameter
// extern Timer_A_initUpModeParam initUpParam_A0;

// Function prototypes
void Init_GPIO(void);
// void Init_Clock(void);
void Init_RTC(void);
void initTemperatureSensor(void);
unsigned int readTemperature(void);
void storeTemperatureToFRAM(unsigned int temp);
unsigned int retrieveTemperatureFromFRAM(void);
void checkTemperatureThreshold(unsigned int temperature);

#endif /* MAIN_H_ */
