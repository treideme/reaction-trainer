/**
 * @file leds.c
 * @brief Implementation of higher-level routines to control the LEDs on board.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <rtthread.h>
#include "drivers/pin.h"

static rt_base_t _led_pins[6];
static rt_base_t _led_en_pin;
static rt_base_t _led_latch_pin;

void init_leds(void) {
  _led_en_pin = rt_pin_get("PC.9");
  _led_latch_pin = rt_pin_get("PB.12");
  _led_pins[0] = rt_pin_get("PC.8");
  _led_pins[1] = rt_pin_get("PC.7");
  _led_pins[2] = rt_pin_get("PC.6");
  _led_pins[3] = rt_pin_get("PB.15");
  _led_pins[4] = rt_pin_get("PB.14");
  _led_pins[5] = rt_pin_get("PB.13");

  for (int i = 0; i < 6; i++) {
    rt_pin_mode(_led_pins[i],PIN_MODE_OUTPUT);
    rt_pin_write(_led_pins[i], PIN_HIGH);
  }

  // Disable output but setup for transparent latch
  rt_pin_mode(_led_en_pin,PIN_MODE_OUTPUT);
  rt_pin_mode(_led_latch_pin,PIN_MODE_OUTPUT);
  rt_pin_write(_led_latch_pin, PIN_HIGH);
  rt_pin_write(_led_en_pin, PIN_HIGH);
}

void led_enable(int state) {
  rt_pin_write(_led_en_pin, (state)?PIN_LOW:PIN_HIGH);
}

void led_ctrl(int led, int state) {
  if(led < 6)
    rt_pin_write(_led_pins[led], (state)?PIN_LOW:PIN_HIGH);
}

/***********************************************************************************************************************
 * Console routines for debugging
 */

void led_arm(int argc, char**argv) {
  int arm = 1;
  if(argc == 2) {
    arm = atoi(argv[1]);
  }
  led_enable(arm);
}

void led(int argc, char**argv) {
  if(argc == 3) {
    int led = atoi(argv[1]);
    int state = atoi(argv[2]);
    led_ctrl(led, state);
  } else {
    rt_kprintf("Invalid argument\r\n");
  }
}

MSH_CMD_EXPORT(led_arm, Arm the LEDS of the device: 1=arm 0=disarm);
MSH_CMD_EXPORT(led, Set the led state: 1=on 0=off);
