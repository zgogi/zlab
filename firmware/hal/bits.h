#ifndef BITS_H_INCLUDED
#define BITS_H_INCLUDED


/***********************************************
How to use it
For example we have register=0000 0000 0000 0000
SET_BIT2(reg, 0, 0b01);     =0000 0000 0000 0001
SET_BIT2(reg, 2, 0b11);     =0000 0000 0011 0001
SET_BIT4(reg, 2, 0b1010);   =0000 1010 0011 0001
CLEAR_BIT4(reg, 2);         =0000 0000 0011 0001
***********************************************/

#define BYTE1(v) 	((uint8_t)v & 0xff)         //LSB
#define BYTE2(v) 	((uint8_t)(v >> 8) & 0xff)  //
#define BYTE3(v) 	((uint8_t)(v >> 16) & 0xff) //
#define BYTE4(v) 	((uint8_t)(v >> 24) & 0xff) //MSB

#define SHORT1(v)	((uint16_t)v & 0xFFFF)
#define SHORT2(v)	((uint16_t)(v >> 16) & 0xFFFF)

#define BIT2_MASK(pos) (0x03 << (pos*2))
#define CLEAR_BIT2(reg, pos) do { reg &= ~BIT2_MASK(pos); } while(0)
#define SET_BIT2(reg, pos, value) do { reg |= ((value) & 3) << ((pos)*2); } while(0)

#define BIT4_MASK(pos) (0x0F << ((pos)*4))
#define CLEAR_BIT4(reg, pos) do { reg &= ~BIT4_MASK(pos); } while(0)
#define SET_BIT4(reg, pos, value) do { (reg) |= (((value) & 0x0f) << ((pos)*4)); } while(0)

#define SET_MASKED16_1(reg, value) do { (reg) = ((value) & 0xFFFF) | ((reg) & 0xFFFF0000); } while(0) 


#define RANGE(v, min, max)	(v>=min) ? ((v<=max)?v:max) : min



#endif