/*
 * led-board.c
 *
 *  Created on: 25/08/2015
 *      Author: Julio
 */

#include "board.h"
#include "led-board.h"

#define LED_1			PA_0
#define LED_2			PA_1
#define LED_3			PA_4
#define LED_4			PB_0

Gpio_t LedGpios[4];

void LedInit() {
	GpioInit(&(LedGpios[0]), LED_1, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, DISABLE);
	GpioInit(&(LedGpios[1]), LED_2, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, DISABLE);
	GpioInit(&(LedGpios[2]), LED_3, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, DISABLE);
	GpioInit(&(LedGpios[3]), LED_4, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, DISABLE);
}

void LedDeInit() {
	GpioInit(&(LedGpios[0]), LED_1, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 1);
	GpioInit(&(LedGpios[1]), LED_2, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 1);
	GpioInit(&(LedGpios[2]), LED_3, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 1);
	GpioInit(&(LedGpios[3]), LED_4, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 1);
}

void LedOn(Led_t led) {
	GpioWrite(&(LedGpios[led]), ENABLE);
}

void LedOff(Led_t led) {
	GpioWrite(&(LedGpios[led]), DISABLE);
}

void LedToggle(Led_t led) {
	GpioWrite(&(LedGpios[led]), GpioRead(&(LedGpios[led])) ^ 1);
}
