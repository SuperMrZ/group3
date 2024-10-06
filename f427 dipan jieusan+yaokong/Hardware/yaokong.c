#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "yaokong.h" 


extern 	RC_Ctl_t RC_Ctl;

extern uint8_t sbus_rx_buffer[18];//声明遥控器接收缓存数组
uint8_t bodan_flag=0;
extern int16_t bodan_angletarget;



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
				 bodan_angletarget+=4000;
				while(bodan_angletarget > 8191)
				{
				  bodan_angletarget -= 8191;
				}


//#define ECD_PER_ROTATION 8192  // 转子一圈对应的编码值
//#define RATIO 36               // 减速比

//void calculateRotorRotation(double outputAngleIncrement, int currentEcd, int *rotorTurns, int *remainingEcd) {
//    // 计算输出轴圈数
//    double outputTurns = outputAngleIncrement / (2.0 * 3.141592653589793); 
//    
//    // 转子所需圈数
//    *rotorTurns = (int)(outputTurns * RATIO); 
//    
//    // 转子剩余目标编码值
//    double remainingOutputAngle = outputAngleIncrement - ((int)(outputTurns) * 2.0 * 3.141592653589793);
//    *remainingEcd = (int)((remainingOutputAngle / (2.0 * 3.141592653589793 / ECD_PER_ROTATION)) + 0.5); 

//    int targetEcd = (currentEcd + *remainingEcd) % ECD_PER_ROTATION;

//    // 编码值确保在有效范围
//    if (targetEcd < 0) {
//        targetEcd += ECD_PER_ROTATION; 

//    *remainingEcd = targetEcd; 
//}

//int main() {
//    double outputAngleIncrement = 1.0; // 输出轴角度增量（弧度制）
//    int currentEcd = 1000; // 当前转子的编码编码位置
//    int rotorTurns = 0;
//    int remainingEcd = 0;

//    calculateRotorRotation(outputAngleIncrement, currentEcd, &rotorTurns, &remainingEcd);
//    
//    
//    return 0;
//}

			}
				RC_Ctl.rc.last_s2 = RC_Ctl.rc.s2;
	

				HAL_UARTEx_ReceiveToIdle_DMA(&huart1,sbus_rx_buffer,18);	
				__HAL_DMA_DISABLE_IT(huart1.hdmarx ,DMA_IT_HT );	
	}
