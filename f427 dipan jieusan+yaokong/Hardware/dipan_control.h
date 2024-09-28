#ifndef _DIPAN_CONTROL
#define _DIPAN_CONTROL

typedef struct
{
	struct
	{ 
		unsigned short ch0;
		unsigned short ch1;
		unsigned short ch2;
		unsigned short ch3;
		unsigned char s1;
		unsigned char s2;
	}rc;
	
//	struct 
//	{
//		unsigned short x;
//		unsigned short y;
//		unsigned short z;
//		unsigned char press_l;
//		unsigned char press_r;
//	}mouse;
	
	struct
	{
		unsigned short v;
	}key;
}RC_Ctl_t;

extern 	RC_Ctl_t RC_Ctl;

 extern uint8_t sbus_rx_buffer[18];//声明遥控器接收缓存数组

extern float dipan_speedtarget[4];
void dipan_speed_jiesuan(RC_Ctl_t RC_Ctl);



#endif
