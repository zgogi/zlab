#include "digit_in.h"
#include "config.h"
#include "hal/timer.h"
#include "hal/gpio.h"
#include "hal/dma.h"

#if ZLAB_USE_DIGIT_IN

#define MODE_IDLE				0
#define MODE_WAIT_TRIGGER		1
#define MODE_RUN_CIRCLE			2
#define MODE_RUN_ONCE			3
#define MODE_FINISHED			4

static volatile int	   	 di_mode = MODE_IDLE;
static volatile int		 di_head_volatile = 0;


di_struct 	di;



void digit_in_init(void)
{
  	di.buffer = &heap[0];
	di.head = 0;
	di.size = ZLAB_HEAP_SIZE;
	di.freq = 100000UL;
	di.trigger_mask = 0;
	di.trigger_value = 0;
	di.trigger_delay = ZLAB_HEAP_SIZE / 2;
	
	HAL_GPIO_CLK_ENABLE(DIGIT_IN_GPIO);
	HAL_DMA_CLK_ENABLE(DIGIT_IN_DMA);
	HAL_TIMER_CLK_ENABLE(DIGIT_IN_TIMER);
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	DIGIT_IN_GPIO->MODER   = 0; // Input
	DIGIT_IN_GPIO->OSPEEDR = 0xFFFFFFFF; // High speed
	DIGIT_IN_GPIO->PUPDR   = 0x55555555; // PullUp

	uint32_t extiCR = 0;
	if (GPIOA == DIGIT_IN_GPIO) extiCR = 0x0000;
	if (GPIOB == DIGIT_IN_GPIO) extiCR = 0x1111;
	if (GPIOC == DIGIT_IN_GPIO) extiCR = 0x2222;
	if (GPIOD == DIGIT_IN_GPIO) extiCR = 0x3333;
	if (GPIOE == DIGIT_IN_GPIO) extiCR = 0x4444;
	if (GPIOF == DIGIT_IN_GPIO) extiCR = 0x5555;

	SYSCFG->EXTICR[0] = extiCR;
	SYSCFG->EXTICR[1] = extiCR;
	SYSCFG->EXTICR[2] = extiCR;
	SYSCFG->EXTICR[3] = extiCR;
}

static inline void start_dma(int circle)
{
  	DIGIT_IN_DMA_CH->CPAR = (uint32_t)(&(DIGIT_IN_GPIO->IDR));
	DIGIT_IN_DMA_CH->CMAR = (uint32_t)(&di.buffer[0]);
  	DIGIT_IN_DMA_CH->CNDTR = di.size;
	NVIC_EnableIRQ(DIGIT_IN_DMA_IRQn);
	
	uint32_t ccr =  DMA_CCR_PL_VERYHIGH 
	  	  			| DMA_CCR_PSIZE_8 
		  			| DMA_CCR_MSIZE_8 
		 			| DMA_CCR_MINC 
					| DMA_CCR_DIR_P_to_M
					| DMA_CCR_TCIE
					| DMA_CCR_EN;	
	
	if (circle)
	  	ccr |= DMA_CCR_CIRC;
	
	DIGIT_IN_DMA_CH->CCR = ccr;
}

static inline void restart_dma_once(uint16_t count)
{
  	DIGIT_IN_DMA_CH->CCR &= ~(DMA_CCR_CIRC | DMA_CCR_EN);
	DIGIT_IN_DMA_CH->CNDTR = count;
	DIGIT_IN_DMA_CH->CCR |= DMA_CCR_EN;
}

static inline void stop_dma(void)
{
  	NVIC_DisableIRQ(DIGIT_IN_DMA_IRQn);
	DIGIT_IN_DMA_CH->CCR = 0;
}

static inline void start_exti(void)
{
  	EXTI->PR = 0xFFFF;
  	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_TSC_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	SET_MASKED16_1(EXTI->RTSR, (di.trigger_mask &  di.trigger_value));
	SET_MASKED16_1(EXTI->FTSR, (di.trigger_mask & ~di.trigger_value));
	SET_MASKED16_1(EXTI->IMR, di.trigger_mask);
}

static inline void stop_exti(void)
{
  	EXTI->IMR = 0;
  	NVIC_DisableIRQ(EXTI0_IRQn);
	NVIC_DisableIRQ(EXTI1_IRQn);
	NVIC_DisableIRQ(EXTI2_TSC_IRQn);
	NVIC_DisableIRQ(EXTI3_IRQn);
	NVIC_DisableIRQ(EXTI4_IRQn);
	NVIC_DisableIRQ(EXTI9_5_IRQn);
	EXTI->PR = 0xFFFF;
}

static inline void start_timer(void)
{
  	DIGIT_IN_TIMER->CR2 = TIM_CR2_CCDS; // DMA Request on update
	DIGIT_IN_TIMER->DIER = TIM_DIER_UDE; //Update DMA en
	DIGIT_IN_TIMER->ARR = DIGIT_IN_CLK / di.freq;
	DIGIT_IN_TIMER->CNT = 0;
  	DIGIT_IN_TIMER->CR1 = TIM_CR1_CEN;	
}

static inline void stop_timer(void)
{
  	DIGIT_IN_TIMER->CR1 = 0;
}

void digit_in_start(void)
{
	if (di.trigger_mask)
	{
	  	start_exti();
	  	start_dma(1);
		di_mode = MODE_WAIT_TRIGGER;
	}
	else
	{
	  	start_dma(0);
		di_mode = MODE_RUN_ONCE;
	}
		
	start_timer();
}

void digit_in_stop(void)
{
  	stop_timer();
	stop_exti();
	stop_dma();
}

int digit_in_started(void)
{
  	return (DIGIT_IN_TIMER->CR1 & TIM_CR1_CEN) != 0;
}

int digit_in_ready(void) 
{ 
  	if (MODE_FINISHED == di_mode)
	{
	  	di.head = di_head_volatile;
	  	return 1;
	}
	else
	  	return 0;
}

void digit_in_clear(void)
{
	di_mode = MODE_IDLE;
}

void digit_in_trigger_irq(void)
{
  	di_head_volatile = (int)di.size - (int)DIGIT_IN_DMA_CH->CNDTR - (int)di.trigger_delay;
	if (di_head_volatile < 0) di_head_volatile += di.size;
  	di_mode = MODE_RUN_CIRCLE;
	stop_exti();
}

void DIGIT_IN_DMA_IRQ_HANDLER(void)
{
  	DIGIT_IN_DMA->IFCR = 0x00F0;
  	SET_BIT4(DIGIT_IN_DMA->IFCR, DIGIT_IN_DMA_CHn-1, 0x0F); //Reset all interrupt flags
	
	
	switch (di_mode)
	{
	case MODE_RUN_CIRCLE:
	  	restart_dma_once(di_head_volatile);
	  	di_mode = MODE_RUN_ONCE;
	  	break;
	case MODE_RUN_ONCE:
	  	digit_in_stop();
		di_mode = MODE_FINISHED;
	  	break;	  
	}
}

void EXTI0_IRQHandler(void) 	{ digit_in_trigger_irq(); }
void EXTI1_IRQHandler(void) 	{ digit_in_trigger_irq(); }
void EXTI2_TSC_IRQHandler(void) { digit_in_trigger_irq(); }
void EXTI3_IRQHandler(void)		{ digit_in_trigger_irq(); }
void EXTI4_IRQHandler(void) 	{ digit_in_trigger_irq(); }
void EXTI9_5_IRQHandler(void)	{ digit_in_trigger_irq(); }

#endif //ZLAB_USE_DIGIT_IN
