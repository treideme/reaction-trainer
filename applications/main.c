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

int main(void)
{
  PRINT("%s\n", VER_LIB);
  WCHBLE_Init();
  HAL_Init();
  GAPRole_PeripheralInit();
  Peripheral_Init();
  app_uart_init();
    rt_kprintf("MCU-CH32V208WBU6\r\n");
    while(1)
    {
      TMOS_SystemProcess();
      app_uart_process();
      rt_thread_yield();
    }
}
