//
// Created by 20125 on 2024/7/9.
//

#ifndef RM_A_TEST_C620_CAN_H
#define RM_A_TEST_C620_CAN_H
#include "stm32f4xx_hal.h"

void My_can_start(CAN_HandleTypeDef* hcan);
void My_can_flt_init(CAN_HandleTypeDef *hcan);
void CAN_Set_c620_current(CAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
int16_t C620_GetSpeed(CAN_HandleTypeDef *hcan,uint16_t ID);
void Can12_Start(void);


#endif //RM_A_TEST_C620_CAN_H
