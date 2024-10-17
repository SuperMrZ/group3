#include "nuc_control.h"
#include "usart.h"
#include <string.h>  // for memcpy
#include "can_user.h"
#include "auto_control.h"
#include "crc.h"

#define MANU 1
#define AUTO 0
#define RxHeader 0xA5
#define TxHeader 0x5A

#define PI 3.1415926f
#define SlowRatio_3508  19.0f
#define SlowRatio_2006  36.0f
#define Radius 48.5f    //unit mm
#define dipan_radius 233 //unit mm
// ״̬�ӿ�
uint8_t mode_config=AUTO;	// �Զ����������궨��
uint8_t err_flg=0;
extern uint8_t Work_State;

// ������ƽӿ�
extern int16_t dipan_speedtarget[4];
extern int16_t yuntai_angletarget[2];
extern int16_t m6020_target[2];
extern int16_t output6020;
extern int16_t m2006_target_speed;

float real_speedtarget[4];



// ����ӿ�

// Tx of auto
Tx_Data_toNuc Tx_Cmd;

// �ֶ����ӿڣ���ʱδ�Խ�ʹ��
// Rx of Manu
Nuc_manu_cmd Manu_Cmd;
uint8_t Manu_Rx_buf[5*sizeof(float)+1];

// �Զ����ӿڣ��Ѿ�����ʹ��
// Rx of auto
Nuc_auto_cmd Auto_Cmd;
//uint8_t Auto_Rx_buf[4*sizeof(float)+1];
uint8_t Auto_Rx_buf[36];

uint8_t Tx_buffer[13];


/*
*��ʼ�����ڽ���ָ�����Ȼص��жϽ���ң��/���ڽ��տ����ж�����nuc����
*
*
*/
void Nuc_Tele_Init() // ����ʹ�ô��ڿ����ж�
{
	if(mode_config==MANU)
	{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Manu_Rx_buf,21);
	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //��ֹ���յ�һ���ֹͣ������һ��һ��Ҫ����д
	}
	else //auto
	{
		Tx_Cmd.header=TxHeader;
		Tx_buffer[0]=TxHeader;
		
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  // ʹ�ܴ��ڿ����ж�
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)Auto_Rx_buf, 36);     //����DMA���䣬������1�����ݰ��˵�buff�У�ÿ�����36���ֽڣ�ȡ���ڻ��������ô�С       
	}
}

//void Nuc_Tele_CRC_Init();

//// ����Ϊʹ�ý��̶ܹ����ֽڵ�DMA�����жϰ汾���������ȶ��Է���		

//void Nuc_Tele_Init() // ����ʹ�ô��ڿ����ж�
//{
//	if(mode_config==MANU)
//	{
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Manu_Rx_buf,21);
//	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //��ֹ���յ�һ���ֹͣ������һ��һ��Ҫ����д
//	}
//	else //auto
//	{
//		Tx_Cmd.header=TxHeader;      
////	
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart2,&Auto_Rx_buf[0],17);
//	__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );  //��ֹ���յ�һ���ֹͣ������һ��һ��Ҫ����д
//	}
//}

/*
*�ֶ����������
* rx_buf ���ջ�������
*	cmd �ֶ���ң�����ݽṹ��ָ��
*/
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

/*
* �Զ����������
* rx_buf ���ջ�������
*	cmd Nuc ң�����ݽṹ��ָ��
* data_length �����жϼ���Ľ������ݰ���С/λ
*/
void Nuc_Auto_Decode(uint8_t* rx_buf, Nuc_auto_cmd* cmd,uint8_t data_length)
{
	// ʹ�� CRC 
   if (Verify_CRC16_Check_Sum(Auto_Rx_buf, data_length)) 
		{
      // CRCУ��ͨ������������
      if (rx_buf[0] == 0xA5&&(data_length-2)==17)
			{
					cmd->header = rx_buf[0]; 
					memcpy(&(cmd->angle_speed), rx_buf + 1, sizeof(float));      // ?? pitch
					memcpy(&(cmd->linear_speed), rx_buf + 5, sizeof(float));        // ?? yaw
					memcpy(&(cmd->yaw_flag), rx_buf + 9, sizeof(float)); // ?? position_x
					memcpy(&(cmd->pitch_flag), rx_buf + 13, sizeof(float));// ?? position_y
					err_flg=0;
					// �������޷�
					Nuc_data_limit(200,0.4);
			}
			else	// ����֡���ˣ��ô���
			{	
				err_flg=1;
			}
    }else // CRC���ˣ�Ҳ�ô���
		{
      err_flg=1;      
    }
			 
		// ��ʹ��CRC
//	 if (rx_buf[0] == 0xA5&&data_length==17)
//    {
//        cmd->header = rx_buf[0]; 
//        memcpy(&(cmd->angle_error), rx_buf + 1, sizeof(float));      // ?? pitch
//        memcpy(&(cmd->linear_speed), rx_buf + 5, sizeof(float));        // ?? yaw
//        memcpy(&(cmd->yaw_flag), rx_buf + 9, sizeof(float)); // ?? position_x
//        memcpy(&(cmd->pitch_flag), rx_buf + 13, sizeof(float));// ?? position_y
//				err_flg=0;
//				// �������޷�
//				Nuc_data_limit(200,0.4);
//    }
//    else
//    {
//			err_flg=1;
//			
//    }
}

/*
*	����2�������ݵ�Nuc
* 
*	
*/
void Tx_data_to_Nuc(void)
{
	// update data to struct
	
	//��������
	Tx_Cmd.yaw_yuntai=0.0;
	Tx_Cmd.homeornot=GoHome_judge();
	Tx_Cmd.pitch_yuntai=code_to_hudu_6020(&(motor_recieve_yuntai6020[1].angle));
	//ת�Ƶ����ͻ������飬����
	memcpy(&(Tx_Cmd.yaw_yuntai),Tx_buffer + 1,sizeof(float));
	memcpy(&(Tx_Cmd.pitch_yuntai),Tx_buffer + 5,sizeof(float));
	memcpy(&(Tx_Cmd.homeornot),Tx_buffer + 9,sizeof(float));
	HAL_UART_Transmit(&huart2,Tx_buffer,sizeof(Tx_buffer),10);
//	�����İ汾
//	HAL_UART_Transmit(&huart2,&(Tx_Cmd.header),sizeof(uint8_t),10);
//	HAL_UART_Transmit(&huart2,(uint8_t*)(&(Tx_Cmd.yaw_yuntai)),3*sizeof(float),10);
	
	
}
/*
*	���̿������ݵ�λ����
*	��3508 ת��ת�ٻ��㵽������Ӧ���ٶȣ�mm/s,���ٶ�:rad/s
*	
*/
void real_to_dipan()
{
	dipan_speedtarget[0]=-(int16_t)(real_speedtarget[0]*19*60/97/PI);
	dipan_speedtarget[1]=(int16_t)(real_speedtarget[1]*19*60/97/PI);
	dipan_speedtarget[2]=-(int16_t)(real_speedtarget[2]*19*60/97/PI);
	dipan_speedtarget[3]=(int16_t)(real_speedtarget[3]*19*60/97/PI);
}
/*
* �����˶�ѧ����
* linearspeed Ŀ�����ٶ�mm/s
*	anglespeed Ŀ����ٶ�rad/s
*/
void Motion_transverse(float linearspeed,float anglespeed)
{
	real_speedtarget[0] =  linearspeed + 1.414*anglespeed*dipan_radius;
	real_speedtarget[1] =  linearspeed - 1.414*anglespeed*dipan_radius;
	real_speedtarget[2] =  linearspeed + 1.414*anglespeed*dipan_radius;
	real_speedtarget[3] =  linearspeed - 1.414*anglespeed*dipan_radius;
	real_to_dipan();
}

/*
* �˶�Ŀ��ֵ���㲢���ã���ϸ����߼�������
* 
*	
*/
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
				Motion_transverse(Auto_Cmd.linear_speed,-Auto_Cmd.angle_speed*1.0);
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
				Motion_transverse(Auto_Cmd.linear_speed,-Auto_Cmd.angle_speed*1.0);
				real_to_dipan();
				m2006_target_speed=0;
			}
			
		}
	}
}
/*����cmd����ָ�����������
*���������ٶ����ޣ����ٶ�����
* ��λ��mm/s  rad/s
*/
void Nuc_data_limit(float linear_limit,float angle_limit)
{
	if(Auto_Cmd.angle_speed>=angle_limit)
		Auto_Cmd.angle_speed=angle_limit;
	else if (Auto_Cmd.angle_speed<=-angle_limit)
	Auto_Cmd.angle_speed=-angle_limit;
	
	if(Auto_Cmd.linear_speed>=linear_limit)
		Auto_Cmd.linear_speed=linear_limit;
	else if (Auto_Cmd.linear_speed<=-linear_limit)
	Auto_Cmd.linear_speed=-linear_limit;
	
}	

// ��̨����תΪ������λΪ���ȣ���̨���Χ��Ӧ�Ƕ�ֵΪ4.21��-55.53��
/* ������������ݵ��ԵؽǶȣ����ڷ��͸��Ӿ�
*
* encoder_num ����������ָ��
*/
float code_to_hudu_6020(uint16_t* encoder_num)
{
	float temp = (-59.7/720.0*(*encoder_num-6630.0)+4.21)*PI/180.0;
	while(temp>PI)
		temp-=PI;
	while(temp<-PI)
		temp+=PI;
	return temp;
}

