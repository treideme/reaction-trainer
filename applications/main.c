/**
 * @file main.c
 * @brief Main control loop of application.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bat.h"
#include "leds.h"
#include "acc.h"
#include "ble_handler.h"
#include <rtthread.h>

int main(void) {
  init_leds();
  bat_init();
  rt_kprintf("Reaction Trainer\r\n");
  rt_thread_t ble_thread = rt_thread_create("ble_loop",
                                            ble_loop,
                                            NULL,
                                            512,
                                            (RT_MAIN_THREAD_PRIORITY + 10),
                                            1);
  if (ble_thread != RT_NULL)
    rt_thread_startup(ble_thread);
  while (1) {
    rt_thread_mdelay(100); // Give OS time to to work
  }
}
