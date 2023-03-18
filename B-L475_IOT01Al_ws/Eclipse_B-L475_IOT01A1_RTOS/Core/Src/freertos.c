/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myHeaderFile.h"
#include "cwpack.h"
#include "cwpack_config.h"
#include "cwpack_internals.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENSOR_PERIOD 1000 //1000
#define WIFI_DELAY 500//500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

// handles for each task
osThreadId defaultTaskHandle;
osThreadId temperatureTaskHandle;
osThreadId humidityTaskHandle;
osThreadId pressureTaskHandle;
osThreadId magnetoTaskHandle;
osThreadId gyroTaskHandle;
osThreadId acceleroTaskHandle;
osThreadId sendMsgTaskHandle;

// TaskProfiler counts times task called (for debugging)
typedef uint32_t TaskProfiler;
TaskProfiler defaultTask_Profiler;
TaskProfiler temperatureTask_Profiler;
TaskProfiler humidityTask_Profiler;
TaskProfiler pressureTask_Profiler;
TaskProfiler magnetoTask_Profiler;
TaskProfiler gyroTask_Profiler;
TaskProfiler acceleroTask_Profiler;
TaskProfiler sendMsgTask_Profiler;
//external vars so I have access to them in other files
osMessageQId msg_queue_Id;
osPoolId memPool_Id;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartDefaultTask(void const *argument);
void StartTemperatureTask(void const *argument);
void StartHumidityTask(void const *argument);
void StartPressureTask(void const *argument);
void StartMagnetoTask(void const *argument);
void StartGyroTask(void const *argument);
void StartAcceleroTask(void const *argument);
void StartSendMsgTask(void const *argument);

void RTOSGetTemperature(void const *argument);
void RTOSGetHumidity(void const *argument);
void RTOSGetPressure(void const *argument);
void RTOSGetMagneto(void const *argument);
void RTOSGetGyro(void const *argument);
void RTOSGetAccelero(void const *argument);
void RTOSSendMsg(void const *argument);

// helper functions
void packInt16T(const char *label, uint16_t val);
void packFloat(const char *label, float val);
void pack316T(char *label, uint16_t val1, uint16_t val2, uint16_t val3);
void pack3float(char *label, float val1, float val2, float val3);
/* USER CODE END FunctionPrototypes */

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */
	/* Create the mutex(es) */
	/* definition and creation of myMutex01 */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */

	/* definition and creation of defaultTask */


	/* Create mempool for use with msg queue */
	osPoolDef(MemPool, 15, MEM_BLOCK_t);
	//osPoolId(memPool_Id);
	memPool_Id = osPoolCreate(osPool(MemPool));
	if(memPool_Id != NULL){
		// Pool created
	}

	/* definition and creation of usart1_msgQHandle */
	// create msg queue of 10 'strings' (uint8_t pointers)
	osMessageQDef(msg_queue, 8, MEM_BLOCK_t*);
	//osMessageQId(usart1_msgQHandle);
	msg_queue_Id = osMessageCreate(osMessageQ(msg_queue), NULL);


//	osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
//	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	osThreadDef(temperatureTask, StartTemperatureTask, osPriorityIdle, 0, 128);
	temperatureTaskHandle = osThreadCreate(osThread(temperatureTask), NULL);

	osThreadDef(humidityTask, StartHumidityTask, osPriorityIdle, 0, 128);
	humidityTaskHandle = osThreadCreate(osThread(humidityTask), NULL);

	osThreadDef(pressureTask, StartPressureTask, osPriorityIdle, 0, 128);
	pressureTaskHandle = osThreadCreate(osThread(pressureTask), NULL);

	osThreadDef(magnetoTask, StartMagnetoTask, osPriorityIdle, 0, 128);
	magnetoTaskHandle = osThreadCreate(osThread(magnetoTask), NULL);

	osThreadDef(gyroTask, StartGyroTask, osPriorityIdle, 0, 128);
	gyroTaskHandle = osThreadCreate(osThread(gyroTask), NULL);

	osThreadDef(acceleroTask, StartAcceleroTask, osPriorityIdle, 0, 128);
	acceleroTaskHandle = osThreadCreate(osThread(acceleroTask), NULL);

	osThreadDef(sendMsgTask, StartSendMsgTask, osPriorityIdle, 0, 128);
	sendMsgTaskHandle = osThreadCreate(osThread(sendMsgTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

// NOTE (wmh) : the task functions defined below and prototyped at the top could be defined anywhere but probably are here to keep the example compact
// we are eventually going to try to connect them to our button and LED code, but for now we'll just leave thtem alone.

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument) {
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;) {
		defaultTask_Profiler++;
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02_button thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTemperatureTask(void const *argument) {
	/* USER CODE BEGIN StartTask02 */
	//BSP_TSENSOR_Init();
	uint32_t prevWakeTime;
	prevWakeTime = osKernelSysTick();
	/* Infinite loop */
	for (;;) {
		temperatureTask_Profiler++;
		RTOSGetTemperature(argument);
		osDelayUntil(&prevWakeTime, SENSOR_PERIOD);
	}
	/* USER CODE END StartTask02 */
} // StartTemperatureTask

/*
/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03_LED thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartHumidityTask(void const *argument) {
	/* USER CODE BEGIN StartTask03 */
	//BSP_HSENSOR_Init();
	uint32_t prevWakeTime;
	prevWakeTime = osKernelSysTick();
	/* Infinite loop */
	for (;;) {
		humidityTask_Profiler++;
		RTOSGetHumidity(argument);
		osDelayUntil(&prevWakeTime, SENSOR_PERIOD);
	}
	/* USER CODE END StartTask03 */
} // StartHumidityTask

/**
 * @brief Function implementing the myTask03_LED thread.
 * @param argument: Not used
 * @retval None
 */
void StartPressureTask(void const *argument) {
	/* USER CODE BEGIN StartTask03 */
	//BSP_PSENSOR_Init();
	uint32_t prevWakeTime;
	prevWakeTime = osKernelSysTick();
	/* Infinite loop */
	for (;;) {
		pressureTask_Profiler++;
		RTOSGetPressure(argument);
		osDelayUntil(&prevWakeTime, SENSOR_PERIOD);
	}
	/* USER CODE END StartTask03 */
} // StartPressureTask

/**
 * @brief Function implementing the myTask03_LED thread.
 * @param argument: Not used
 * @retval None
 */
void StartMagnetoTask(void const *argument) {
	/* USER CODE BEGIN StartTask03 */
	//BSP_MAGNETO_Init();
	uint32_t prevWakeTime;
	prevWakeTime = osKernelSysTick();
	/* Infinite loop */
	for (;;) {
		magnetoTask_Profiler++;
		RTOSGetMagneto(argument);
		osDelayUntil(&prevWakeTime, SENSOR_PERIOD);
	}
	/* USER CODE END StartTask03 */
} // StartMagnetoTask

/**
 * @brief Function implementing the myTask03_LED thread.
 * @param argument: Not used
 * @retval None
 */
void StartGyroTask(void const *argument) {
	/* USER CODE BEGIN StartTask03 */
	//BSP_GYRO_Init();
	uint32_t prevWakeTime;
	prevWakeTime = osKernelSysTick();
	/* Infinite loop */
	for (;;) {
		gyroTask_Profiler++;
		RTOSGetGyro(argument);
		osDelayUntil(&prevWakeTime, SENSOR_PERIOD);
	}
	/* USER CODE END StartTask03 */
} // StartGyroTask

/**
 * @brief Function implementing the myTask03_LED thread.
 * @param argument: Not used
 * @retval None
 */
void StartAcceleroTask(void const *argument) {
	/* USER CODE BEGIN StartTask03 */
	//BSP_ACCELERO_Init();
	/* Infinite loop */
	uint32_t prevWakeTime;
	prevWakeTime = osKernelSysTick();
	for (;;) {
		acceleroTask_Profiler++;
		RTOSGetAccelero(argument);
		osDelayUntil(&prevWakeTime, SENSOR_PERIOD);
	}
	/* USER CODE END StartTask03 */
} // StartAcceleroTask

/**
 * @brief Function implementing the myTask03_LED thread.
 * @param argument: Not used
 * @retval None
 */
void StartSendMsgTask(void const *argument) {
	/* USER CODE BEGIN StartTask03 */
	//start_connection_to_server();
	connect_wifi();
	/* Infinite loop */
	for (;;) {
		sendMsgTask_Profiler++;
		RTOSSendMsg(argument);
		osDelay(WIFI_DELAY);
	}
	/* USER CODE END StartTask03 */
} // StartAcceleroTask


/*
 * works
 */
void RTOSGetTemperature(void const *argument) {
	  float temp_value = 0;
	  temp_value = BSP_TSENSOR_ReadTemp();
	  packFloat("temp", temp_value);

} // RTOSGetTemperature

/*
 * works
 */
void RTOSGetHumidity(void const *argument) {
	float humidity_value = 0;
	//BSP_HSENSOR_Init();
	humidity_value = BSP_HSENSOR_ReadHumidity();

	packFloat("humid", humidity_value);
	//printf("HUMIDITY is = %.2f %%\n", humidity_value);
} // RTOSGetHumidity

/*
 * Not getting init not working (test again tho)
 */
void RTOSGetPressure(void const *argument) {
	float press_value = 0;
	press_value = BSP_PSENSOR_ReadPressure();
	packFloat("press", press_value);
	 //printf("PRESSURE is = %.2f mBar \n", press_value);
} // RTOSGetPressure

/*
 * works
 */
void RTOSGetMagneto(void const *argument) {
	int16_t pDataXYZ[3] = {0};
	BSP_MAGNETO_GetXYZ(pDataXYZ);
	pack316T("mag", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
   // printf("MAGNETO_X = %d \n", pDataXYZ[0]);
    //printf("MAGNETO_Y = %d \n", pDataXYZ[1]);
    //printf("MAGNETO_Z = %d \n", pDataXYZ[2]);
} // RTOSGetMagneto

/*
 * works
 */
void RTOSGetGyro(void const *argument) {
	float pGyroDataXYZ[3] = {0};
	BSP_GYRO_GetXYZ(pGyroDataXYZ);
	pack3float("gyro", pGyroDataXYZ[0], pGyroDataXYZ[1], pGyroDataXYZ[2]);
   // printf("GYRO_X = %.2f \n", pGyroDataXYZ[0]);
    //printf("GYRO_Y = %.2f \n", pGyroDataXYZ[1]);
    //printf("GYRO_Z = %.2f \n", pGyroDataXYZ[2]);
} // RTOSGetGyro

/*
 * Not working!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
void RTOSGetAccelero(void const *argument) {
	int16_t pDataXYZ[3] = {0};
	BSP_ACCELERO_AccGetXYZ(pDataXYZ);
	pack316T("accel", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
    //printf("ACCELERO_X = %d \n", pDataXYZ[0]);
    //printf("ACCELERO_Y = %d \n", pDataXYZ[1]);
    //printf("ACCELERO_Z = %d \n", pDataXYZ[2]);
} // RTOSGetAccelero

/*
 * Will only run when data is available in msg_queue
 * otherwise task will be blocked and other tasks can use cpu
 */
void RTOSSendMsg(void const *argument) {
	osEvent evt = osMessageGet(msg_queue_Id, osWaitForever);

	if(evt.status == osEventMessage){
		// transmit the message
		MEM_BLOCK_t *rptr;

		rptr = evt.value.p;

		if(start_connection_to_server() == 0){
			taskENTER_CRITICAL();
			send_data_to_server(rptr->buffer, rptr->len);
			end_connection_to_server();
			taskEXIT_CRITICAL();
		}

		osPoolFree(memPool_Id, rptr);
	}
} // RTOSSendMsg
/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void packInt16T(const char *label, uint16_t val) {
	// allocate memory for message
	MEM_BLOCK_t *mptr;
	mptr = (MEM_BLOCK_t*) osPoolAlloc(memPool_Id);
	// pack the msg into the buffer
	cw_pack_context pc;
	cw_pack_context_init(&pc, mptr->buffer, PACK_BUFFER_SIZE, 0);
	cw_pack_map_size(&pc, 1);
	cw_pack_str (&pc, label, strlen(label));
	cw_pack_unsigned(&pc, val);

	//if (pc.return_code != CWP_RC_OK)  //printf("pack uint16_t len error");
	//int length = pc.current - pc.start;
	//if (length != 20) printf("pack uint16_t len error");

	uint16_t len =  pc.current - pc.start;
	mptr->len = len;
	// put buffer in queue so can be sent
	osMessagePut(msg_queue_Id, (uint32_t)mptr, osWaitForever);
} // packInt16T

void packFloat(const char *label, float val) {
	// allocate memory for message
	MEM_BLOCK_t *mptr;
	mptr = (MEM_BLOCK_t*) osPoolAlloc(memPool_Id);
	// pack the msg into the buffer
	cw_pack_context pc;
	cw_pack_context_init(&pc, mptr->buffer, PACK_BUFFER_SIZE, 0);
	cw_pack_map_size(&pc, 2);

	cw_pack_str(&pc, "lbl", 3);
	cw_pack_str(&pc, label, strlen(label));

	cw_pack_str(&pc, "val", 3);
	cw_pack_float(&pc, val);

	if (pc.return_code == CWP_RC_OK){
		uint16_t len =  pc.current - pc.start;
		mptr->len = len;
		// put buffer in queue so can be sent
		osMessagePut(msg_queue_Id, (uint32_t)mptr, osWaitForever);
	}
}

void pack316T(char *label, uint16_t val1, uint16_t val2, uint16_t val3) {
	// allocate memory for message
	MEM_BLOCK_t *mptr;
	mptr = (MEM_BLOCK_t*) osPoolAlloc(memPool_Id);
	// pack the msg into the buffer
	cw_pack_context pc;
	cw_pack_context_init(&pc, mptr->buffer, PACK_BUFFER_SIZE, 0);
	cw_pack_map_size(&pc, 4);

	cw_pack_str(&pc, "lbl", 3);
	cw_pack_str(&pc, label, strlen(label));

	cw_pack_str(&pc, "val1", 4);
	cw_pack_unsigned(&pc, (uint64_t)val1);

	cw_pack_str(&pc, "val2", 4);
	cw_pack_unsigned(&pc, (uint64_t)val2);

	cw_pack_str(&pc, "val3", 4);
	cw_pack_unsigned(&pc, (uint64_t)val3);

	if (pc.return_code == CWP_RC_OK){
		uint16_t len =  pc.current - pc.start;
		mptr->len = len;
		// put buffer in queue so can be sent
		osMessagePut(msg_queue_Id, (uint32_t)mptr, osWaitForever);
	}
}

void pack3float(char *label, float val1, float val2, float val3) {
	// allocate memory for message
	MEM_BLOCK_t *mptr;
	mptr = (MEM_BLOCK_t*) osPoolAlloc(memPool_Id);
	// pack the msg into the buffer
	cw_pack_context pc;
	cw_pack_context_init (&pc, mptr->buffer, PACK_BUFFER_SIZE, 0);
	cw_pack_map_size (&pc, 4);

	cw_pack_str(&pc, "lbl", 3);
	cw_pack_str(&pc, label, strlen(label));

	cw_pack_str(&pc, "val1", 4);
	cw_pack_float (&pc, val1);

	cw_pack_str(&pc, "val2", 4);
	cw_pack_float(&pc, val2);

	cw_pack_str(&pc, "val3", 4);
	cw_pack_float(&pc, val3);

	if (pc.return_code == CWP_RC_OK){
		uint16_t len =  pc.current - pc.start;
		mptr->len = len;
		// put buffer in queue so can be sent
		osMessagePut(msg_queue_Id, (uint32_t)mptr, osWaitForever);
	}
}
/* USER CODE END Application */
