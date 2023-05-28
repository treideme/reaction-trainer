/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     WangShun       first version
 */

#include "ch32v20x.h"
#include "wchble.h"
#include "config.h"
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
 */
void ble_loop(void* paramenter)
{
  while (1)
  {
    /* waiting for an event to occur */
    TMOS_SystemProcess();
    app_uart_process();
    /* Serve and process events */
    rt_thread_yield();
  }
}

int main(void)
{
  WCHBLE_Init();
  HAL_Init();
  GAPRole_PeripheralInit();
  Peripheral_Init();
  app_uart_init();
    rt_kprintf("MCU-CH32V208WBU6\r\n");
  rt_thread_t peripheral = rt_thread_create("ble_loop",
                                            ble_loop,
                                            NULL,
                                            1024,
                                            (RT_MAIN_THREAD_PRIORITY+10),
                                            10);
  if (peripheral != RT_NULL)
    rt_thread_startup(peripheral);
  while(1)
  {
    rt_thread_mdelay(1000);
    rt_kprintf(".");
  }
}
