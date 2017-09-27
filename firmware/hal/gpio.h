#ifndef STM32_HAL_GPIO_H_INCLUDED
#define STM32_HAL_GPIO_H_INCLUDED

#include "bits.h"
#include "stm32f3xx.h"

#define HAL_GPIO_CLK_ENABLE(gpio)	do { \
							 if (GPIOA == gpio) RCC->AHBENR |= RCC_AHBENR_GPIOAEN; \
 							 if (GPIOB == gpio) RCC->AHBENR |= RCC_AHBENR_GPIOBEN; \
							 if (GPIOC == gpio) RCC->AHBENR |= RCC_AHBENR_GPIOCEN; \
							 if (GPIOD == gpio) RCC->AHBENR |= RCC_AHBENR_GPIODEN; \
							 if (GPIOE == gpio) RCC->AHBENR |= RCC_AHBENR_GPIOEEN; \
							 if (GPIOF == gpio) RCC->AHBENR |= RCC_AHBENR_GPIOFEN; \
							} while (0)
							  
					  
		
#define HAL_GPIO_SET_AF(port, pin, af) do { \
							  			if (pin < 8) SET_BIT4(port->AFR[0], pin,     af); \
								  		else         SET_BIT4(port->AFR[1], (pin-8), af); \
									} while (0)							  
					
#define HAL_GPIO_PULL_NONE			0
#define HAL_GPIO_PULL_UP			1
#define HAL_GPIO_PULL_DOWN			2									  
									  
							  
#endif							  