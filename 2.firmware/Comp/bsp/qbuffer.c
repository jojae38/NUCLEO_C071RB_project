/*
 * qbuffer.c
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */


#include "qbuffer.h"

void qbufferInit(void)
{

}

bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t len)
{
  bool ret = true;

  p_node->in = 0;
  p_node->out = 0;
  p_node->p_buffer = p_buf;
  p_node->len = len;

  return ret;
}

bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t len)
{
  bool ret = true;
  uint32_t next_in;
  for(int i = 0; i< len; i++)
  {
    next_in = (p_node->in + 1) % p_node->len;

    if(next_in != p_node->out)
    {
      if(p_node->p_buffer != NULL)
      {
        p_node->p_buffer[p_node->in] = p_data[i];
      }
      p_node->in = next_in;
    }
    else
    {
      ret = false;
      break;
    }
  }
  return ret;
}

bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t len)
{
  bool ret = true;
  for(int i = 0; i< len; i++)
  {
    if(p_node->out != p_node->in)
    {
      if(p_node->p_buffer != NULL)
      {
        p_data[i] = p_node->p_buffer[p_node->out];
      }
      p_node->out = (p_node->out + 1) % p_node->len;
    }
    else
    {
      ret = false;
      break;
    }
  }
  return ret;
}

uint32_t qbufferAvailable(qbuffer_t *p_node)
{
  uint32_t ret;
  if (p_node->in >= p_node->out)
    ret = p_node->in - p_node->out;
  else
    ret = p_node->len + p_node->in - p_node->out;
  return ret;
}

void qbufferFlush(qbuffer_t *p_node)
{
  p_node->out = p_node->in;
//  p_node->in = 0;
//  p_node->out = 0;
}

bool qbuffer16Create(qbuffer16_t *p_node, uint16_t *p_buf, uint32_t len)
{
  bool ret = true;

  p_node->in = 0;
  p_node->out = 0;
  p_node->p_buffer = p_buf;
  p_node->len = len;

  return ret;
}

bool qbuffer16Write(qbuffer16_t *p_node, uint16_t *p_data, uint32_t len)
{
  bool ret = true;
  uint32_t next_in;

  for(int i = 0; i< len; i++)
  {
    next_in = (p_node->in + 1) % p_node->len;

    if(next_in != p_node->out)
    {
      if(p_node->p_buffer != NULL)
      {
        p_node->p_buffer[p_node->in] = p_data[i];
      }
      p_node->in = next_in;
    }
    else
    {
      ret = false;
      break;
    }
  }

  return ret;
}

bool qbuffer16Read(qbuffer16_t *p_node, uint16_t *p_data, uint32_t len)
{
  bool ret = true;
  for(int i = 0; i< len; i++)
  {
    if(p_node->out != p_node->in)
    {
      if(p_node->p_buffer != NULL)
      {
        p_data[i] = p_node->p_buffer[p_node->out];
      }
      p_node->out = (p_node->out + 1) % p_node->len;
    }
    else
    {
      ret = false;
      break;
    }
  }
  return ret;
}

uint32_t qbuffer16Available(qbuffer16_t *p_node)
{
  uint32_t ret;
  if (p_node->in >= p_node->out)
    ret = p_node->in - p_node->out;
  else
    ret = p_node->len + p_node->in - p_node->out;
  return ret;
}

void qbuffer16Flush(qbuffer16_t *p_node)
{
  p_node->in = 0;
  p_node->out = 0;
}

