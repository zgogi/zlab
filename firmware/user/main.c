#include "main.h"
#include "driver/driver.h"
#include "comm/comm.h"
#include "driver/pwm_out.h"



int main(void)
{
  	drv_init();
	comm_init();
	
	// Debug
	pwm_out_set_time(0.001, 0.005);
	pwm_out_start();
	// End debug
	
	while (1)
	{ 
		comm_tick();
	}
}


void _Error_Handler(char * file, int line)
{
  while(1)
  {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}

#endif

