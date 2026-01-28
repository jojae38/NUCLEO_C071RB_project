/*
 * hw.c
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */


#include "hw.h"

void hwInit(void)
{
	cliInit();
	rfidInit();
}

void hwMain(void)
{
	cliMain();
	rfidMain();
}
