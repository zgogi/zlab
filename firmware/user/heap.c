#include "heap.h"

uint8_t heap[ZLAB_HEAP_SIZE];


#if 0
heap_def heap[ZLAB_HEAP_BLOCK_COUNT];

void heap_init(void)
{
    int i;
    for (i=0 ; i<ZLAB_HEAP_BLOCK_COUNT ; ++i)
		heap[i].status = HEAP_STATUS_EMPTY;
}

heap_entry* heap_alloc(const char *mod, size_t size)
{
	heap_def *ptr = heap_find(HEAP_STATUS_EMPTY);
	if (NULL != ptr)
	{
		ptr->mod[0] = mod[0];
		ptr->mod[1] = mod[1];
		ptr->size = (uint8_t)(size-1);
		ptr->status = HEAP_STATUS_LOCKED;
	}
	return ptr;
}

heap_def* heap_find(int status)
{
	int i;
    for (i=0 ; i<ZLAB_HEAP_BLOCK_COUNT ; ++i)
		if (heap[i].status = status)
			return &heap[i];
	return NULL;
}
#endif
