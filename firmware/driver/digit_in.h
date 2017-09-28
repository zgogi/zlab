#ifndef DIGIT_IN_H_INCLUDED
#define DIGIT_IN_H_INCLUDED

#include "config.h"
#include "heap.h"
#include <stdlib.h>
#include "hal/bits.h" //for RANGE macros

#if ZLAB_USE_DIGIT_IN

#define DIGIT_IN_GPIO				GPIOD
#define DIGIT_IN_TIMER				TIM2
#define DIGIT_IN_DMA				DMA1
#define DIGIT_IN_DMA_CHn			2
#define DIGIT_IN_DMA_CH			 	DMA1_Channel2
#define DIGIT_IN_DMA_IRQ_HANDLER	DMA1_Channel2_IRQHandler
#define DIGIT_IN_DMA_IRQn			DMA1_Channel2_IRQn
#define DIGIT_IN_CLK				CLK_AHB

#define DIGIT_IN_MIN_FREQ			(DIGIT_IN_CLK / UINT16_MAX)
#define DIGIT_IN_MAX_FREQ			(DIGIT_IN_CLK / 2)
#define DIGIT_IN_MAX_CHANNELS		8

typedef struct di_tag
{
	uint8_t *buffer;
	int size; 
	uint32_t freq;
	int head; 
	uint16_t trigger_mask;
	uint16_t trigger_value;
	uint16_t trigger_delay;
} di_struct;

extern di_struct di;
  
void digit_in_init(void);
void digit_in_start(void);
void digit_in_stop(void);
int digit_in_started(void);

int digit_in_ready(void);
void digit_in_clear(void);


static inline uint8_t digit_in_get_value(int pos)			
{
  	int idx = di.head + pos;
	if (idx >= di.size)
		return di.buffer[idx-di.size];
	else
	  	return di.buffer[idx];
}

static inline int      	digit_in_get_size(void)          { return di.size; }
static inline void     	digit_in_set_size(int size)      { di.size = RANGE(size, 1, ZLAB_HEAP_SIZE); }
static inline void     	digit_in_set_freq(uint32_t freq) { di.freq = RANGE(freq, DIGIT_IN_MIN_FREQ, DIGIT_IN_MAX_FREQ); }
static inline uint32_t 	digit_in_get_freq(void)          { return di.freq; }
static inline void     	digit_in_set_trigger_mask(uint16_t mask)   { di.trigger_mask = mask; }
static inline void     	digit_in_set_trigger_value(uint16_t value) { di.trigger_value = value; } 
static inline void     	digit_in_set_delay(uint16_t dl)  { di.trigger_delay = dl; }

#endif //ZLAB_USE_DIGIT_IN

#endif // DIGIT_IN_H_INCLUDED