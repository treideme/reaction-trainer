/**
 * @file main.c
 * @brief Main control loop of application.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ch32v20x.h"
#include "wchble.h"
#include "config.h"
#include "HAL.h"
#include "peripheral.h"
#include "bat.h"
#include "leds.h"
#include "acc.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"
#include <board.h>

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) u32 MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
uint8_t const MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/**
 * TMOS event handler. This is soo ugly, please WCH open-source that BLE stack so we can fix this mess.
 * WTF do I have to poll another OS in a tight-loop to just make BLE work. This could be much better
 * handled in a purely-event driven fashion and hence better integrate with RTThread.
 * @param paramenter
 * FIXME: turn this into a timer-based event handler
 */
void ble_loop(void *paramenter) {
  /* waiting for an event to occur */
  TMOS_SystemProcess();
  app_uart_process();
  /* Serve and process events */
  rt_thread_yield();
}

int main(void) {
  WCHBLE_Init();
  HAL_Init();
  GAPRole_PeripheralInit();
  Peripheral_Init();
  app_uart_init();
  init_leds();
  bat_init();
  rt_kprintf("MCU-CH32V208WBU6 New\r\n");
  rt_thread_t ble_thread = rt_thread_create("ble_loop",
                                            ble_loop,
                                            NULL,
                                            256,
                                            (RT_MAIN_THREAD_PRIORITY + 10),
                                            1);
  if (ble_thread != RT_NULL)
    rt_thread_startup(ble_thread);
  while (1) {
    rt_thread_mdelay(100); // Give OS time to to work
  }
}
