#ifndef STM32_HAL_TIMER_H_INCLUDED
#define STM32_HAL_TIMER_H_INCLUDED

#include "stm32f3xx.h"

#define HAL_TIMER_CLK_ENABLE(tim)   do { if (TIM1 == tim) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; \
							 if (TIM2 == tim) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; \
							 if (TIM3 == tim) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; \
							 if (TIM4 == tim) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; \
							 if (TIM6 == tim) RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; \
							 if (TIM7 == tim) RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; \
							 if (TIM8 == tim) RCC->APB2ENR |= RCC_APB2ENR_TIM8EN; \
							} while (0)

#endif
