/*
 * bsp.c
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */
#include "bsp.h"

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}

void Reset(void)
{
  NVIC_SystemReset();
}

bool oneshot_start(oneshot_t* t, uint32_t delay_ms)
{
  if(t->started) return false;
  t->deadline = millis() + delay_ms;
  t->started = true;
  return true;
}

void oneshot_stop(oneshot_t* t)
{
  t->deadline = 0;
  t->started = false;
}

bool oneshot_expired(oneshot_t *t)
{
    if (t->started && (int32_t)(millis() - t->deadline) >= 0) {
        t->started = false;
        return true;
    }
    return false;
}

bool isTimeOver(uint32_t term, uint32_t last_check)
{
  return (millis() - last_check) >= term;
}

//uint8_t stringInBuffer(uint8_t* buffer, const char* tar_str, uint8_t buffer_len)
//{
//  uint8_t ret = buffer_len;
//  uint8_t tar_str_len = strlen(tar_str);
//  for(int i = 0; i < buffer_len - tar_str_len; i++)
//  {
//    if(memcmp(&buffer[i], tar_str, tar_str_len) == 0)
//    {
//      ret = i;
//      break;
//    }
//  }
//  return ret;
//}

int getIntFromBuffer(char* p_start, char* p_end)
{
  char num_buffer[16] = {0};
  int len = p_end - p_start;
  memcpy(num_buffer,p_start,len);
  num_buffer[len] = '\0';
  return atoi(num_buffer);
}

void hexstr_to_bytes(const char *str, uint8_t *out, int *out_len)
{
    int len = strlen(str);
    *out_len = len / 2;
    for (int i = 0; i < *out_len; i++)
    {
        char byte_str[3] = {str[2*i], str[2*i+1], 0};
        out[i] = (unsigned char)strtol(byte_str, NULL, 16);
    }
}

void bytes_to_hexstr(const uint8_t *bytes, char *out, int len)
{
  for(int i = 0; i< len; i++)
  {
//    out[i] = bytes[2*i+1]*16 + byptes[2*i];

    //문제 있음 TODO
    sprintf(out + (i * 2), "%02X", bytes[i]);
  }
  out[len*2] = '\0';
}

bool stringInBuffer(uint8_t* buf, uint8_t buf_len, const char * str)
{
  size_t str_len = strlen(str);
  if(str_len == 0||str_len > buf_len)
  {
    return false;
  }

  for(size_t i = 0; i<= buf_len - str_len; i++)
  {
    if(memcmp(&buf[i],str,str_len) == 0)
    {
      return true;
    }
  }
  return false;
}
