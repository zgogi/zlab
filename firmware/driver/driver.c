#include "driver.h"
#include "usb_cdc_device.h"

#include "pwm_out.h"
#include "digit_in.h"

void drv_init(void)
{
	pwm_out_init();
	digit_in_init();
}



