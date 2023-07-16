/**
 * @file RTC.c
 * @brief HAL RTC functions for timing the TMOS stack for BLE.
 * @author Thomas Reidemeister <treideme@gmail.com>, WCH
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @brief This file is a modified version of the original WCH file.
 */

/******************************************************************************/
/* Header file contains */
#include "HAL.h"
#include "rtthread.h"

/*******************************************************************************
 * @fn      HAL_Time0Init
 *
 * @brief   System timer initialization
 *
 * @param   None.
 *
 * @return  None.
 */
void HAL_TimeInit(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
#if( CLK_OSC32K )
    RCC_LSICmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_OFF);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#else
    RCC_LSEConfig(RCC_LSE_ON);
    /* Check the specified RCC logo position settings or not, 
     * wait for the low-speed crystal oscillator to be ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) 
    {
        temp++;
        Delay_Ms(10);
    }
    if(temp>=250)
    {
        printf("time error..\n");
    }
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#endif
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask();
    RTC_WaitForLastTask();
    RTC_SetPrescaler(1);
    RTC_WaitForLastTask();
    RTC_SetCounter(0);
    RTC_WaitForLastTask();
#if( CLK_OSC32K )
    Lib_Calibration_LSI();
#endif
    TMOS_TimerInit( RTC_GetCounter );
}

__attribute__((interrupt()))
void RTCAlarm_IRQHandler(void)
{
  GET_INT_SP();
  /* enter interrupt */
  rt_interrupt_enter();

    TMOS_TimerIRQHandler();
    RTC_ClearITPendingBit(RTC_IT_ALR);
    RTC_WaitForLastTask();

  rt_interrupt_leave();
  FREE_INT_SP();
}

/******************************** endfile @ time ******************************/
