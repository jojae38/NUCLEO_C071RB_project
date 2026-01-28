/*
 * oneshot_timer.c
 *
 *  Created on: Aug 28, 2025
 *      Author: jojae
 */



#include "oneshot_timer.h"

uint8_t head;
uint8_t freehead;
uint8_t used_count;

static oneshot_node_tbl_t node_tbl[ONESHOT_TIMER_MAX];

static volatile uint8_t fr,fw;
uint8_t fired_drop;
static fired_t fired_q[ONESHOT_TIMER_MAX];
static uint32_t last_time = 0;

static void oneShotExecute(void);
static void insertOneShot(uint8_t index, uint32_t delta_ms);

static inline uint32_t ticks_since(uint32_t now, uint32_t last){
  return (uint32_t)(now - last);    // wrap-safe (unsigned)
}

static void freeNode(uint8_t index)
{
  node_tbl[index].next = freehead;
  node_tbl[index].delta = 0;
  node_tbl[index].activate = false;
  freehead = index;
}

void oneShotInit(void)
{
  for(int i = 0; i<ONESHOT_TIMER_MAX; i++)
  {
    node_tbl[i].next = (i == ONESHOT_TIMER_MAX - 1)?NIL:i + 1;
    node_tbl[i].activate = false;
    node_tbl[i].cb = NULL;
    node_tbl[i].user = NULL;
    node_tbl[i].delta = 0;
  }
  head = NIL;
  freehead = 0;
  used_count = 0;

  //실행 큐
  fr = 0;
  fw = 0;
  fired_drop = 0;
}

void oneShotMain(void)
{
  uint32_t now = millis();
  uint32_t dt  = ticks_since(now, last_time);
  last_time    = now;

  if(head == NIL) return;

  if (node_tbl[head].delta > dt) node_tbl[head].delta -= dt;
  else node_tbl[head].delta = 0;

  while(head!= NIL && node_tbl[head].delta == 0)
  {
    uint8_t cur = head;
    uint8_t next_fw = ((fw + 1)%ONESHOT_TIMER_MAX);
    if(next_fw != fr)
    {
      fired_q[fw].cb = node_tbl[cur].cb;
      fired_q[fw].user = node_tbl[cur].user;
      fw = next_fw;
    }
    else
    {
      fired_drop ++;
    }

    head = node_tbl[cur].next;
    freeNode(cur);
    if(used_count)used_count--;
  }

  oneShotExecute();
}

bool addOneShot(uint32_t delta_ms, oneshot_cb cb, void* user)
{
  if(!cb) return false;
  if(freehead == NIL) return false;

  uint8_t n = freehead;
  freehead = node_tbl[freehead].next;

  node_tbl[n].activate = true;
  node_tbl[n].cb = cb;
  node_tbl[n].user = user;
  insertOneShot(n,delta_ms);

  used_count ++;
  return true;
}

bool cancelOneShot(oneshot_cb cb, void* user)
{
  uint8_t prev = NIL;
  uint8_t cur = head;
  while(cur!=NIL)
  {
    if(node_tbl[cur].cb == cb && node_tbl[cur].user == user)
    {
      uint8_t nxt = node_tbl[cur].next;
      if(nxt != NIL) node_tbl[nxt].delta += node_tbl[cur].delta;
      if(prev == NIL) head = nxt;
      else node_tbl[prev].next = nxt;
      freeNode(cur);
      if(used_count) used_count --;
      return true;
    }
    prev = cur;
    cur = node_tbl[cur].next;
  }

  return false;
}

static void insertOneShot(uint8_t index, uint32_t delta_ms)
{
  node_tbl[index].delta = delta_ms;
  node_tbl[index].next = NIL;

  //비어있는 경우
  if(head == NIL){head = index; return;}

  uint32_t dms = delta_ms;
  uint8_t cur = head;
  uint8_t prev = NIL;

  //비어있지 않은 경우
  while(cur != NIL && node_tbl[cur].delta <= dms)
  {
    dms -= node_tbl[cur].delta;
    prev = cur;
    cur = node_tbl[cur].next;
  }

  node_tbl[index].delta = dms;

  if(cur!=NIL)
  {
    node_tbl[cur].delta -= dms;
  }
  node_tbl[index].next = cur;
  if(prev == NIL)
  {
    //처음
    head = index;
  }
  else
  {
    //마지막
    node_tbl[prev].next = index;
  }
}

static void oneShotExecute(void)
{
  while(fr!=fw)
  {
    oneshot_cb cb = fired_q[fr].cb;
    void*      us = fired_q[fr].user;
    fr = (uint8_t)((fr+1) % ONESHOT_TIMER_MAX);
    if(cb) cb(us);
  }
}
