/**
 * @file bat.c
 * @brief Li-Ion Battery management.
 * @author Thomas Reidemeister <treideme@gmail.com>
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ch32v20x_adc.h"
#include "drivers/adc.h"
#include "drivers/pin.h"

#define CH_VBAT          9
#define CH_TEMP         16
#define CH_VREF         17
#define REFER_VOLTAGE 1200
#define V25           1400
#define AVG_SLOPE     4000
#define DIVIDER_UP     510
#define DIVIDER_DOWN  1000

static rt_adc_device_t _adc_dev;
static rt_base_t _stat_pin;

void bat_init() {
  /* Enable ADC for VREF, TEMP and VBAT */
  _adc_dev = rt_device_find("adc1");
  ADC_TempSensorVrefintCmd(ENABLE); // Enable temperature sensor and Vrefint channel
  rt_err_t res = rt_adc_enable(_adc_dev, CH_VBAT);
  if(res != RT_EOK) {
    printf("Error enabling ADC channel %d: %d\r\n", CH_VBAT, res);
  }
  res = rt_adc_enable(_adc_dev, CH_TEMP);
  if(res != RT_EOK) {
    printf("Error enabling ADC channel %d: %d\r\n", CH_TEMP, res);
  }
  res = rt_adc_enable(_adc_dev, CH_VREF);
  if(res != RT_EOK) {
    printf("Error enabling ADC channel %d: %d\r\n", CH_VREF, res);
  }
  // Drain ADC capacitor
  for(int i = 0; i < 10; i++) {
    rt_adc_read(_adc_dev, CH_VBAT);
  }

  /* Enable STAT pin */
  _stat_pin = rt_pin_get("PD.6");
  rt_pin_mode(_stat_pin, PIN_MODE_INPUT_PULLUP);
}

rt_uint32_t bat_mv() {
//  rt_uint32_t vref_raw = rt_adc_read(_adc_dev, CH_VREF);
  rt_uint32_t vbat_raw = rt_adc_read(_adc_dev, CH_VBAT);
  vbat_raw *= REFER_VOLTAGE;
  vbat_raw /= (1<<12 - 1);

  vbat_raw *= (DIVIDER_UP + DIVIDER_DOWN) / DIVIDER_UP;

  return vbat_raw;
}

rt_uint32_t bat_temp() {
  // Temperature (℃) = ((VSENSE-V25)/Avg_Slope)+25
  int vsense = (int)rt_adc_read(_adc_dev, CH_TEMP);
  vsense *= REFER_VOLTAGE;
  vsense /= (1<<12 - 1);

  rt_uint32_t temp = (vsense - V25) / AVG_SLOPE + 25;
  return temp;
}

int bat_charge() {
  rt_uint8_t state = rt_pin_read(_stat_pin);
  return ~state;
}

/***********************************************************************************************************************
 * Console routines for debugging
 */

void vbat() {
  rt_uint32_t vref = bat_mv();
  rt_kprintf("VBAT: %d mV\r\n", vref);
}

void charge() {
  int charge = bat_charge();
  rt_kprintf("battery charging: %s\r\n", (charge)?("on"):("off"));
}

void temp() {
  rt_uint32_t temp = bat_temp();
  rt_kprintf("Temperature: %d C\r\n", temp);
}

MSH_CMD_EXPORT(vbat, Output the battery voltage in mV);
MSH_CMD_EXPORT(charge, Output the charge state of the battery);
MSH_CMD_EXPORT(temp, Output the temperature in Celsius);
