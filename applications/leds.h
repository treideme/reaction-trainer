/**
 * @file leds.h
 * @brief Higher-level routines to control the LEDs on board.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _leds_h_
#define _leds_h_

/**
 * @brief Initialize the 74HC573 driving the LEDs.
 */
void init_leds(void);

/**
 * @brief Enable or disable the 74HC573 in transparent mode to drive the LEDs.
 * @param state 0 to disable, 1 to enable
 */
void led_enable(int state);

/**
 * @brief Set the state of all LEDs.
 * @param state 0 to turn off, 1 to turn on
 * @param timeout Time in milliseconds to turn off LEDs (0 to leave on).
 */
void led_set(int state, int timeout);

/**
 * @brief Get the state of all LEDs (note only set by led_set).
 * @return 0 if all LEDs are off, 1 if any LED is on
 */
int led_state(void);

/**
 * @brief Set the state of a single LED.
 * @param led The LED number (0-5)
 * @param state 0 to turn off, 1 to turn on
 */
void led_ctrl(int led, int state);

#endif // _leds_h_