/*
 * button-board.c
 *
 *  Created on: 26/08/2015
 *      Author: Julio
 */

#include "board.h"
#include "button-board.h"

#define BTN_1		PC_0
#define BTN_2		PC_1

Button_t Buttons[2];

void ButtonInit() {
	GpioInit(&(Buttons[0].ButtonGpio), BTN_1, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, RESET);
	GpioInit(&(Buttons[1].ButtonGpio), BTN_2, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, RESET);

	Buttons[0].ButtonState = RELEASED;
	Buttons[1].ButtonState = RELEASED;
}

ButtonState_t ButtonState(ButtonId_t button) {
	if (GpioRead(&(Buttons[button].ButtonGpio)) == RESET && Buttons[button].ButtonState == RELEASED) {
		Buttons[button].ButtonState = PRESSED;
	}

	if (GpioRead(&(Buttons[button].ButtonGpio)) == SET && Buttons[button].ButtonState != RELEASED) {
		Buttons[button].ButtonState = RELEASED;
	}

	return Buttons[button].ButtonState;
}
