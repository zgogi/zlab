#ifndef SUMP_H_INCLUDED
#define SUMP_H_INCLUDED

#include <stdint.h>

typedef void (*sump_transmit_func)(const uint8_t *buf, int size);

void sump_init(sump_transmit_func func);
void sump_receive(const uint8_t *buf, int len);
void sump_tick(void);


#endif