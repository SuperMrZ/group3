#ifndef _YK_H
#define _YK_H

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
		unsigned char s2_last;
	}rc;
	
	
	
}RC_Ctl_t;

extern 	RC_Ctl_t RC_Ctl;

 extern uint8_t sbus_rx_buffer[18];//声明遥控器接收缓存数组

extern int8_t yaokongjishi; 

#endif
