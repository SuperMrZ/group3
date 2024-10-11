#ifndef ATTITUDE_ESTIMATION_H
#define ATTITUDE_ESTIMATION_H

#include "stm32f4xx_hal.h"
#include "bmi088.h"
// ��̬������صĳ���
#define Kp 2.0f  // ��������
#define Ki 0.0f  // ��������
#define DT 0.01  // ����ʱ��

// ��Ԫ��
extern float q[4];  // ��Ԫ��
extern float integralFB[3];  // ���ַ���

void AttitudeEstimation_Init(void);
void Update_Quaternion(acc_raw_data_t *accel, gyro_raw_data_t *gyro);
void Calculate_Angles(float *roll, float *pitch, float *yaw);
void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro);

#endif // ATTITUDE_ESTIMATION_H
