/*
 * oneshot_timer.h
 *
 *  Created on: Aug 28, 2025
 *      Author: jojae
 */

#ifndef SERVICES_ONESHOT_TIMER_H_
#define SERVICES_ONESHOT_TIMER_H_

#include "hw_def.h"
#define ONESHOT_TIMER_MAX 8

typedef void (*oneshot_cb)(void* user);

typedef struct{
  uint8_t     next;
  uint32_t    delta;
  oneshot_cb  cb;
  void*       user;
  bool        activate;
}oneshot_node_tbl_t;

typedef struct{
  oneshot_cb cb;
  void*      user;
}fired_t;

#define NIL     ((uint8_t)255)

void oneShotInit(void);
void oneShotMain(void);
bool addOneShot(uint32_t delta_ms, oneshot_cb cb, void* user);
bool cancelOneShot(oneshot_cb cb, void* user);

#endif /* SERVICES_ONESHOT_TIMER_H_ */
