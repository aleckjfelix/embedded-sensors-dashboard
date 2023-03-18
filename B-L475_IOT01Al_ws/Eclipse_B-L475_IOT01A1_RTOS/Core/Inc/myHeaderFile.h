/*
 * myHeaderFile.h
 *
 *  Created on: May 11, 2022
 *      Author: Aleck
 */

#ifndef INC_MYHEADERFILE_H_
#define INC_MYHEADERFILE_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern osMessageQId usart1_msgQHandle;
extern osPoolId memPool_Id;

#define PACK_BUFFER_SIZE 100
typedef struct {
	uint8_t buffer[PACK_BUFFER_SIZE]; // 20 is max size of packed msg
	uint8_t len;
} MEM_BLOCK_t;

#endif /* INC_MYHEADERFILE_H_ */
