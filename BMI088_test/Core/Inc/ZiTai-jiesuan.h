#ifndef ATTITUDE_ESTIMATION_H
#define ATTITUDE_ESTIMATION_H

#include "stm32f4xx_hal.h"
#include "bmi088.h"
// ��̬������صĳ���
#define Kp 0.5f  // ��������
#define Ki 0.1f  // ��������
#define DT 0.01  // ����ʱ��

//��̬�ǽṹ��
typedef struct zitai_angle {
    float roll;
    float pitch;
    float yaw;
} zitai_angle;

// ��Ԫ��
extern float q[4];  // ��Ԫ��
extern float integralFB[3];  // ���ַ���

void AttitudeEstimation_Init(void);
void Update_Quaternion(acc_raw_data_t *accel, gyro_raw_data_t *gyro);
void Calculate_Angles(float *roll, float *pitch, float *yaw);
void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro);

#endif // ATTITUDE_ESTIMATION_H
