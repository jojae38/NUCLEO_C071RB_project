/*
 * RFID.c
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */

#include "RFID.h"
#include "cli.h"

#define DEFAULT_RFID_TIMEOUT 500

extern SPI_HandleTypeDef hspi1;

RFID_tbl_t RFID_tbl = {
    .SPI_HANDLER = &hspi1,
    .NSS_PORT = SPI1_NSS_GPIO_Port,
    .NSS_PIN = SPI1_NSS_Pin,
    .isinit = false,
    .FIFO_LEN = 0,
    .Int_status = 0,
    .Int_mask = 0,
};

uint8_t st25_cmd_fifo(uint8_t addr) { return (uint8_t)(0x80 | (addr & 0x3F)); }
uint8_t st25_cmd_dr(uint8_t addr) { return (uint8_t)(0xC0 | (addr & 0x3F)); }
uint8_t st25_cmd_wr(uint8_t addr) { return (uint8_t)(0x00 | (addr & 0x3F)); }
uint8_t st25_cmd_rd(uint8_t addr) { return (uint8_t)(0x40 | (addr & 0x3F)); }

void BSS_L(void){ HAL_GPIO_WritePin(RFID_tbl.NSS_PORT, RFID_tbl.NSS_PIN, GPIO_PIN_RESET); }
void BSS_H(void){ HAL_GPIO_WritePin(RFID_tbl.NSS_PORT, RFID_tbl.NSS_PIN, GPIO_PIN_SET); }


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
  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, send_buffer, len + 1, DEFAULT_RFID_TIMEOUT);
  BSS_H();

  return status;
}

HAL_StatusTypeDef rfidSpiReceive(uint8_t address, uint8_t* pdata, uint8_t len)
{
  if(len == 0) return HAL_ERROR;
  BSS_L();
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t rd_addr = st25_cmd_rd(address);
  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, &rd_addr, 1, 100);
  if(status == HAL_OK) status = HAL_SPI_Receive(RFID_tbl.SPI_HANDLER, pdata, len, 500);

//  status = HAL_SPI_TransmitReceive(RFID_tbl.SPI_HANDLER, &rd_addr, pdata, len, DEFAULT_RFID_TIMEOUT);
  BSS_H();
  return status;
}

HAL_StatusTypeDef rfidSpiDrTransmit(uint8_t address, uint8_t len)
{
  if(len == 0) return HAL_ERROR;

  BSS_L();
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t dr_addr = st25_cmd_dr(address);

  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, &dr_addr, len, DEFAULT_RFID_TIMEOUT);
  BSS_H();

  return status;
}

HAL_StatusTypeDef rfidSpiFifoReceive(uint8_t* pdata, uint8_t len)
{
  if(len == 0) return HAL_ERROR;

  BSS_L();
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t fifo_addr = st25_cmd_fifo(FIFO_READ);
  status = HAL_SPI_Transmit(RFID_tbl.SPI_HANDLER, &fifo_addr, 1, 100);
  if(status == HAL_OK) status = HAL_SPI_Receive(RFID_tbl.SPI_HANDLER, pdata, len, DEFAULT_RFID_TIMEOUT);
  BSS_H();

  return status;
}

bool rfidCheckId(void)
{
  uint8_t ID;
  bool ret = false;
  if(rfidSpiReceive(IC_IDENTITY, &ID, 1) == HAL_OK)
  {
    if(ID == ST25R3916B_IC_IDENTITY)
      ret = true;
  }
  return ret;
}

bool rfidCheckReg(uint8_t reg, uint8_t mask, uint8_t val)
{
  uint8_t regVal = 0;
  rfidSpiReceive(reg, &regVal, 1);
  return ((regVal & mask) == val);
}

void rfidDisableInterrupt(uint8_t reg)
{
}

uint8_t rfidWaitInterruptsTimed(uint8_t reg, uint32_t timeout)
{

}

uint8_t rfidGetInterrupt(uint8_t reg)
{
  uint8_t irqs;
  if(irqs != 0x00)
  {
    RFID_tbl.Int_status &= ~irqs;
  }
  return irqs;
}

bool rfidOscOn(void)
{

}

void rfidInit(void)
{
  uint8_t buffer[20];
  uint8_t data = 0;
  memset(buffer,0,20);
  RFID_tbl.isinit = true;

//Set Default
  if(rfidSpiDrTransmit(SET_DEFAULT, 1) != HAL_OK) goto error;

//ID 확인 - 통신 확인
  if(rfidCheckId() == false)
    goto error;

//인터럽트 초기화
  RFID_tbl.Int_mask = 0;
  RFID_tbl.Int_status = 0;

//전압 레벨 변경
  data = IO_CONFIGURATION_REG_01_DATA;
  if(rfidSpiTransmit(IO_CONFIGURATION_REG_01, &data, 1) != HAL_OK) goto error;

//SPI 속도 설정
  data = IO_CONFIGURATION_REG_02_DATA;
  if(rfidSpiTransmit(IO_CONFIGURATION_REG_02, &data, 1) != HAL_OK) goto error;

//오실레이터 확인


  data = OPERATION_CONTROL_REG_DATA;
  if(rfidSpiTransmit(OPERATION_CONTROL_REG, &data, 1) != HAL_OK) goto error;

  delay(10);
  //오실레이터 확인
  if(rfidSpiReceive(AUXILIARY_DISPLAY_REG,&data,1)!= HAL_OK) goto error;;
  if(data != AUXILIARY_DISPLAY_REG_EXPECT) goto error;

  data = MODE_DEFINITION_REG_DATA;
  if(rfidSpiTransmit(MODE_DEFINITION_REG, &data, 1) != HAL_OK) goto error;

  data = BIT_RATE_DEFINITION_REG_DATA;
  if(rfidSpiTransmit(BIT_RATE_DEFINITION_REG, &data, 1) != HAL_OK) goto error;
  if(rfidSpiDrTransmit(STOP_ALL_ACTIVITIES, 1) != HAL_OK) goto error;
  if(rfidSpiDrTransmit(RESET_RX_GAIN, 1) != HAL_OK) goto error;
  if(rfidSpiDrTransmit(NFC_INITIAL_FIELD_ON, 1) != HAL_OK) goto error;


//  delay(10);
//  if(rfidSpiDrTransmit(TRANSMIT_REQA, 1) != HAL_OK) goto error;


  cliPrintf("RFID INIT SUCCESS\r\n");
  return;

error:
  RFID_tbl.isinit = false;
  cliPrintf("RFID INIT FAILED\r\n");
  return;
}

void rfidReset(void)
{

}

void rfidSendRequest(void)
{
  if(rfidSpiDrTransmit(TRANSMIT_REQA, 1) != HAL_OK)
  {
    cliPrintf("RFID ERROR\r\n");
    rfidReset();
  }
  cliPrintf("REQUEST SEND\r\n");
}

void rfidClearFifo(void)
{
  if(rfidSpiDrTransmit(CLEAR_FIFO, 1) != HAL_OK)
  {
    cliPrintf("RFID ERROR\r\n");
    rfidReset();
  }
  cliPrintf("CLEAR FIFO\r\n");
}

uint16_t rfidReadFifo(uint8_t * buffer)
{
  uint16_t fifo_len = rfidFifoAvailable();
  if(fifo_len == 0)
    return 0;
  rfidSpiFifoReceive(buffer, fifo_len);
  return fifo_len;
}

uint16_t rfidFifoAvailable(void)
{
  uint16_t ret = 0;
  uint8_t buffer[2];
  if(rfidSpiReceive(FIFO_STATUS_REG_02, &buffer[1],1) != HAL_OK)
  {
    cliPrintf("RFID ERROR\r\n");
    rfidReset();
  }
  if(rfidSpiReceive(FIFO_STATUS_REG_01, &buffer[0],1) != HAL_OK)
  {
    cliPrintf("RFID ERROR\r\n");
    rfidReset();
  }
  ret = ret | ((buffer[1] & 0xC0) << 2);
  ret = ret | (uint16_t)buffer[0];

  RFID_tbl.FIFO_LEN = ret;
  return ret;
}

bool init_timer = false;
uint8_t timer = 0;
void rfidMain(void)
{

//  if(BSP_LED_GetState(LED_BLUE) == true)
//  {
//    if(init_timer == false)
//    {
//      init_timer = true;
//      timer = millis() + 500;
//    }
//  }
//
//  if(init_timer && timer - millis() < 0)
//  {
//    BSP_LED_Off(LED_BLUE);
//  }
}



//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == GPIO_PIN_0)
//	{
//		BSP_LED_On(LED_BLUE);
//	}
//}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		BSP_LED_On(LED_BLUE);
	}
}
