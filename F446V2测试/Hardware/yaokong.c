#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "yaokong.h" 
#include "tim.h"
#include "can_user.h"
#include "nuc_control.h"



extern 	RC_Ctl_t RC_Ctl;

extern uint8_t sbus_rx_buffer[18];//ÉùÃ÷Ò£¿ØÆ÷½ÓÊÕ»º´æÊý×é

extern  uint16_t* bodan_target_angle;
extern motor_recieve motor_recieve_yuntai3508[3];
extern int16_t yuntai_speed_target[3];

extern Nuc_manu_cmd Manu_Cmd;
extern uint8_t Manu_Rx_buf[5*sizeof(float)+1];

extern Nuc_auto_cmd Auto_Cmd;
extern uint8_t Auto_Rx_buf[4*sizeof(float)+1];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	
	if(huart==&huart3)
	{
		yaokongjishi=10;
				RC_Ctl.rc.s2_last = RC_Ctl.rc.s2;
        RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;          
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;       
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;          
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;           
				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                           
				RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003);
				
				RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8);                    //!< Mouse X axis        
				RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8);                    //!< Mouse Y axis      
				RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8);                  //!< Mouse Z axis         
				RC_Ctl.mouse.press_l = sbus_rx_buffer[12];                                        //!< Mouse Left Is Press      
				RC_Ctl.mouse.press_r = sbus_rx_buffer[13];                                        //!< Mouse Right Is Press 
				RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8);   			//!< KeyBoard value
	
	
				if(RC_Ctl.rc.s1 == 2)  //×ó²¦¸ËÆô¶¯Ä¦²ÁÂÖ
				{
					yuntai_speed_target[1] = 500;
					yuntai_speed_target[2] = 500;
					CAN_cmd_speed_yuntaimotor(yuntai_speed_target,motor_recieve_yuntai3508);
				}
				
				if(RC_Ctl.rc.s2_last == 3 && RC_Ctl.rc.s2 == 2) //ÓÒ²¦¸ËÆô¶¯×°µ¯
				{
					*bodan_target_angle +=8192*6;
				}
				
					HAL_UARTEx_ReceiveToIdle_DMA(&huart3,sbus_rx_buffer,18);	
				__HAL_DMA_DISABLE_IT(huart3.hdmarx ,DMA_IT_HT );

			}else if(huart==&huart2)
			{
				// Manu
//				HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Manu_Rx_buf,21);
//				Nuc_Manu_Decode(Manu_Rx_buf,&Manu_Cmd);
//			__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );
				// Auto
				HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Auto_Rx_buf,17);
				Nuc_Auto_Decode(Auto_Rx_buf,&Auto_Cmd);
			__HAL_DMA_DISABLE_IT(huart2.hdmarx ,DMA_IT_HT );
			}
}

	
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim == (&htim2))
    {
        yaokongjishi--;
		if(yaokongjishi<=0)
		{
			RC_Ctl.rc.ch0=1024;
			RC_Ctl.rc.ch1=1024;
			RC_Ctl.rc.ch2=1024;
			RC_Ctl.rc.ch3=1024;
			RC_Ctl.rc.s1=3;
			RC_Ctl.rc.s2=3;

		}
    }
}
