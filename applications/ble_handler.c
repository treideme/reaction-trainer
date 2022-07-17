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
#include "ble_vcp.h"
#include "HAL.h"
#include "rtthread.h"
#include "bat.h"
#include "leds.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) u32 MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
uint8_t const MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

const char* ble_handle(char* data, size_t *len)
{
  if(*len > 0) {
    int target_led_state = -1;
    int timeout = -1;
    uint16_t vbat = bat_mv();
    uint8_t led = led_state();

    // Battery status
    if(rt_strncmp(data, "status", 6) == 0) {
      sprintf(data, "V %i %i", vbat, led);
      *len = rt_strlen(data);
      rt_kprintf("Status Request\n");
    } else if(sscanf(data, "L %i %i", &target_led_state, &timeout) == 2) {
      led_set(target_led_state, timeout);
      sprintf(data, "V %i %i", vbat, led);
      *len = rt_strlen(data);
      rt_kprintf("Led Request\n");
    } else {
      rt_kprintf("Unhandled BLE CMD: %s\n", data);
      *len = 0;
    }
  }
  return data;
}

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
