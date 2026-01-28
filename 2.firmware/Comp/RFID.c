/*
 * RFID.c
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */

#include "RFID.h"

extern SPI_HandleTypeDef hspi1;

RFID_tbl_t RFID_tbl = {
    .SPI_HANDLER = &hspi1,
    .NSS_PORT = SPI1_NSS_GPIO_Port,
    .NSS_PIN = SPI1_NSS_Pin
};

static inline uint8_t st25_cmd_wr(uint8_t addr) { return (uint8_t)(0x00 | (addr & 0x3F)); }
static inline uint8_t st25_cmd_rd(uint8_t addr) { return (uint8_t)(0x40 | (addr & 0x3F)); }

static inline void BSS_L(void){ HAL_GPIO_WritePin(RFID_tbl.NSS_PORT, RFID_tbl.NSS_PIN, GPIO_PIN_RESET); }
static inline void BSS_H(void){ HAL_GPIO_WritePin(RFID_tbl.NSS_PORT, RFID_tbl.NSS_PIN, GPIO_PIN_SET); }


HAL_StatusTypeDef rfidSpiTransmit(uint8_t address, uint8_t* pdata, uint8_t len)
{
  if(len == 0) return HAL_ERROR;
  uint8_t send_buffer[64];
  memset(send_buffer, 0, 64);

  BSS_L();
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t wr_addr = st25_cmd_wr(address);

  send_buffer[0] = wr_addr;
  memcpy(&send_buffer[1],pdata,len);
//  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, &wr_addr, 1, 100);
//  if(status == HAL_OK) status = HAL_SPI_Transmit(&hspi1, pdata, len, 500);

  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, send_buffer, len + 1, 500);
  BSS_H();

  return status;
}

HAL_StatusTypeDef rfidSpiReceive(uint8_t address, uint8_t* pdata, uint8_t len)
{
  if(len == 0) return HAL_ERROR;
  BSS_L();
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t rd_addr = st25_cmd_wr(address);
  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, &rd_addr, 1, 100);
  if(status == HAL_OK) status = HAL_SPI_Receive(&hspi1, pdata, len, 500);
  BSS_H();
  return status;
}

void rfidInit(void)
{

}

void rfidMain(void)
{

}



void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		BSP_LED_On(LED_BLUE);
	}
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		BSP_LED_On(LED_BLUE);
	}
}
