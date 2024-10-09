#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "yaokong.h" 
#include "tim.h"

extern 	RC_Ctl_t RC_Ctl;

extern uint8_t sbus_rx_buffer[18];//声明遥控器接收缓存数组

extern  uint16_t* bodan_target_angle;



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	
	
				yaokongjishi=10;
				RC_Ctl.rc.s2_last = RC_Ctl.rc.s2;
                RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;          
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;       
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;          
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;           
				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                           
				RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003);
	
	
				if(RC_Ctl.rc.s2_last == 3 && RC_Ctl.rc.s2 == 2)
				{
					*bodan_target_angle +=8192*6;
				}
	
				
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,sbus_rx_buffer,18);	
				__HAL_DMA_DISABLE_IT(huart1.hdmarx ,DMA_IT_HT );

				
	
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
