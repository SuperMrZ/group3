#include "stm32f4xx.h"                  // Device header
#include "can_user.h"
#include "yaokong.h"
#include "dipan_control.h"

extern 	RC_Ctl_t RC_Ctl;
extern int16_t yuntai_angletarget[2];
extern int16_t dipan_speedtarget[4];

void dipan_gensui()
{
	//step1.遥控控制yaw6020转动x角度
	CAN_cmd_angle_6020motor(yuntai_angletarget,motor_recieve_yuntai6020);
	
	
	//step2.底盘转动x角度
	int16_t angle_diff;
	int16_t a=40;
	angle_diff = yuntai_angletarget[1]-4000;
	
	if(angle_diff<=3&&angle_diff>=-3)//防止摇杆零点漂移带来误差
	{
		angle_diff=0;
	}
	
	dipan_speedtarget[0] += -a*angle_diff;
	dipan_speedtarget[1] += -a*angle_diff; 
	dipan_speedtarget[2] += -a*angle_diff; 
	dipan_speedtarget[3] += -a*angle_diff; 
	CAN_cmd_speed_3508motor(dipan_speedtarget,motor_recieve_dipan3508);

	
	//yaw6020回转x角度
	HAL_Delay(1);

	yuntai_angletarget[1]=4000;
	CAN_cmd_angle_6020motor(yuntai_angletarget,motor_recieve_yuntai6020);
	

	
}
