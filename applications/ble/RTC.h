/**
 * @file RTC.h
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
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief   Initialize time Service.
 */
void HAL_TimeInit(void);

#ifdef __cplusplus
}
#endif

#endif
