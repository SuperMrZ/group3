#ifndef _CAN_USER_H
#define _CAN_USER_H


#define ECD_PER_ROTATION 8192  // 转子一圈对应的编码值
#define RATIO 36               // 减速比

#define FULL_COUNT 36        // 完整圈数
#define HALF_ECD_RANGE 4096  // 半个编码器的范围
#define ECD_RANGE 8192       // 整个编码器的范围
#define MOTOR_ECD_TO_ANGLE  ( 2*3.141592653589793 / ECD_RANGE ) // 弧度制


 typedef struct 
{
	uint16_t motor_id;
	uint16_t angle;
  int16_t speed;
	int16_t last_angle;
    
}motor_recieve;

extern motor_recieve motor_recieve_dipan3508[4];
extern motor_recieve motor_recieve_yuntai6020[2];
extern motor_recieve motor_recieve_bodan2006;



void CAN_cmd_current_3508motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_speed_3508motor(int16_t target[4], motor_recieve motor_recieve_info[4]);

void CAN_cmd_current_6020motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_speed_6020motor(int16_t target[2], motor_recieve motor_recieve_info[2]);
void CAN_cmd_angle_6020motor(int16_t target[2], motor_recieve motor_recieve_info[2]);

void CAN_cmd_speed_2006motor(int16_t target, motor_recieve motor_recieve_info);
void CAN_cmd_angle_2006motor(int16_t target, motor_recieve motor_recieve_info);

void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

void calculateRotorRotation(double outputAngleIncrement, motor_recieve motor_recieve_info, int16_t *rotorTurns, int16_t *remainingEcd);
void updateMotorAngle(motor_recieve motor_recieve_info);




#endif
