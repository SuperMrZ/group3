
#include "stm32f4xx.h"                  // Device header
#include "yaokong.h"
#include "can_user.h"


int16_t yuntai_angle_target[3];
extern motor_recieve motor_recieve_yuntai3508[3];

void zhuangdan(uint16_t* bodan_target_angle,int16_t yuntai_speed_target[3])
{
	if(*bodan_target_angle>8192)
	{
		yuntai_speed_target[0]=2000;
		CAN_cmd_speed_yuntaimotor(yuntai_speed_target,motor_recieve_yuntai3508);
		if(motor_recieve_yuntai3508[0].last_angle - motor_recieve_yuntai3508[0].angle > 4096)
		{
			*bodan_target_angle = *bodan_target_angle - 8192;
			
		}
		motor_recieve_yuntai3508[0].last_angle = motor_recieve_yuntai3508[0].angle;
	}
	if(*bodan_target_angle<=8192)
	{
		yuntai_angle_target[0] = *bodan_target_angle;
	//	CAN_cmd_angle_yuntaimotor(yuntai_angle_target,motor_recieve_yuntai3508);
		CAN_cmd_angle_single_yuntaimotor(yuntai_angle_target[0],1,motor_recieve_yuntai3508);
		yuntai_speed_target[0]=0;
		CAN_cmd_speed_yuntaimotor(yuntai_speed_target,motor_recieve_yuntai3508);
		
	}
	
}
