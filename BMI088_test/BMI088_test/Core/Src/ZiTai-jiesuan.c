#include "ZiTai-jiesuan.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// ��ʼ����Ԫ���ͻ��ַ���
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};  // ��ʼ��Ԫ��
float integralFB[3] = {0.0f, 0.0f, 0.0f};  // ���ַ���

// ������Ԫ���ĺ���
void Update_Quaternion(acc_raw_data_t *accel, gyro_raw_data_t *gyro) {
    float norm;        // ���ڼ����һ��ֵ
    float vx, vy, vz;  // �洢��ǰ��������
    float ex, ey, ez;  // �洢��̬���
    float x=accel->x;
	float y=accel->y;
	float z=accel->z;
	
    // ������ٶȼ����ݵĹ�һ����
    norm = sqrt(x * x + y * y + z * z);
    if (norm > 0) {
        x /= norm;  // ��һ�����ٶȼƵ�x������
        y /= norm;  // ��һ�����ٶȼƵ�y������
        z /= norm;  // ��һ�����ٶȼƵ�z������
    }

    // ����Ԫ�����㵱ǰ��������
    float q0 = q[0];  
    float q1 = q[1];
    float q2 = q[2];
    float q3 = q[3];

    // ������������������������
    vx = 2.0f * (q1 * q3 - q0 * q2);  
    vy = 2.0f * (q0 * q1 + q2 * q3);  
//  vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;  //��ͬ
	vz = q0 * q0 - 0.5f + q3 * q3;

    // ������̬���
    ex = (y * vz - z * vy);  
    ey = (z * vx - x * vz);  
    ez = (x * vy - y * vx);  

    // ��������
	// ��ͬ
    integralFB[0] += ex * DT; 
    integralFB[1] += ey * DT;  
    integralFB[2] += ez * DT;  

    // ���������ǵĽ��ٶ�
//    gyro->roll += Kp * ex + Ki * integralFB[0];  
//    gyro->pitch += Kp * ey + Ki * integralFB[1];  
//    gyro->yaw += Kp * ez + Ki * integralFB[2];  

      gyro->roll += Kp * ex;  
      gyro->pitch += Kp * ey;  
      gyro->yaw += Kp * ez;  


    // ������Ԫ��
    q0 += (-q1 * gyro->roll - q2 * gyro->pitch - q3 * gyro->yaw) * DT;  //��ͬ
    q1 += (q0 * gyro->roll + q2 * gyro->yaw - q3 * gyro->pitch) * DT;
    q2 += (q0 * gyro->pitch - q1 * gyro->yaw + q3 * gyro->roll) * DT;
    q3 += (q0 * gyro->yaw + q1 * gyro->pitch - q2 * gyro->roll) * DT;

    // ��һ����Ԫ��
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q[0] = q0 / norm;  
    q[1] = q1 / norm;
    q[2] = q2 / norm;
    q[3] = q3 / norm;
}

// ����Ԫ��������̬�ǵĺ���
//void Calculate_Angles(float *roll, float *pitch, float *yaw) {
//    *roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]),
//                    q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]) * 180.0 / M_PI;
//    *pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2])) * 180.0 / M_PI;
//    *yaw = atan2(2.0f * (q[3] * q[2] + q[0] * q[1]),
//                    q[0] * q[0] + q[3] * q[3] - q[2] * q[2] - q[1] * q[1]) * 180.0 / M_PI;
//}
//void Calculate_Angles(float *roll, float *pitch, float *yaw) {
//	
////    *roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]),
////                    2.0f*(q[0] * q[0] + q[1] * q[1]-1.0f)) * 180.0 / M_PI;
////    *pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2])) * 180.0 / M_PI;
////    *yaw = atan2(2.0f * (q[3] * q[0] + q[2] * q[1]),
////                    2.0f*(q[0] * q[0] + q[1] * q[1]-1.0f)) * 180.0 / M_PI;
//	
//	*yaw = atan2f(2.0f*(q[0]*q[3]+q[1]*q[2]), 2.0f*(q[0]*q[0]+q[1]*q[1])-1.0f); 
//    *pitch = asinf(-2.0f*(q[1]*q[3]-q[0]*q[2])); 
//    *roll = atan2f(2.0f*(q[0]*q[1]+q[2]*q[3]),2.0f*(q[0]*q[0]+q[3]*q[3])-1.0f); 
//}

// ��ȡ���������ݵĺ���
//void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro) {
//    // �����к������Զ�ȡ���ٶȼƺ������ǵ�����
//     ReadAccData(accel); 
//	 ReadGyroData(gyro);
//    // ������Ԫ��
//    Update_Quaternion(accel, gyro);
//}
