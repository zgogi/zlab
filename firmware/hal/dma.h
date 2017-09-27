#ifndef STM32_HAL_DMA_H_INCLUDED
#define STM32_HAL_DMA_H_INCLUDED

#include "stm32f3xx.h"

#define HAL_DMA_CLK_ENABLE(dma)	do { if (DMA1 == dma) RCC->AHBENR |= RCC_AHBENR_DMA1EN; \
								 if (DMA2 == dma) RCC->AHBENR |= RCC_AHBENR_DMA2EN; \
								 } while (0)

static inline void hal_dma_start(DMA_Channel_TypeDef *dma, uint32_t *periph, uint32_t *mem, size_t size)
{
  	dma->CPAR = (uint32_t)(periph);
	dma->CMAR = (uint32_t)(mem);
	dma->CNDTR = size;
	dma->CCR |= DMA_CCR_EN;
}

static inline void hal_dma_stop(DMA_Channel_TypeDef *dma)
{
  	dma->CCR &= ~DMA_CCR_EN;
}			

								  
#define DMA_CCR_PL_LOW   			0
#define DMA_CCR_PL_MEDIUM   		DMA_CCR_PL_0
#define DMA_CCR_PL_HIGH   			DMA_CCR_PL_1
#define DMA_CCR_PL_VERYHIGH   		DMA_CCR_PL_0 | DMA_CCR_PL_1

#define DMA_CCR_PSIZE_8  			0
#define DMA_CCR_PSIZE_16  			DMA_CCR_PSIZE_0
#define DMA_CCR_PSIZE_32  			DMA_CCR_PSIZE_1

#define DMA_CCR_MSIZE_8  			0
#define DMA_CCR_MSIZE_16  			DMA_CCR_MSIZE_0
#define DMA_CCR_MSIZE_32  			DMA_CCR_MSIZE_1

#define DMA_CCR_DIR_P_to_M			0
#define DMA_CCR_DIR_M_to_P			DMA_CCR_DIR

								   
#endif
