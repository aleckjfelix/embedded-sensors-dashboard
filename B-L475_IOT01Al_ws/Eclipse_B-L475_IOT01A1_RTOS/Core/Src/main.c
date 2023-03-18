/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dfsdm.h"
#include "i2c.h"
#include "quadspi.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Update SSID and PASSWORD with own Access point settings */
#define SSID     "Alec-Hotspot"
#define PASSWORD "12345678"
#define LOCALPORT           5021
#define HOSTPORT			7000

#define TERMINAL_USE

#define WIFI_WRITE_TIMEOUT 10000
#define WIFI_READ_TIMEOUT  10000
#define SOCKET                 0

#ifdef  TERMINAL_USE
#define LOG(a) printf a
#else
#define LOG(a)
#endif
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#if defined (TERMINAL_USE)
extern UART_HandleTypeDef hDiscoUart;
#endif /* TERMINAL_USE */

static uint8_t http[1024]; // string to send
static uint8_t IP_Addr[4]; // wifi module ip
static uint8_t host_ip_addr[4] = { 192, 168, 1, 43 }; // ip of host (python server)
//static int LedState = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
#if defined (TERMINAL_USE)
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
#endif /* TERMINAL_USE */

//static void SystemClock_Config(void);
static int wifi_start(void);
static int wifi_connect(void);
void test_values();
void init_BSP();
//int start_connection_to_server(void);
//int end_connection_to_server(void);
//int send_data_to_server(uint8_t data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void buttonLED();//in buttonLED.c; turn on green LED when blue button is pressed
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* Configure the peripherals common clocks */
	PeriphCommonClock_Config();

	/* USER CODE BEGIN SysInit */
#if defined (TERMINAL_USE)
	/* Initialize all configured peripherals */
	hDiscoUart.Instance = DISCOVERY_COM1;
	hDiscoUart.Init.BaudRate = 115200;
	hDiscoUart.Init.WordLength = UART_WORDLENGTH_8B;
	hDiscoUart.Init.StopBits = UART_STOPBITS_1;
	hDiscoUart.Init.Parity = UART_PARITY_NONE;
	hDiscoUart.Init.Mode = UART_MODE_TX_RX;
	hDiscoUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hDiscoUart.Init.OverSampling = UART_OVERSAMPLING_16;
	hDiscoUart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	hDiscoUart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	BSP_COM_Init(COM1, &hDiscoUart);
	//BSP_TSENSOR_Init();

	printf("****** Wifi Client ****** \n\n");

#endif /* TERMINAL_USE */
	test_values();
	init_BSP();
	//start_app();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	/*
	 MX_GPIO_Init();
	 MX_DFSDM1_Init();
	 MX_I2C2_Init();
	 MX_QUADSPI_Init();
	 MX_SPI3_Init();
	 MX_USART1_UART_Init();
	 MX_USART3_UART_Init();
	 MX_USB_OTG_FS_PCD_Init();
	 MX_ADC1_Init();
	 */
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Call init function for freertos objects (in freertos.c) */
	MX_FREERTOS_Init();
	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

void init_BSP() {
	//BSP_ACCELERO_DeInit();


	BSP_ACCELERO_Init();
	BSP_GYRO_Init();
	BSP_MAGNETO_Init();
	BSP_PSENSOR_Init();
	BSP_HSENSOR_Init();
	BSP_TSENSOR_Init();
}

void test_values() {

	int16_t pDataXYZ[3] = {0};
	float pGyroDataXYZ[3] = {0};

	BSP_ACCELERO_Init();
	BSP_ACCELERO_AccGetXYZ(pDataXYZ);
	printf("ACCELERO_X = %d \n", pDataXYZ[0]);
	printf("ACCELERO_Y = %d \n", pDataXYZ[1]);
	printf("ACCELERO_Z = %d \n", pDataXYZ[2]);
	BSP_ACCELERO_DeInit();

	BSP_GYRO_Init();
	BSP_GYRO_GetXYZ(pGyroDataXYZ);
    printf("GYRO_X = %.2f \n", pGyroDataXYZ[0]);
    printf("GYRO_Y = %.2f \n", pGyroDataXYZ[1]);
    printf("GYRO_Z = %.2f \n", pGyroDataXYZ[2]);
	BSP_GYRO_DeInit();

	BSP_MAGNETO_Init();
	BSP_MAGNETO_GetXYZ(pDataXYZ);
    printf("MAGNETO_X = %d \n", pDataXYZ[0]);
    printf("MAGNETO_Y = %d \n", pDataXYZ[1]);
    printf("MAGNETO_Z = %d \n", pDataXYZ[2]);
    BSP_MAGNETO_DeInit();

	BSP_PSENSOR_Init();
	float press_value = 0;
	press_value = BSP_PSENSOR_ReadPressure();
	printf("PRESSURE is = %.2f mBar \n", press_value);

	BSP_HSENSOR_Init();
	 float humidity_value = 0;
	humidity_value = BSP_HSENSOR_ReadHumidity();
	printf("HUMIDITY is = %.2f %%\n", humidity_value);

	BSP_TSENSOR_Init();
	float temp_value = 0;
	temp_value = BSP_TSENSOR_ReadTemp();
	printf("TEMPERATURE is = %.2f �C\n", temp_value);

}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void) {
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the peripherals clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK
			| RCC_PLLSAI1_ADC1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
/*
 *
 */
void start_app() {
	start_connection_to_server();
} //void start_app()

int connect_wifi() {
	// Attempt to connect to selected access point
	if (wifi_connect() != 0)
		return -1;
	return 0;
}
/*
 * int start_connection_to_server(void)
 */
int start_connection_to_server(void) {

	// Attempt to connect to selected access point
	//if (wifi_connect() != 0)
	//	return -1;

	// blocking connect to server
	while (WIFI_STATUS_OK
			!= WIFI_OpenClientConnection(SOCKET, WIFI_TCP_PROTOCOL, "",
					host_ip_addr, HOSTPORT, LOCALPORT)) {
		LOG(
				("Waiting connection to  %d.%d.%d.%d\n",host_ip_addr[0],host_ip_addr[1],host_ip_addr[2],host_ip_addr[3]));
	}

	LOG(
			("Connected %d.%d.%d.%d:%d\n",host_ip_addr[0],host_ip_addr[1],host_ip_addr[2],host_ip_addr[3],host_ip_addr));

	// test send
	//send_data_to_server();

	//end_connection_to_server();
	return 0;
} // init_connection_to_server(void)

/*
 * int end_connection_to_server(void)
 */
int end_connection_to_server(void) {
	WIFI_CloseClientConnection(SOCKET);
} // int end_connection_to_server(void)

/*
 * int send_data_to_server()
 */
int send_data_to_server(uint8_t *data, uint16_t len) {
	uint16_t SentDataLength;
	WIFI_Status_t ret;

	//strcpy((char*) http, (char*) "Hello Server\r\n");

	ret = WIFI_SendData(SOCKET, data, len, &SentDataLength,
			WIFI_WRITE_TIMEOUT);

	if ((ret == WIFI_STATUS_OK) && (SentDataLength != len)) {
		ret = WIFI_STATUS_ERROR;
	}

	return ret;
} // int send_data_to_server()

int send_data_to_ip(uint8_t *data) {
	uint16_t SentDataLength;
	WIFI_Status_t ret;

	strcpy((char*) http, (char*) "Hello Server\r\n");

	ret = WIFI_SendData(SOCKET, http, strlen((char*) http), &SentDataLength,
			WIFI_WRITE_TIMEOUT);

	if ((ret == WIFI_STATUS_OK) && (SentDataLength != strlen((char*) http))) {
		ret = WIFI_STATUS_ERROR;
	}

	return ret;
}

/**
 * static int wifi_start(void)
 * initializes this devices WIFI module
 */
static int wifi_start(void) {
	uint8_t MAC_Addr[6];

	/*Initialize and use WIFI module */
	if (WIFI_Init() == WIFI_STATUS_OK) {
		LOG(("ES-WIFI Initialized.\n"));
		if (WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK) {
			LOG(
					("> eS-WiFi module MAC Address : %02X:%02X:%02X:%02X:%02X:%02X\n", MAC_Addr[0], MAC_Addr[1], MAC_Addr[2], MAC_Addr[3], MAC_Addr[4], MAC_Addr[5]));
		} else {
			LOG(("> ERROR : CANNOT get MAC address\n"));
			return -1;
		}
	} else {
		return -1;
	}
	return 0;
} // static int wifi_start(void)

/*
 * int wifi_connect(void)
 * Connect device to Network using SSID and PASSWORD
 */
int wifi_connect(void) {

	wifi_start();

	LOG(("\nConnecting to %s , %s\n","SSID","PASSWORD"));
	if (WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK) {
		if (WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK) {
			LOG(
					("> es-wifi module connected: got IP Address : %d.%d.%d.%d\n", IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3]));
		} else {
			LOG((" ERROR : es-wifi module CANNOT get IP address\n"));
			return -1;
		}
	} else {
		LOG(("ERROR : es-wifi module NOT connected\n"));
		return -1;
	}
	return 0;
} // int wifi_connect(void)

/**
 * @brief  Gets numeric values from the Hyperterminal.
 * @param  Maximum value allowed (value)
 * @retval The character received
 */
uint32_t Serial_Scanf(uint32_t value) {
	uint16_t tmp = 0;

	tmp = getchar();

	if (tmp > value) {
		printf("\n\r  !!! Please enter valid number between 0 and %lu \n",
				value);
		return 0xFF;
	}
	return tmp;
}

#if defined (TERMINAL_USE)
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	HAL_UART_Transmit(&hDiscoUart, (uint8_t*) &ch, 1, 0xFFFF);

	return ch;
}

/**
 * @brief Retargets the C library scanf function to the USART.
 * @param None
 * @retval None
 */
GETCHAR_PROTOTYPE {
	/* Place your implementation of fgetc here */
	/* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
	uint8_t ch = 0;
	while (HAL_OK != HAL_UART_Receive(&hDiscoUart, (uint8_t*) &ch, 1, 30000)) {
		;
	}
	return ch;
}
#endif /* TERMINAL_USE */

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case (GPIO_PIN_1): {
		SPI_WIFI_ISR();
		break;
	}
	default: {
		break;
	}
	}
}

/**
 * @brief  SPI3 line detection callback.
 * @param  None
 * @retval None
 */
void SPI3_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi);
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM17 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM17) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

