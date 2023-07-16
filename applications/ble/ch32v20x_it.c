/**
 * @file ch32v20x_it.c
 * @brief BLE Interrupt routines.
 * @author Thomas Reidemeister <treideme@gmail.com>, WCH
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @brief This file is a modified version of the original WCH file.
 */

/*********************************************************************
 * INCLUDES
 */
#include "ch32v20x_it.h"
#include "ble_config.h"

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void BB_IRQHandler(void) __attribute__((interrupt()));
void LLE_IRQHandler(void) __attribute__((interrupt()));

/*********************************************************************
 * @fn      BB_IRQHandler
 *
 * @brief   BB Interrupt for BLE.
 *
 * @return  None
 */
void BB_IRQHandler(void)
{
  GET_INT_SP();
  /* enter interrupt */
  rt_interrupt_enter();
    BB_IRQLibHandler();
  rt_interrupt_leave();
  FREE_INT_SP();
}

/*********************************************************************
 * @fn      LLE_IRQHandler
 *
 * @brief   LLE Interrupt for BLE.
 *
 * @return  None
 */
void LLE_IRQHandler(void)
{
  GET_INT_SP();
  /* enter interrupt */
  rt_interrupt_enter();
    LLE_IRQLibHandler();
  rt_interrupt_leave();
  FREE_INT_SP();
}
