/*
 * _uart.c
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */

#include "_uart.h"
#include "qbuffer.h"

extern UART_HandleTypeDef huart1;

uint8_t rx_data[HW_UART_MAX_CH];

// UART 테이블
typedef struct{
  UART_HandleTypeDef* const uart_handler;
  __vo uint32_t            baudrate;
  qbuffer_t                qbuffer;
  volatile bool            is_open;
}uart_tbl_t;

// UART 테이블 초기화
static uart_tbl_t uart_tbl[HW_UART_MAX_CH] = {
    {.uart_handler = &huart1, .baudrate = 115200, .qbuffer = {0},.is_open = false},
//    {.uart_handler = &huart3,115200,{0},false},
};

void      uartInit(void)
{
  return;
//  uart_tbl[0].uart_handler = &hlpuart1;
//  uart_tbl[1].uart_handler = &huart1;
//  uart_tbl[2].uart_handler = &huart2;
}

bool      uartOpen(uint8_t ch, uint32_t baud, uint8_t* p_buffer, uint16_t buffer_size)
{
  bool ret = true;
  if(ch >= HW_UART_MAX_CH)
    return false;

  //QBUFFER 생성
  qbufferCreate(&uart_tbl[ch].qbuffer, p_buffer, buffer_size);
  uart_tbl[ch].is_open = true;

  //BAUD가 다를 경우
  if(baud != uart_tbl[ch].baudrate)
  {
    ret = uartChangeBaud(ch,baud);
  }
  else
  {
    HAL_UART_Receive_IT(uart_tbl[ch].uart_handler,&rx_data[ch],1);
  }
  return ret;
}

uint32_t uartFlush(uint8_t ch)
{
  uint32_t ret = uartAvailable(ch);
  qbufferFlush(&uart_tbl[ch].qbuffer);
  return ret;
}

uint32_t  uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;
  ret = qbufferAvailable(&uart_tbl[ch].qbuffer);
  return ret;
}
uint8_t   uartRead(uint8_t ch)
{
  uint8_t ret;
  qbufferRead(&uart_tbl[ch].qbuffer, &ret, 1);
  return ret;
}
uint32_t  uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;
  status = HAL_UART_Transmit(uart_tbl[ch].uart_handler, p_data, length, 100);
  if(status == HAL_OK)
  {
    ret = length;
  }
  return ret;
}
uint32_t  uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;

  va_start(args,fmt);
  len = vsnprintf(buf,256,fmt,args);

  uartWrite(ch,(uint8_t *)buf, len);
  va_end(args);

  return len;
}

uint32_t  uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;
  ret = uart_tbl[ch].baudrate;
  return ret;
}

bool uartChangeBaud(uint8_t ch, uint32_t baud)
{
  bool ret = true;
  uart_tbl[ch].baudrate = baud;
  uart_tbl[ch].uart_handler->Init.BaudRate = baud;
  // 변경 적용
  if (HAL_UART_DeInit(uart_tbl[ch].uart_handler) != HAL_OK) {
    Error_Handler();
    ret = false;
    return ret;
  }
  if (HAL_UART_Init(uart_tbl[ch].uart_handler) != HAL_OK) {
    Error_Handler();
    ret = false;
    return ret;
  }
  HAL_UART_Receive_IT(uart_tbl[ch].uart_handler,&rx_data[ch],1);
  return ret;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  for(int i = 0; i< HW_UART_MAX_CH; i++)
  {
    if(huart->Instance == uart_tbl[i].uart_handler->Instance)
    {
      qbufferWrite(&uart_tbl[i].qbuffer,&rx_data[i],1);
      HAL_UART_Receive_IT(uart_tbl[i].uart_handler, (uint8_t*)&rx_data[i], 1);
    }
  }
}

