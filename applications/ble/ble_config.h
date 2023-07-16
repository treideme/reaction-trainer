/**
 * @file config.h
 * @brief BLE Specific Configurations.
 * @author Thomas Reidemeister <treideme@gmail.com>, WCH
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @brief This file is a modified version of the original WCH file.
 */
#ifndef __BLE_CONFIG_H
#define __BLE_CONFIG_H

#ifdef WCHBLE_ROM
#include "WCHBLE_ROM.H"
#else
#include "wchble.h"
#endif

#include "ch32v20x.h"

/*********************************************************************
 BLE_MAC                                    - whether to customize the Bluetooth Mac address
                                              (default: FALSE - use the chip Mac address),
                                              you need to modify the Mac address definition in main.c

 HAL_SLEEP                                  - whether to enable the sleep function (default: FALSE )
 WAKE_UP_MAX_TIME_US                        - early wake-up time, that is, the time required for the system clock to
                                              stabilize
                                              Pause Mode - 45
                                              Idle Mode - 5
 
 TEM_SAMPLE                                 - Whether to enable the function of calibrating according to temperature
                                              changes, a single calibration takes less than 10ms (default: TRUE )
 
 BLE_CALIBRATION_ENABLE                     - whether to enable the timing calibration function, a single
                                              calibration takes less than 10ms (default: TRUE )
 BLE_CALIBRATION_PERIOD                     - the period of timing calibration, in ms (default: 120000 )
 
 BLE_SNV                                    - Whether to enable the SNV function to store binding information (default: TRUE )
 BLE_SNV_ADDR                               - SNV information save address, use data flash last (default: 0x77E00 )
 BLE_SNV_NUM                                - SNV information storage sector number equal to number of bindings that
                                              can be stored (default: 3)
                                            - If the SNVNum parameter is configured, you need to modify the flash size
                                              erased in the Lib_Write_Flash function accordingly, and the size is
                                              SNVBlock*SNVNum

 CLK_OSC32K                                 - RTC clock selection, if the master role is included, the external 32K must
                                              be used ( 0 external (32768Hz), default: 1: internal (32000Hz),
                                              2: internal (32768Hz) )

 BLE_MEMHEAP_SIZE                           - RAM size used by the Bluetooth protocol stack, not less than 6K
                                              (default: (1024*6) )


 BLE_BUFF_MAX_LEN                           - the maximum packet length of a single connection
                                              (default: 27 (ATT_MTU=23), value range [27~251] )
 BLE_BUFF_NUM                               - number of packets buffered by the controller (default: 5)
 BLE_TX_NUM_EVENT                           - How many packets can be sent for a single connection event (default: 1 )
 BLE_TX_POWER                               - transmit power (default: LL_TX_POWEER_0_DBM (0dBm) )
 
 PERIPHERAL_MAX_CONNECTION                  - how many slave roles can be done at the same time (default: 1)
 CENTRAL_MAX_CONNECTION                     - how many host roles can be done at the same time (default: 3)

 **********************************************************************/

/*********************************************************************
 * Ĭ������ֵ
 */
#ifndef BLE_MAC
#define BLE_MAC                             FALSE
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP                           FALSE
#endif
#ifndef WAKE_UP_MAX_TIME_US
#define WAKE_UP_MAX_TIME_US                 2400
#endif
#ifndef HAL_KEY
#define HAL_KEY                             FALSE
#endif
#ifndef HAL_LED
#define HAL_LED                             FALSE
#endif
#ifndef TEM_SAMPLE
#define TEM_SAMPLE                          FALSE
#endif
#ifndef BLE_CALIBRATION_ENABLE
#define BLE_CALIBRATION_ENABLE              TRUE
#endif
#ifndef BLE_CALIBRATION_PERIOD
#define BLE_CALIBRATION_PERIOD              120000
#endif
#ifndef BLE_SNV
#define BLE_SNV                             FALSE
#endif
#ifndef BLE_SNV_ADDR
#define BLE_SNV_ADDR                        0x08077C00
#endif
#ifndef BLE_SNV_NUM
#define BLE_SNV_NUM                         3
#endif
#ifndef CLK_OSC32K
#define CLK_OSC32K                          1
#endif
#ifndef BLE_MEMHEAP_SIZE
#define BLE_MEMHEAP_SIZE                    (1024*7)
#endif
#ifndef BLE_BUFF_MAX_LEN
#define BLE_BUFF_MAX_LEN                    27
#endif
#ifndef BLE_BUFF_NUM
#define BLE_BUFF_NUM                        5
#endif
#ifndef BLE_TX_NUM_EVENT
#define BLE_TX_NUM_EVENT                    1
#endif
#ifndef BLE_TX_POWER
#define BLE_TX_POWER                        LL_TX_POWEER_0_DBM
#endif
#ifndef PERIPHERAL_MAX_CONNECTION
#define PERIPHERAL_MAX_CONNECTION           1
#endif
#ifndef CENTRAL_MAX_CONNECTION
#define CENTRAL_MAX_CONNECTION              3
#endif

extern uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
extern const uint8_t MacAddr[6];

#endif

