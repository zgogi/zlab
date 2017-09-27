#include "usb_cdc_device.h"
#include "usbd_cdc.h"
#include "usbd_desc.h"
#include "stm32f3xx_hal.h"
#include "hal/gpio.h"
#include "comm/comm.h"
#include "config.h"

#define USB_MAX_PACKET_SIZE  CDC_DATA_FS_MAX_PACKET_SIZE

uint8_t 			usb_rx_buffer[USB_MAX_PACKET_SIZE];
USBD_HandleTypeDef 	usb_handle;

static int8_t cdc_usb_init(void);
static int8_t cdc_usb_deinit(void);
static int8_t cdc_usb_control(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t cdc_usb_receive(uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS = 
{
	cdc_usb_init,
	cdc_usb_deinit,
	cdc_usb_control,  
	cdc_usb_receive
};

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void usb_init(void)
{
  	HAL_Init();
	SystemClock_Config();
	HAL_GPIO_CLK_ENABLE(USB_GPIO);
	USBD_Init(&usb_handle, &FS_Desc, DEVICE_FS);
  	USBD_RegisterClass(&usb_handle, &USBD_CDC);
  	USBD_CDC_RegisterInterface(&usb_handle, &USBD_Interface_fops_FS);
  	USBD_Start(&usb_handle);
}

static int8_t cdc_usb_init(void)
{ 
	USBD_CDC_SetRxBuffer(&usb_handle, usb_rx_buffer);
	return (USBD_OK);
}

static int8_t cdc_usb_deinit(void)
{
  	return (USBD_OK);
}

static int8_t cdc_usb_control(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{ 
	return (USBD_OK);
}

static int8_t cdc_usb_receive(uint8_t* buf, uint32_t *size)
{
	USBD_CDC_ReceivePacket(&usb_handle);
	//host_rx(buf, *size);
	comm_receive(buf, (int)(*size));
	return USBD_OK;
}

size_t usb_send(const uint8_t* buf, size_t size)
{
  	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)usb_handle.pClassData;
  	if (0 == hcdc->TxState)
	{
	  
	 	// Fix error on sending 64,128,etc bytes at once
		// TODO fix this error less ugly
	  	if (size >= USB_MAX_PACKET_SIZE)
		  	size = USB_MAX_PACKET_SIZE-1; 
		
		USBD_CDC_SetTxBuffer(&usb_handle, (uint8_t*)buf, size);
  		if (USBD_OK == USBD_CDC_TransmitPacket(&usb_handle))
  			return size;
	}
	return 0;
}


