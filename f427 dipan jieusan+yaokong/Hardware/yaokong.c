#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "yaokong.h" 
#include "can_user.h"


extern 	RC_Ctl_t RC_Ctl;

extern uint8_t sbus_rx_buffer[18];//����ң�������ջ�������
extern int16_t bodan_angletarget;
extern int16_t rotorTurns; //2006ת��Ŀ��Ȧ�� 
extern int16_t remainingEcd; // 2006ת�ӽǶ�


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
        RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;          
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;       
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;          
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;           
				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                           
				RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003);
	
				if (RC_Ctl.rc.last_s2 == 3 && RC_Ctl.rc.s2 == 2)
			{
				
				calculateRotorRotation(6.00,motor_recieve_dipan3508[0],&rotorTurns,&remainingEcd);
				
			}
				RC_Ctl.rc.last_s2 = RC_Ctl.rc.s2;
				

				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,sbus_rx_buffer,18);	
				__HAL_DMA_DISABLE_IT(huart1.hdmarx ,DMA_IT_HT );	
	}
