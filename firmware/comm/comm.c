#include "comm.h"
#include "usb_cdc_device.h"
#include "sump.h"

#define COMM_MODE_OFF	0
#define COMM_MODE_SUMP	1

#define COMM_RX_BUFER_SIZE	64

int comm_mode = COMM_MODE_OFF;
// We need global buffer in case if we receive commands in few parts
//uint8_t comm_rx_buffer[COMM_RX_BUFFER_SIZE];
//int comm_rx_size = 0;

void comm_init(void)
{
 	usb_init();
	sump_init(comm_transmit);
}

void comm_transmit(const uint8_t *buf, int size)
{
	while (size)
	{
	  	size_t sended = usb_send(buf, size);
		buf += sended;
		size -= sended;
	}
}

void comm_receive(const uint8_t *buf, int size)
{
 	sump_receive(buf, size);
	comm_mode = COMM_MODE_SUMP;
}

void comm_tick(void)
{
	if (COMM_MODE_SUMP == comm_mode)
		sump_tick();
}
