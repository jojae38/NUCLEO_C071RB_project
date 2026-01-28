/*
 * btn.c
 *
 *  Created on: 2026. 1. 28.
 *      Author: jojae
 */


#include "btn.h"


void BSP_PB_Callback(Button_TypeDef Button)
{
	if(Button == BUTTON_USER)
	{
		//TODO 함수 넣을것!
		BSP_LED_Toggle(LED_BLUE);
	}
}
