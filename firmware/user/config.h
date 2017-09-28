#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#ifndef HSE_VALUE
	#define HSE_VALUE    	8000000UL 
#endif
#ifndef HSI_VALUE
	#define HSI_VALUE    	8000000UL
#endif

#define CLK_USE_HSE  // Comment this line to use HSI
#define CLK_AHB			72000000UL


#define ZLAB_USE_PWM_OUT		1
#define ZLAB_USE_DIGIT_IN 		1

#define ZLAB_HEAP_SIZE			(24*1024)

// For future
#define ZLAB_HEAP_BLOCK_SIZE	1024
#define ZLAB_HEAP_BLOCK_COUNT	24

#endif //CONFIG_H_INCLUDED
