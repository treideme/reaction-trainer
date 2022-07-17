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
static int _led_state;
static rt_timer_t _led_timer;


void led_enable(int state) {
  rt_pin_write(_led_en_pin, (state)?PIN_LOW:PIN_HIGH);
}

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

  led_enable(1);
  _led_state = 0;
  _led_timer = NULL;
}

void led_ctrl(int led, int state) {
  if(led < 6)
    rt_pin_write(_led_pins[led], (state)?PIN_LOW:PIN_HIGH);
}

void led_set(int state, int timeout);

void _timeout_event(void* parameter){
  led_set(0, 0);
}

void led_set(int state, int timeout) {
  _led_state = state;
  for(int i = 0; i < 6; i++) {
    led_ctrl(i, state);
  }
  if(state && timeout > 0) {
    if(_led_timer != NULL) {
      rt_timer_stop(_led_timer);
      rt_timer_delete(_led_timer);
      _led_timer = NULL;
    }
    _led_timer = rt_timer_create("led_timer", _timeout_event, NULL, timeout, RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
    rt_timer_start(_led_timer);
  } else {
    if(_led_timer != NULL) {
      rt_timer_stop(_led_timer);
      rt_timer_delete(_led_timer);
      _led_timer = NULL;
    }
  }
}

int led_state(void) {
  return _led_state;
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
