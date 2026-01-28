/*
 * RFID.h
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */

#ifndef RFID_H_
#define RFID_H_

#include "hw_def.h"

typedef struct {
  SPI_HandleTypeDef* SPI_HANDLER;
  GPIO_TypeDef* NSS_PORT;
  uint16_t      NSS_PIN;
}RFID_tbl_t;

typedef enum {
  IO_CONFIGURATION_REG_01 = 0x00,
  IO_CONFIGURATION_REG_02 = 0x01,
  OPERATION_CONTROL_REG   = 0x02,
  MODE_DEFINITION_REG     = 0x03,
  BIT_RATE_DEFINITION_REG = 0x04,



}RFID_REG;


void rfidInit(void);
void rfidMain(void);

HAL_StatusTypeDef rfidSpiTransmit(uint8_t address, uint8_t* pdata, uint8_t len);
HAL_StatusTypeDef rfidSpiReceive(uint8_t address, uint8_t* pdata, uint8_t len);

#endif /* RFID_H_ */
