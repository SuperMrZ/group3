#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "yaokong.h"

extern 	RC_Ctl_t RC_Ctl;

int16_t dipan_speedtarget[4];
int16_t yuntai_angletarget[2]={4000,4000};
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
	dipan_speedtarget[2] = 3*(-a1*dipan_y_speed  + a2*dipan_x_speed - a3*dipan_z_speed);//右后
	dipan_speedtarget[3] = 3*(-a1*-dipan_y_speed + a2*dipan_x_speed - a3*dipan_z_speed);//左后
	
	yuntai_angletarget[1] +=0.1*(RC_Ctl.rc.ch0 - 1024);
	if(yuntai_angletarget[1]>8190)
	{
		yuntai_angletarget[1]=3;
	}
	if(yuntai_angletarget[1]<2)
	{
		yuntai_angletarget[1]=8190;
	}

	
}	








	

