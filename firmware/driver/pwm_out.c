#include "pwm_out.h"
#include "hal/gpio.h"
#include "hal/timer.h"

#if ZLAB_USE_PWM_OUT

#define TIM_CCMR1_PWMMODE1 	TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2

void pwm_out_init(void)
{
  	HAL_GPIO_CLK_ENABLE(PWM_OUT_GPIO);
  	HAL_TIMER_CLK_ENABLE(PWM_OUT_TIMER);
	
  	PWM_OUT_TIMER->CCMR1 = TIM_CCMR1_PWMMODE1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE;
	PWM_OUT_TIMER->CCER = TIM_CCER_CC1E;
	PWM_OUT_TIMER->ARR = 200;
  	PWM_OUT_TIMER->CCR1 = 100;
	
	SET_BIT2(PWM_OUT_GPIO->OSPEEDR, PWM_OUT_PIN, 3); //High speed
	SET_BIT2(PWM_OUT_GPIO->MODER, PWM_OUT_PIN, 2);   // Alternate
	SET_BIT4(PWM_OUT_GPIO->AFR[1], 0, 6); // AF6 on AFRH
	//HAL_GPIO_SET_AF(PWM_OUT_GPIO, PWM_OUT_PIN, 6);
}

void pwm_out_start(void)
{
  	PWM_OUT_TIMER->CR1 |= TIM_CR1_CEN;
	PWM_OUT_TIMER->BDTR |= TIM_BDTR_MOE;
}

void pwm_out_stop(void)
{
  	PWM_OUT_TIMER->CR1 &= ~TIM_CR1_CEN;
	PWM_OUT_TIMER->BDTR &= ~TIM_BDTR_MOE;
}

int pwm_out_get_active(void)
{
 	return (PWM_OUT_TIMER->CR1 & TIM_CR1_CEN) != 0;
}

float pwm_out_get_pulse(void)
{
  	float psc = (float)(PWM_OUT_TIMER->PSC+1);
  	float pl = (float)PWM_OUT_TIMER->CCR1+1;
  	return (psc * pl) / (float)PWM_OUT_CLK;
}

float pwm_out_get_period(void)
{
  	float psc = (float)(PWM_OUT_TIMER->PSC+1);
	float pr = (float)PWM_OUT_TIMER->ARR+1;
  	return (psc * pr) / (float)PWM_OUT_CLK;
}

void pwm_out_set_time(float pulse, float period)
{
  	if (pulse >= period) return;
	
  	uint32_t pl = (uint32_t)(pulse  * (float)PWM_OUT_CLK)-1;
  	uint32_t pr = (uint32_t)(period * (float)PWM_OUT_CLK)-1;
	uint32_t psc = 0;

	if (pr > UINT16_MAX)
	{
	  	psc = pr / UINT16_MAX;
		pr = UINT16_MAX;
	  	pl = (uint32_t)((float)pr * (pulse / period));
	}
	
	if (pl < 1) pl = 1;
	if (pr < 2) pr = 2;
	
	PWM_OUT_TIMER->PSC = psc;
 	PWM_OUT_TIMER->CCR1 = pl;
	PWM_OUT_TIMER->ARR  = pr;
}


#endif //ZLAB_USE_PWM_OUT