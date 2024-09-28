#include "stm32f4xx.h"                  // Device header
#include "usart.h"

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


float dipan_speedtarget[4];
int16_t dipan_x_speed,dipan_y_speed,dipan_z_speed;//底盘前后，左右，自旋速度
	
void dipan_speed_jiesuan(RC_Ctl_t RC_Ctl)
{

	int16_t a1=1,a2=1,a3=1;//速度系数
	//
	dipan_y_speed = RC_Ctl.rc.ch3-1024;
	dipan_x_speed = RC_Ctl.rc.ch2-1024;
	//s1拨杆控制是否沿z轴自旋
	if (RC_Ctl.rc.s1==3)
	{
		dipan_z_speed = 0;
	}
	else if(RC_Ctl.rc.s1 == 1)
	{
		dipan_z_speed = 0x150 ;
	}
	else if(RC_Ctl.rc.s1 == 2)
	{
		dipan_z_speed = -0x150 ;
	}

	dipan_speedtarget[0] = 3*(a1*-dipan_y_speed - a2*dipan_x_speed - a3*dipan_z_speed);//右前
	dipan_speedtarget[1] = 3*(a1*dipan_y_speed  - a2*dipan_x_speed - a3*dipan_z_speed);//左前
	dipan_speedtarget[2] = (float)0.7368*3*(-a1*dipan_y_speed  + a2*dipan_x_speed - a3*dipan_z_speed);//右后
	dipan_speedtarget[3] = 0.7368*3*(-a1*-dipan_y_speed + a2*dipan_x_speed - a3*dipan_z_speed);//左后


	
}	




void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
                RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;          
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;       
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;          
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;           
				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                           
				RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003);
	
//				RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8);                    //!< Mouse X axis        
//				RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8);                    //!< Mouse Y axis      
//				RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8);                  //!< Mouse Z axis         
//				RC_Ctl.mouse.press_l = sbus_rx_buffer[12];                                        //!< Mouse Left Is Press      
//				RC_Ctl.mouse.press_r = sbus_rx_buffer[13];                                        //!< Mouse Right Is Press 
//				RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8);   			//!< KeyBoard value
	
	


				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,sbus_rx_buffer,18);	
				__HAL_DMA_DISABLE_IT(huart1.hdmarx ,DMA_IT_HT );	
	}




	

