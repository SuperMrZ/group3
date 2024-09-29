#ifndef _CAN_USER_H
#define _CAN_USER_H


 typedef struct 
{
	uint16_t motor_id;
	uint16_t angle;
    int16_t speed;
    
}motor_recieve;

extern motor_recieve  motor_recieve_dipan3508[4];;

void CAN_cmd_current_3508motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_speed_3508motor(int16_t target[4], motor_recieve motor_recieve_info[4]);
void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);




#endif
