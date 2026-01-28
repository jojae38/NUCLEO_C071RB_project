/*
 * hw_def.h
 *
 *  Created on: Jul 31, 2025
 *      Author: jojae
 */

#ifndef BSP_HW_DEF_H_
#define BSP_HW_DEF_H_

#include "def.h"
#include "main.h"
#include "bsp.h"

#define MAIN_CLOCK           48000000
#define DEBUGOK
#define HW_VERSION           100

//UART
#define _USE_HW_UART
#define HW_UART_MAX_CH 1

//CLI
#define _USE_HW_CLI
#define HW_CLI_MAX_CH  1

//RFID
#define _USE_HW_RFID
#define HW_RFID_MAX_CH 1
#define MAX_CARD_COUNT 20



#define HW_CLI_CMD_NAME_MAX    16
#define HW_CLI_CMD_LIST_MAX    16
#define HW_CLI_LINE_HIS_MAX    4
#define HW_CLI_LINE_BUF_MAX    50

#define SW_DEBOUNCE 500
#define SIGNAL_DEBOUNCE 200

#endif /* BSP_HW_DEF_H_ */
