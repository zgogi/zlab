#ifndef ZLAB_PWM_OUT_H_INCLUDED
#define ZLAB_PWM_OUT_H_INCLUDED

#include "config.h"
#include <stdint.h>

#ifdef ZLAB_USE_PWM_OUT

#define PWM_OUT_GPIO        GPIOA
#define PWM_OUT_PIN         8
#define PWM_OUT_TIMER       TIM1
#define PWM_OUT_TIMER_CH    TIM_CHANNEL_1
#define PWM_OUT_CLK			48000000UL




// Freq = CLK/((PSC+1)*(ARR+1))
#define PWM_OUT_MIN_PERIOD	( (float)PWM_OUT_CLK / ( ((float)UINT16_MAX+1)*((float)UINT16_MAX+1) ) )
#define PWM_OUT_MAX_PERIOD	( PWM_OUT_CLK / 2 )


void pwm_out_init(void);
void pwm_out_start(void);
void pwm_out_stop(void);
int  pwm_out_get_active(void);

float pwm_out_get_pulse(void);
float pwm_out_get_period(void);

void pwm_out_set_time(float pulse, float period);



#endif //ZLAB_USE_PWM_OUT

#endif //ZLAB_PWM_OUT_H_INCLUDED
