#ifndef COMM_H_INCLUDED
#define COMM_H_INCLUDED

#include <stdint.h>

#define USB_GPIO			GPIOA

void comm_init(void);
void comm_transmit(const uint8_t *buf, int size);
void comm_receive(const uint8_t *buf, int size);
void comm_tick(void);

#endif