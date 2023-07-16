/**
 * @file HAL.h
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
#ifndef __HAL_H
#define __HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ble_config.h"
#include "RTC.h"

/* hal task Event */
#define HAL_REG_INIT_EVENT    0x2000

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern tmosTaskID halTaskID;

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/**
 * @brief   Hardware initialization
 */
extern void HAL_Init(void);

/**
 * @brief   HAL processing
 *
 * @param   task_id - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 */
extern tmosEvents HAL_ProcessEvent(tmosTaskID task_id, tmosEvents events);

/**
 * @brief   Initialization of the BLE library
 */
extern void WCHBLE_Init(void);

/**
 * @brief   Internal 32K calibration
 */
extern void Lib_Calibration_LSI(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
