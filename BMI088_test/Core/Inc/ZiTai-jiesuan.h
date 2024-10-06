#ifndef ATTITUDE_ESTIMATION_H
#define ATTITUDE_ESTIMATION_H

#include "stm32f4xx_hal.h"
#include "bmi088.h"
// 姿态解算相关的常量
#define Kp 0.5f  // 比例增益
#define Ki 0.1f  // 积分增益
#define DT 0.01  // 采样时间

//姿态角结构体
typedef struct zitai_angle {
    float roll;
    float pitch;
    float yaw;
} zitai_angle;

// 四元数
extern float q[4];  // 四元数
extern float integralFB[3];  // 积分反馈

void AttitudeEstimation_Init(void);
void Update_Quaternion(acc_raw_data_t *accel, gyro_raw_data_t *gyro);
void Calculate_Angles(float *roll, float *pitch, float *yaw);
void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro);

#endif // ATTITUDE_ESTIMATION_H
