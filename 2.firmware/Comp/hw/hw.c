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

static __vo uint32_t timer_100ms = 0;
static __vo uint32_t last_100ms = 0;
static __vo uint32_t last_200ms = 0;
static __vo uint32_t last_500ms = 0;
static __vo uint32_t last_1s = 0;
static __vo uint32_t last_5s = 0;

void hwMain(void)
{
  timer_100ms = millis()/100;

  //0.1초
  if(timer_100ms != last_100ms)
  {
    last_100ms = timer_100ms;

//    btnMain();

    // 0.2초
    if(timer_100ms - last_200ms >= 2)
    {
      last_200ms = timer_100ms;
    }

    //0.5초
    if(timer_100ms - last_500ms >= 5)
    {
      last_500ms = timer_100ms;

    }

    //1초
    if(timer_100ms - last_1s >= 10)
    {
      last_1s = timer_100ms;
      rfidMain();
    }

    //5초
    if(timer_100ms - last_5s >= 50)
    {
      last_5s = timer_100ms;
    }

  }

	cliMain();
//	rfidMain();
}
