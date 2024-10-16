#ifndef _CAN_USER_H
#define _CAN_USER_H


 typedef struct 
{
	uint16_t motor_id;
	uint16_t angle;
    int16_t speed;
	int16_t last_angle;
    
}motor_recieve;

extern motor_recieve  motor_recieve_dipan3508[4];
extern motor_recieve motor_recieve_yuntai6020[2];


void CAN_cmd_current_3508motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_speed_3508motor(int16_t target[4], motor_recieve motor_recieve_info[4]);

void CAN_cmd_current_6020motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_speed_6020motor(int16_t target[2], motor_recieve motor_recieve_info[2]);
void CAN_cmd_angle_6020motor(int16_t target[2], motor_recieve motor_recieve_info[2]);

void CAN_cmd_current_yuntaimotor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_speed_yuntaimotor(int16_t target[3], motor_recieve motor_recieve_info[3]);
void CAN_cmd_angle_yuntaimotor(int16_t target[3], motor_recieve motor_recieve_info[3]);


void can_filter_init(void);
void can_filter2_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);




#endif
