/**
 * @file bat.h
 * @brief Li-Ion Battery management.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _bat_h_
#define _bat_h_

#include "drivers/adc.h"

void bat_init();
rt_uint32_t bat_mv();
int bat_charge();

#endif // _bat_h_
