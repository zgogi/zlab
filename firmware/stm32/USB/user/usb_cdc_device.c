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


