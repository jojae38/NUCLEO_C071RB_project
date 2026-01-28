/*
 * qbuffer.h
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */

#ifndef SERVICES_QBUFFER_H_
#define SERVICES_QBUFFER_H_

#include "def.h"

//일반 8비트 전용

typedef struct{
  __vo uint32_t in;
  __vo uint32_t out;
  uint32_t len;
  uint8_t* p_buffer;
}qbuffer_t;

void qbufferInit(void);
bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t len);
bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t len);
bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t len);
uint32_t qbufferAvailable(qbuffer_t *p_node);
void qbufferFlush(qbuffer_t *p_node);

//16비트 전용

typedef struct{
  __vo uint32_t in;
  __vo uint32_t out;
  uint32_t len;
  uint16_t* p_buffer;
}qbuffer16_t;

bool qbuffer16Create(qbuffer16_t *p_node, uint16_t *p_buf, uint32_t len);
bool qbuffer16Write(qbuffer16_t *p_node, uint16_t *p_data, uint32_t len);
bool qbuffer16Read(qbuffer16_t *p_node, uint16_t *p_data, uint32_t len);
uint32_t qbuffer16Available(qbuffer16_t *p_node);
void qbuffer16Flush(qbuffer16_t *p_node);

#endif /* SERVICES_QBUFFER_H_ */
