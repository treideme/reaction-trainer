/**
 * @file MCU.c
 * @brief TMOS Hardware Abstraction Layer header file for initializations needed for BLE (mostly RTC).
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
#include "string.h"
#include "rtthread.h"

tmosTaskID halTaskID;

/*******************************************************************************
 * @fn      Lib_Calibration_LSI
 *
 * @brief   Internal 32K calibration
 *
 * @param   None.
 *
 * @return  None.
 */
void Lib_Calibration_LSI(void)
{
    Calibration_LSI(Level_64);
}

#if(defined(BLE_SNV)) && (BLE_SNV == TRUE)
/*******************************************************************************
 * @fn      Lib_Read_Flash
 *
 * @brief   Callback function used for BLE lib.
 *
 * @param   addr.
 * @param   num.
 * @param   pBuf.
 *
 * @return  None.
 */
uint32_t Lib_Read_Flash(uint32_t addr, uint32_t num, uint32_t *pBuf)
{
    tmos_memcpy(pBuf, (uint32_t*)addr, num*4);
    return 0;
}

/*******************************************************************************
 * @fn      Lib_Write_Flash
 *
 * @brief   Callback function used for BLE lib.
 *
 * @param   addr.
 * @param   num.
 * @param   pBuf.
 *
 * @return  None.
 */
uint32_t Lib_Write_Flash(uint32_t addr, uint32_t num, uint32_t *pBuf)
{
    FLASH_Unlock_Fast();
    FLASH_ErasePage_Fast( addr );
    FLASH_ProgramPage_Fast( addr, pBuf);
    FLASH_Lock_Fast();
    Delay_Us(1);
    return 0;
}
#endif

/*******************************************************************************
 * @fn      WCHBLE_Init
 *
 * @brief   BLE library initialization
 *
 * @param   None.
 *
 * @return  None.
 */
void WCHBLE_Init(void)
{
    uint8_t     i;
    bleConfig_t cfg;
    if(!tmos_memcmp(VER_LIB, VER_FILE, strlen(VER_FILE)))
    {
        rt_kprintf("head file error...\n");
        while(1);
    }

    // 32M crystal capacitance and current
    OSC->HSE_CAL_CTRL &= ~(0x07<<28);
    OSC->HSE_CAL_CTRL |= 0x03<<28;
    OSC->HSE_CAL_CTRL |= 3<<24;

    tmos_memset(&cfg, 0, sizeof(bleConfig_t));
    cfg.MEMAddr = (uint32_t)MEM_BUF;
    cfg.MEMLen = (uint32_t)BLE_MEMHEAP_SIZE;
    cfg.BufMaxLen = (uint32_t)BLE_BUFF_MAX_LEN;
    cfg.BufNumber = (uint32_t)BLE_BUFF_NUM;
    cfg.TxNumEvent = (uint32_t)BLE_TX_NUM_EVENT;
    cfg.TxPower = (uint32_t)BLE_TX_POWER;
#if(defined(BLE_SNV)) && (BLE_SNV == TRUE)
    cfg.SNVAddr = (uint32_t)BLE_SNV_ADDR;
    cfg.SNVNum = (uint32_t)BLE_SNV_NUM;
    cfg.readFlashCB = Lib_Read_Flash;
    cfg.writeFlashCB = Lib_Write_Flash;
#endif
    cfg.ClockFrequency = CAB_LSIFQ/2;
#if(CLK_OSC32K==0)
    cfg.ClockAccuracy = 50;
#else
    cfg.ClockAccuracy = 1000;
#endif
    cfg.ConnectNumber = (PERIPHERAL_MAX_CONNECTION & 3) | (CENTRAL_MAX_CONNECTION << 2);
#if(defined TEM_SAMPLE) && (TEM_SAMPLE == TRUE)
    // Calibrate RF and internal RC according to temperature changes (greater than 7 degrees Celsius)
    cfg.tsCB = HAL_GetInterTempValue;
  #if(CLK_OSC32K)
    cfg.rcCB = Lib_Calibration_LSI; // Internal 32K clock calibration
  #endif
#endif
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    cfg.idleCB = BLE_LowPower; // Enable sleep
#endif
#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
    for(i = 0; i < 6; i++)
    {
        cfg.MacAddr[i] = MacAddr[5 - i];
    }
#else
    {
        uint8_t MacAddr[6];
        FLASH_GetMACAddress(MacAddr);
        for(i = 0; i < 6; i++)
        {
            cfg.MacAddr[i] = MacAddr[i]; // Use chip mac address
        }
    }
#endif
    if(!cfg.MEMAddr || cfg.MEMLen < 4 * 1024)
    {
        while(1);
    }
    i = BLE_LibInit(&cfg);
    if(i)
    {
        rt_kprintf("LIB init error code: %x ...\n", i);
        while(1);
    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE );
    NVIC_EnableIRQ( BB_IRQn );
    NVIC_EnableIRQ( LLE_IRQn );
}

/*******************************************************************************
 * @fn      HAL_ProcessEvent
 *
 * @brief   HAL processing
 *
 * @param   task_id - The TMOS assigned task ID.
 * @param   events  - events to process.  This is a bit map and can
 *                      contain more than one event.
 *
 * @return  events.
 */
tmosEvents HAL_ProcessEvent(tmosTaskID task_id, tmosEvents events)
{
    uint8_t *msgPtr;

    if(events & SYS_EVENT_MSG)
    { 
        /**
         * Process the HAL layer message, call tmos_msg_receive to read the message, 
         * and delete the message after processing.
         */
        msgPtr = tmos_msg_receive(task_id);
        if(msgPtr)
        {
            /* De-allocate */
            tmos_msg_deallocate(msgPtr);
        }
        return events ^ SYS_EVENT_MSG;
    }
    if(events & HAL_REG_INIT_EVENT)
    {
#if(defined BLE_CALIBRATION_ENABLE) && (BLE_CALIBRATION_ENABLE == TRUE) // Calibration tasks, a single time is less than 10ms
        BLE_RegInit();                                                  // Calibrate RF
#if(CLK_OSC32K)
        Lib_Calibration_LSI(); // Calibrate internal RC
#endif
        tmos_start_task(halTaskID, HAL_REG_INIT_EVENT, MS1_TO_SYSTEM_TIME(BLE_CALIBRATION_PERIOD));
        return events ^ HAL_REG_INIT_EVENT;
#endif
    }
    return 0;
}

/*******************************************************************************
 * @fn      HAL_Init
 *
 * @brief   Ó²¼þ³õÊ¼»¯
 *
 * @param   None.
 *
 * @return  None.
 */
void HAL_Init()
{
    halTaskID = TMOS_ProcessEventRegister(HAL_ProcessEvent);
    HAL_TimeInit();
#if(defined BLE_CALIBRATION_ENABLE) && (BLE_CALIBRATION_ENABLE == TRUE)
    // Add a calibration task, and a single calibration takes less than 10ms
    tmos_start_task(halTaskID, HAL_REG_INIT_EVENT, MS1_TO_SYSTEM_TIME(BLE_CALIBRATION_PERIOD)); 
#endif
}

/******************************** endfile @ mcu ******************************/
