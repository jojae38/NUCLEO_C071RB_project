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
  bool          isinit;
  uint16_t      FIFO_LEN;
}RFID_tbl_t;

typedef enum {
  IO_CONFIGURATION_REG_01 = 0x00,
  IO_CONFIGURATION_REG_02 = 0x01,
  OPERATION_CONTROL_REG   = 0x02,
  MODE_DEFINITION_REG     = 0x03,
  BIT_RATE_DEFINITION_REG = 0x04,


  MAIN_INTERRUPT_REG      = 0x1A,

  FIFO_STATUS_REG_01      = 0x1E,
  FIFO_STATUS_REG_02      = 0x1F,
  AUXILIARY_DISPLAY_REG   = 0x31,
  IC_IDENTITY             = 0x3F,
}RFID_REG;

typedef enum{
  SET_DEFAULT =                 0xC0,
  STOP_ALL_ACTIVITIES =         0xC2,
  TRANSMIT_WITH_CRC =           0xC4,
  TRANSMIT_WITHOUT_CRC =        0xC5,
  TRANSMIT_REQA =               0xC6,
  TRANSMIT_WUPA =               0xC7,
  NFC_INITIAL_FIELD_ON =        0xC8,
  NFC_RESPONSE_FIELD_ON =       0xC9,
  GO_TO_SENSE =                 0xCD,
  GO_TO_SLEEP =                 0xCE,
  MASK_RECEIVE_DATA =           0xD0,
  UNMASK_RECEIVE_DATA =         0xD1,
  CHANGE_AM_MODULATION_STATE =  0xD2,
  MEASURE_AMPLITUDE =           0xD3,
  RESET_RX_GAIN =               0xD5,
  ADJUST_REGULATORS =           0xD6,
  CALIBRATE_DRIVER_TIMING =     0xD8,
  MEASURE_PHASE =               0xD9,
  CLEAR_RSSI =                  0xDA,
  CLEAR_FIFO =                  0xDB,
  ENTER_TRANSPARENT_MODE =      0xDC,
  MEASURE_POWER_SUPPLY =        0xDF,
  START_GENERAL_PURPOSE_TIMER = 0xE0,
  START_WAKEUP_TIMER =          0xE1,
  START_MASK_RECEIVE_TIMER =    0xE2,
  START_NO_RESPONSE_TIMER =     0xE3,
  START_PP_TIMER =              0xE4,
  STOP_NO_RESPONSE_TIMER =      0xE8,
  TRIGGER_RC_CALIBRATION =      0xEA,
  REGISTER_SPACE_B_ACCESS =     0xFB,
  TEST_ACCESS =                 0xFC,
}RFID_DM;

//메인 인터럽트 플래그
#define INT_FLAG_OSC  (1 << 7) //오실레이터 작동 시작
#define INT_FLAG_WL   (1 << 6) //FIFO에 300이상 차 있을 경우
#define INT_FLAG_RXS  (1 << 5) //받기 시작
#define INT_FLAG_RXE  (1 << 4) //받기 끝
#define INT_FLAG_TXE  (1 << 3) //통신 완료
#define INT_FLAG_COL  (1 << 2) //충돌?
#define INT_FLAG_RST  (1 << 1)

#define ST25R3916B_IC_IDENTITY          0x31

#define FIFO_LOAD                       0x00
#define FIFO_READ                       0x1F

#define IO_CONFIGURATION_REG_01_DATA    0x0C
#define IO_CONFIGURATION_REG_02_DATA    0x84
#define OPERATION_CONTROL_REG_DATA      0xC0
#define MODE_DEFINITION_REG_DATA        0x09
#define BIT_RATE_DEFINITION_REG_DATA    0x00

#define AUXILIARY_DISPLAY_REG_EXPECT    0x10

void rfidInit(void);
void rfidMain(void);

HAL_StatusTypeDef rfidSpiTransmit(uint8_t address, uint8_t* pdata, uint8_t len);
HAL_StatusTypeDef rfidSpiReceive(uint8_t address, uint8_t* pdata, uint8_t len);
HAL_StatusTypeDef rfidSpiDrTransmit(uint8_t address, uint8_t len);
HAL_StatusTypeDef rfidSpiFifoReceive(uint8_t len);
uint16_t rfidFifoAvailable(void);

#endif /* RFID_H_ */
