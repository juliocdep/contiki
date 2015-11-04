/*
 * button-board.h
 *
 *  Created on: 25/08/2015
 *      Author: Julio
 */

#ifndef BOARDS_NUCLEO_SHIELD_BUTTON_BOARD_H_
#define BOARDS_NUCLEO_SHIELD_BUTTON_BOARD_H_

typedef enum
{
    BTN_LEFT = 0,
    BTN_RIGHT,
} ButtonId_t;

typedef enum
{
	RELEASED = 0,
	PRESSED,
} ButtonState_t;

typedef struct
{
	Gpio_t ButtonGpio;
	ButtonState_t ButtonState;
} Button_t;

void ButtonInit();

ButtonState_t ButtonState(ButtonId_t button);

#endif /* BOARDS_NUCLEO_SHIELD_BUTTON_BOARD_H_ */
