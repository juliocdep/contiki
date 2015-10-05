/*
 * led-board.h
 *
 *  Created on: 24/08/2015
 *      Author: Julio
 */

#ifndef BOARDS_NUCLEO_SHIELD_LED_BOARD_H_
#define BOARDS_NUCLEO_SHIELD_LED_BOARD_H_

typedef enum
{
	LED_RX = 0,
	LED_TX = 1,
	LED_DBG1 = 2,
	LED_DBG2 = 3,
} Led_t;

void LedInit();

void LedDeInit();

void LedOn(Led_t led);

void LedOff(Led_t led);

void LedToggle(Led_t led);

#endif /* BOARDS_NUCLEO_SHIELD_LED_BOARD_H_ */
