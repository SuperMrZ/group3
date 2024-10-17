#include "nuc_control.h"
#include "usart.h"
#include <string.h>  // for memcpy
#include "can_user.h"
#include "auto_control.h"

#define MANU 1
#define AUTO 0
#define RxHeader 0xA5
#define TxHeader 0x5A

#define PI 3.1415926f
#define SlowRatio_3508  19.0f
#define SlowRatio_2006  36.0f
#define Radius 48.5f    //unit mm
#define dipan_radius 233 //unit mm

extern int16_t dipan_speedtarget[4];
extern int16_t yuntai_angletarget[2];
extern int16_t m6020_target[2];
extern int16_t output6020;
extern int16_t m2006_target_speed;

float real_speedtarget[4];

uint8_t err_flg=0;
uint8_t mode_config=AUTO;	// 自动车解码程序
extern uint8_t Work_State;


// Tx of auto
Tx_Data_toNuc Tx_Cmd;

// if manu copy this to main.c
// Rx of Manu
Nuc_manu_cmd Manu_Cmd;
uint8_t Manu_Rx_buf[5*sizeof(float)+1];

// if auto copy this to main.c
// Rx of auto
Nuc_auto_cmd Auto_Cmd;
//uint8_t Auto_Rx_buf[4*sizeof(float)+1];
uint8_t Auto_Rx_buf[36];



void Nuc_Tele_Init() // 现在使用串口空闲中断
{
	if(mode_config==MANU)
	{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Manu_Rx_buf,21);
	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //防止接收到一半就停止，跟上一句一定要配套写
	}
	else //auto
	{
		Tx_Cmd.header=TxHeader;
		
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  // 使能串口空闲中断
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)Auto_Rx_buf, 36);     //设置DMA传输，将串口1的数据搬运到buff中，每次最多36个字节，取决于缓存区设置大小       
	}
}

void Nuc_Tele_CRC_Init();

//// 以下为使用接受固定长字节的DMA接受中断版本，现已因稳定性废弃		

//void Nuc_Tele_Init() // 现在使用串口空闲中断
//{
//	if(mode_config==MANU)
//	{
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Manu_Rx_buf,21);
//	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //防止接收到一半就停止，跟上一句一定要配套写
//	}
//	else //auto
//	{
//		Tx_Cmd.header=TxHeader;      
////	
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart2,&Auto_Rx_buf[0],17);
//	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //防止接收到一半就停止，跟上一句一定要配套写
//	}
//}


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


void Nuc_Auto_Decode(uint8_t* rx_buf, Nuc_auto_cmd* cmd,uint8_t data_length)
{
	 if (rx_buf[0] == 0xA5&&data_length==17)
    {
        cmd->header = rx_buf[0]; 
        memcpy(&(cmd->angle_error), rx_buf + 1, sizeof(float));      // ?? pitch
        memcpy(&(cmd->linear_speed), rx_buf + 5, sizeof(float));        // ?? yaw
        memcpy(&(cmd->yaw_flag), rx_buf + 9, sizeof(float)); // ?? position_x
        memcpy(&(cmd->pitch_flag), rx_buf + 13, sizeof(float));// ?? position_y
				err_flg=0;
				// 添加输出限幅
				Nuc_data_limit(200,0.4);
    }
    else
    {
			err_flg=1;
			
    }
}

void Tx_data_to_Nuc(void)
{
	// update data to struct
	
	//refresh / demo
	Tx_Cmd.yaw_yuntai=0.0;
	Tx_Cmd.homeornot=GoHome_judge();
	Tx_Cmd.pitch_yuntai=code_to_hudu_6020(&(motor_recieve_yuntai6020[1].angle));
	// upload&transmit
	HAL_UART_Transmit(&huart2,&(Tx_Cmd.header),sizeof(uint8_t),10);
	HAL_UART_Transmit(&huart2,(uint8_t*)(&(Tx_Cmd.yaw_yuntai)),3*sizeof(float),10);
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
		m2006_target_speed=0;
	}
	else if(err_flg==0)
	{
		if(Work_State==GoWork)
		{
			if(Auto_Cmd.yaw_flag==1)
			{
				Motion_transverse(0,0.2);
				real_to_dipan();
				m6020_target[1]= output6020;
				m2006_target_speed=10;
			}
			else
			{
				Motion_transverse(Auto_Cmd.linear_speed,-Auto_Cmd.angle_error*1.0);
				real_to_dipan();
				m2006_target_speed=0;
			}
		}else if(Work_State==DEAD)
		{
			Motion_transverse(0,0);
			real_to_dipan();
			m6020_target[1]= output6020;
			m2006_target_speed=0;
		}else if(Work_State==GoHome)
		{
			if(Auto_Cmd.yaw_flag==1)
			{
				Motion_transverse(0,0.2);
				real_to_dipan();
				m6020_target[1]= output6020;
				m2006_target_speed=10;
			}
			else
			{
				Motion_transverse(Auto_Cmd.linear_speed,-Auto_Cmd.angle_error*1.0);
				real_to_dipan();
				m2006_target_speed=0;
			}
			
		}
	}
}
/*
*参数：线速度上限，角速度上限
* 单位：mm/s  rad/s
*/
void Nuc_data_limit(float linear_limit,float angle_limit)
{
	if(Auto_Cmd.angle_error>=angle_limit)
		Auto_Cmd.angle_error=angle_limit;
	else if (Auto_Cmd.angle_error<=-angle_limit)
	Auto_Cmd.angle_error=-angle_limit;
	
	if(Auto_Cmd.linear_speed>=linear_limit)
		Auto_Cmd.linear_speed=linear_limit;
	else if (Auto_Cmd.linear_speed<=-linear_limit)
	Auto_Cmd.linear_speed=-linear_limit;
	
}	

// 云台向上转为正方向单位为弧度，云台活动范围对应角度值为4.21到-55.53°
float code_to_hudu_6020(uint16_t* encoder_num)
{
	float temp = (-59.7/720.0*(*encoder_num-6630.0)+4.21)*PI/180.0;
	while(temp>PI)
		temp-=PI;
	while(temp<-PI)
		temp+=PI;
	return temp;
}