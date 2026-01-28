/*
 * bsp.h
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#include "def.h"
#include "stm32c071xx.h"
#include "stm32c0xx_hal.h"
#include "stm32c0xx.h"
#include "stm32c0xx_it.h"

void vectorInit(void);
void delay(uint32_t ms);
uint32_t millis(void);
void Reset(void);
bool isTimeOver(uint32_t term, uint32_t last_check);

typedef struct{
  uint32_t deadline;
  bool     started;
}oneshot_t;
bool oneshot_start(oneshot_t* t, uint32_t delay_ms);
void oneshot_stop(oneshot_t* t);
bool oneshot_expired(oneshot_t *t);
//uint8_t stringInBuffer(uint8_t* buffer, const char* tar_str, uint8_t buffer_len);
int getIntFromBuffer(char*p_start, char* p_end);
void hexstr_to_bytes(const char *str, uint8_t *out, int *out_len);
void bytes_to_hexstr(const uint8_t *bytes, char *out, int len);
bool stringInBuffer(uint8_t* buf, uint8_t buf_len, const char * str);

/* ---------- write (serialize) ---------- */
static inline void wr_u8(uint8_t *b, size_t *i, uint8_t v) {
  b[(*i)++] = v;
}
static inline void wr_be16(uint8_t *b, size_t *i, uint16_t v) {  // MSB first
  b[(*i)++] = (uint8_t)(v >> 8);
  b[(*i)++] = (uint8_t)(v);
}
static inline void wr_le16(uint8_t *b, size_t *i, uint16_t v) {  // LSB first
  b[(*i)++] = (uint8_t)(v);
  b[(*i)++] = (uint8_t)(v >> 8);
}
static inline void wr_be32(uint8_t *b, size_t *i, uint32_t v) {
  b[(*i)++] = (uint8_t)(v >> 24);
  b[(*i)++] = (uint8_t)(v >> 16);
  b[(*i)++] = (uint8_t)(v >> 8);
  b[(*i)++] = (uint8_t)(v);
}
static inline void wr_le32(uint8_t *b, size_t *i, uint32_t v) {
  b[(*i)++] = (uint8_t)(v);
  b[(*i)++] = (uint8_t)(v >> 8);
  b[(*i)++] = (uint8_t)(v >> 16);
  b[(*i)++] = (uint8_t)(v >> 24);
}
static inline void wr_be64(uint8_t *b, size_t *i, uint64_t v) {
  for (int k = 7; k >= 0; --k) b[(*i)++] = (uint8_t)(v >> (8*k));
}
static inline void wr_le64(uint8_t *b, size_t *i, uint64_t v) {
  for (int k = 0; k < 8;  ++k) b[(*i)++] = (uint8_t)(v >> (8*k));
}
static inline void wr_bytes(uint8_t *b, size_t *i, const void *src, size_t n) {
  const uint8_t *s = (const uint8_t*)src;
  for (size_t j = 0; j < n; ++j) b[(*i)++] = s[j];
}

/* ---------- read (deserialize) ---------- */
static inline uint8_t  rd_u8 (const uint8_t *b, size_t *i) { return b[(*i)++]; }
static inline uint16_t rd_be16(const uint8_t *b, size_t *i) {
  uint16_t v  = (uint16_t)b[(*i)++] << 8;
  v |=          (uint16_t)b[(*i)++];
  return v;
}
static inline uint16_t rd_le16(const uint8_t *b, size_t *i) {
  uint16_t v  = (uint16_t)b[(*i)++];
  v |=        ((uint16_t)b[(*i)++] << 8);
  return v;
}
static inline uint32_t rd_be32(const uint8_t *b, size_t *i) {
  uint32_t v  = (uint32_t)b[(*i)++] << 24;
  v |=         (uint32_t)b[(*i)++] << 16;
  v |=         (uint32_t)b[(*i)++] << 8;
  v |=         (uint32_t)b[(*i)++];
  return v;
}
static inline uint32_t rd_le32(const uint8_t *b, size_t *i) {
  uint32_t v  = (uint32_t)b[(*i)++];
  v |=        ((uint32_t)b[(*i)++] << 8);
  v |=        ((uint32_t)b[(*i)++] << 16);
  v |=        ((uint32_t)b[(*i)++] << 24);
  return v;
}
static inline uint64_t rd_be64(const uint8_t *b, size_t *i) {
  uint64_t v = 0;
  for (int k = 7; k >= 0; --k) v |= (uint64_t)b[(*i)++] << (8*k);
  return v;
}
static inline uint64_t rd_le64(const uint8_t *b, size_t *i) {
  uint64_t v = 0;
  for (int k = 0; k < 8;  ++k) v |= (uint64_t)b[(*i)++] << (8*k);
  return v;
}
static inline void rd_bytes(const uint8_t *b, size_t *i, void *dst, size_t n) {
  uint8_t *d = (uint8_t*)dst;
  for (size_t j = 0; j < n; ++j) d[j] = b[(*i)++];
}

#endif /* BSP_BSP_H_ */
