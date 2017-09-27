#ifndef ZLAB_HEAP_H_INCLUDED
#define ZLAB_HEAP_H_INCLUDED

#include "config.h"
#include <stdlib.h>
#include <stdint.h>

extern uint8_t heap[ZLAB_HEAP_SIZE];




//****************** for future use *****
// Heap divided to blocks 256 bytes each
/*************** USING ****************
1. In TX handler
heap_def *h = heap_get_tx();
if (h) {
	TX_Send(h->data, h->size);
	heap_free(h);
}

2. In RX handler
heap_def *h = heap_alloc("aa", size);
if (h) {
	RX_Read(h->data, size);
	heap_rcvd(h);
}

3. When need buffer for transmit
heap_def *h = heap_alloc("aa", size);
if (h) {
	Fill(h->data, size);
	heap_send(h);
}

************************************/
#if 0
typedef {
	uint8_t status;
	uint8_t size;
	uint8_t mod[2];
	uint8_t data[ZLAB_HEAP_BLOCK_SIZE];
} heap_def;

enum {
	HEAP_STATUS_EMPTY,
	HEAP_STATUS_LOCKED,
	HEAP_STATUS_RX,
	HEAP_STATUS_TX
};


void heap_init(void);

heap_def* heap_alloc(const char *mod, size_t size);

static inline void heap_free(heap_def *h) { h->status = HEAP_STATUS_EMPTY; }
static inline void heap_send(heap_def *h) { h->status = HEAP_STATUS_TX; }
static inline void heap_rcvd(heap_def *h) { h->status = HEAP_STATUS_RX; }

heap_def* heap_find(int status);
static inline heap_def* heap_get_tx(void) { return heap_find(HEAP_STATUS_TX); }
static inline heap_def* heap_get_rx(void) { return heap_find(HEAP_STATUS_RX); }
#endif

#endif // HEAP_H_INCLUDED
