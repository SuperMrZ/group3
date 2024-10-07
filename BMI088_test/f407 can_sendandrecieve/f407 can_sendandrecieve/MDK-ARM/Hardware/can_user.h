#ifndef _CAN_USER_H
#define _CAN_USER_H



void CAN_cmd_6020motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);




#endif
