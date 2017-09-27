#include "sump.h"
#include "driver/digit_in.h"
#include "heap.h"
#include "hal/bits.h"

//frequency base of the original SUMP logic analyzer
//used for converting frequency dividers
#define SUMP_ORIGINAL_FREQ	(100000000UL)

//short sump commands
#define SUMP_CMD_RESET	0x00
#define SUMP_CMD_RUN	0x01
#define SUMP_CMD_ID		0x02
#define SUMP_CMD_META	0x04
//long sump commands
#define SUMP_CMD_SET_SAMPLE_RATE	0x80
#define SUMP_CMD_SET_COUNTS			0x81
#define SUMP_CMD_SET_FLAGS			0x82
//basic triggers
#define SUMP_CMD_SET_BT0_MASK		0xC0
#define SUMP_CMD_SET_BT0_VALUE		0xC1

#define SUMP_META_END	0x00
#define SUMP_META_NAME	0x01
#define SUMP_META_FPGA_VERSION	0x02
#define SUMP_META_CPU_VERSION	0x03
#define SUMP_META_PROBES_DW		0x20
#define SUMP_META_SAMPLE_RAM	0x21
#define SUMP_META_DYNAMIC_RAM	0x22
#define SUMP_META_SAMPLE_RATE	0x23
#define SUMP_META_PROTOCOL		0x24
#define SUMP_META_PROBES_B		0x40
#define SUMP_META_PROTOCOL_B	0x41

#define SUMP_FLAG1_DDR			0x0001
#define SUMP_FLAG1_NOISE_FILTER	0x0002
#define SUMP_FLAG1_GROUPS		0x003C
#define SUMP_FLAG1_GR0_DISABLE	0x0004
#define SUMP_FLAG1_GR1_DISABLE	0x0008
#define SUMP_FLAG1_GR2_DISABLE	0x0010
#define SUMP_FLAG1_GR3_DISABLE	0x0020
#define SUMP_FLAG1_ENABLE_RLE	0x0100

#define SUMP_FLAG1_GR_8BIT		(SUMP_FLAG1_GR1_DISABLE | SUMP_FLAG1_GR2_DISABLE | SUMP_FLAG1_GR3_DISABLE)
#define SUMP_FLAG1_GR_16BIT		(SUMP_FLAG1_GR2_DISABLE | SUMP_FLAG1_GR3_DISABLE)
#define SUMP_FLAG1_GR_32BIT		(0)

#define SUMP_NAME				"ZLAB logic analizer"
#define SUMP_FPGA_VERSION		"No FPGA :("
#define SUMP_CPU_VERSION		"0.1b"
#define SUMP_CHANNEL_COUNT		DIGIT_IN_MAX_CHANNELS
#define SUMP_MAX_MEMORY			ZLAB_HEAP_SIZE
#define SUMP_MAX_FREQ			(DIGIT_IN_MAX_FREQ / SUMP_ORIGINAL_FREQ - 1)
#define SUMP_EXTENDED_PROTOCOL	2



static const uint8_t sump_meta_data[] 
     = {SUMP_META_NAME, 'L', 'o', 'g', 'i', 'c', ' ', 'a', 'n', 'a', 'l','y', 'z', 'e', 'r', 0,
		SUMP_META_FPGA_VERSION, 'N', 'o', 'F', 'P', 'G', 'A', ' ', ':', '(', 0,
		SUMP_META_CPU_VERSION, 'V', 'e', 'r', 'y', ' ','b' ,'e', 't', 'a', 0,
		SUMP_META_SAMPLE_RATE, BYTE4(SUMP_CHANNEL_COUNT), BYTE3(SUMP_CHANNEL_COUNT), BYTE2(SUMP_MAX_FREQ), BYTE1(SUMP_MAX_FREQ),
		SUMP_META_SAMPLE_RAM, 0, 0, BYTE2(SUMP_MAX_MEMORY), BYTE1(SUMP_MAX_MEMORY), //24*1024 b
		SUMP_META_PROBES_B, 8,
		SUMP_META_PROTOCOL_B, 2,
		SUMP_META_END
};


static sump_transmit_func sump_transmit = NULL;

int sump_is_short_cmd(uint8_t command);

void sump_init(sump_transmit_func func)
{
	sump_transmit = func;  
}

void sump_receive(const uint8_t *buf, int size)
{
  	switch(buf[0])
	{
	case SUMP_CMD_RESET:
		digit_in_stop();
	  	break;
	case SUMP_CMD_RUN:
		digit_in_start();
	  	break;
	case SUMP_CMD_ID:
		sump_transmit("1ALS", 4);
	  	break;
	case SUMP_CMD_META:
		sump_transmit(sump_meta_data, sizeof(sump_meta_data));
	  	break;
	case SUMP_CMD_SET_SAMPLE_RATE:
		if(5 == size)
		{
		  	// f = 100MHz / (x+1)		  
		  	uint32_t divider = *((uint32_t*)(buf+1));
			digit_in_set_freq(SUMP_ORIGINAL_FREQ / (divider+1));
		}
		break;
	case SUMP_CMD_SET_COUNTS:
		if(5 == size)
		{
			uint16_t readCount  = 1 + *((uint16_t*)(buf+1));
			uint16_t delayCount = *((uint16_t*)(buf+3));
			digit_in_set_size(4*readCount);
			digit_in_set_delay(4*(readCount-delayCount));			
		}
		break;
	case SUMP_CMD_SET_BT0_MASK:
		if(5 == size)
		  	digit_in_set_trigger_mask(*(uint16_t*)(buf+1));
		break;
	case SUMP_CMD_SET_BT0_VALUE:
		if(5 == size)
		{
			digit_in_set_trigger_value(*(uint16_t*)(buf+1));
		}
		break;
	case SUMP_CMD_SET_FLAGS:
		if(5 == size)
		{
			//sampler.SetFlags(*(uint16_t*)(buffer+1));
		}
		break;
	}
}

void sump_send_data(void)
{
  	// We need to send data in reverse order
	int pos = digit_in_get_size()-1;
	while (pos >= 0) {
		uint8_t value = digit_in_get_value(pos);
		sump_transmit(&value, 1);
		--pos;
	};
}

void sump_tick(void)
{
	if (digit_in_ready())
	{
		sump_send_data();
	  	digit_in_clear();
	}
}


int sump_is_short_cmd(uint8_t command)
{
	switch(command)
	{
	case SUMP_CMD_RESET:
	case SUMP_CMD_RUN:
	case SUMP_CMD_ID:
	case SUMP_CMD_META:
		return 1;
	default:
		return 0;
	}
}