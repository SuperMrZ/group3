#ifndef _YAOKONG_H
#define _YAOKONG_H

typedef struct
{
	struct
	{ 
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint16_t s1;
		uint16_t s2;
	}rc;
	
//	struct 
//	{
//		unsigned short x;
//		unsigned short y;
//		unsigned short z;
//		unsigned char press_l;
//		unsigned char press_r;
//	}mouse;
	
//	struct
//	{
//		unsigned short v;
//	}key;
}RC_Ctl_t;

extern 	RC_Ctl_t RC_Ctl;

extern uint8_t sbus_rx_buffer[18];//声明遥控器接收缓存数组





#endif
