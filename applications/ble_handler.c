/**
 * @file ble_handler.h
 * @brief High-level wrapper for WCH BLE stack.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ble_handler.h"
#include "wchble.h"
#include "ble_config.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) u32 MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
uint8_t const MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/**
 * TMOS event handler for WCH BLE stack
 */
void ble_loop(void* paramenter)
{
  (void)paramenter;
  ble_vcp_init();
  WCHBLE_Init();
  HAL_Init();
  GAPRole_PeripheralInit();
  Peripheral_Init();

  while (1)
  {
    /* waiting for an event to occur */
    TMOS_SystemProcess();
    ble_vcp_process();
    /* Serve and process events */
    rt_thread_yield();
  }
}
