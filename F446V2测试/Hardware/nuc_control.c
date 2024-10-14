#include "nuc_control.h"
#include "usart.h"
#include <string.h>  // for memcpy
#include "can_user.h"

#define MANU 1
#define RxHeader 0xA5
#define TxHeader 0x5A

#define PI 3.1415926f
#define SlowRatio_3508  19.0f
#define SlowRatio_2006  36.0f
#define Radius 48.5f    //unit mm
#define dipan_radius 233 //unit mm
extern int16_t dipan_speedtarget[4];

float real_speedtarget[4];

uint8_t err_flg=0;

uint8_t mode_config=0;
// Tx of auto
Tx_Data_toNuc Tx_Cmd;

// if manu copy this to main.c
// Rx of Manu
Nuc_manu_cmd Manu_Cmd;
uint8_t Manu_Rx_buf[5*sizeof(float)+1];

// if auto copy this to main.c
// Rx of auto
Nuc_auto_cmd Auto_Cmd;
uint8_t Auto_Rx_buf[4*sizeof(float)+1];


void Nuc_Tele_Init()
{
	if(mode_config==MANU)
	{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Manu_Rx_buf,21);
	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //防止接收到一半就停止，跟上一句一定要配套写
	}
	else //auto
	{
		Tx_Cmd.header=TxHeader;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Auto_Rx_buf,17);
	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //防止接收到一半就停止，跟上一句一定要配套写
	}
}


void Nuc_Manu_Decode(uint8_t* rx_buf, Nuc_manu_cmd* cmd)
{
    if (rx_buf[0] == 0xA5)
    {
        cmd->header = rx_buf[0]; 
        memcpy(&(cmd->pitch), rx_buf + 1, sizeof(float));      // ?? pitch
        memcpy(&(cmd->yaw), rx_buf + 5, sizeof(float));        // ?? yaw
        memcpy(&(cmd->position_x), rx_buf + 9, sizeof(float)); // ?? position_x
        memcpy(&(cmd->position_y), rx_buf + 13, sizeof(float));// ?? position_y
        memcpy(&(cmd->shoot), rx_buf + 17, sizeof(float));     // ?? shoot
    }
    else
    {
			err_flg=1;
    }
}

void Nuc_Auto_Decode(uint8_t* rx_buf, Nuc_auto_cmd* cmd)
{
	 if (rx_buf[0] == 0xA5)
    {
        cmd->header = rx_buf[0]; 
        memcpy(&(cmd->angle_error), rx_buf + 1, sizeof(float));      // ?? pitch
        memcpy(&(cmd->linear_speed), rx_buf + 5, sizeof(float));        // ?? yaw
        memcpy(&(cmd->yaw_flag), rx_buf + 9, sizeof(float)); // ?? position_x
        memcpy(&(cmd->pitch_flag), rx_buf + 13, sizeof(float));// ?? position_y
			
    }
    else
    {
			err_flg=1;
    }
}

void Tx_data_to_Nuc(void)
{
	// update data to struct
	
	//demo
	Tx_Cmd.pitch_yuntai=1.0;
	Tx_Cmd.yaw_yuntai=0.0;
	// upload&transmit
	HAL_UART_Transmit(&huart2,&(Tx_Cmd.header),sizeof(uint8_t),10);
	HAL_UART_Transmit(&huart2,(uint8_t*)(&(Tx_Cmd.yaw_yuntai)),2*sizeof(float),10);
}

void real_to_dipan()
{
	dipan_speedtarget[0]=-(int16_t)(real_speedtarget[0]*19*60/97/PI);
	dipan_speedtarget[1]=(int16_t)(real_speedtarget[1]*19*60/97/PI);
	dipan_speedtarget[2]=-(int16_t)(real_speedtarget[2]*19*60/97/PI);
	dipan_speedtarget[3]=(int16_t)(real_speedtarget[3]*19*60/97/PI);
}

void Motion_transverse(float linearspeed,float anglespeed)
{
	real_speedtarget[0] =  linearspeed + 1.414*anglespeed*dipan_radius;
	real_speedtarget[1] =  linearspeed - 1.414*anglespeed*dipan_radius;
	real_speedtarget[2] =  linearspeed + 1.414*anglespeed*dipan_radius;
	real_speedtarget[3] =  linearspeed - 1.414*anglespeed*dipan_radius;
	real_to_dipan();
}

void Nuc_ctrl()
{
	if (err_flg==1)
	{
		Motion_transverse(0,0);
		real_to_dipan();
	}
	else if(err_flg==0)
	{
		if(Auto_Cmd.yaw_flag==1)
		{
			Motion_transverse(0,-0.1);
			real_to_dipan();
		}
		else
		{
			Motion_transverse(Auto_Cmd.linear_speed,-Auto_Cmd.angle_error*2);
			real_to_dipan();
		}
		
	}
}

void Nuc_data_limit()
{
	if(Auto_Cmd.angle_error>=PI)
		Auto_Cmd.angle_error=PI;
	else if (Auto_Cmd.angle_error<=-PI)
	Auto_Cmd.angle_error=-PI;
	
	if(Auto_Cmd.linear_speed>=400)
		Auto_Cmd.linear_speed=400;
	else if (Auto_Cmd.linear_speed<=-400)
	Auto_Cmd.linear_speed=-400;
}	