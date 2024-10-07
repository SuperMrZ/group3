#include "ZiTai-jiesuan.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// 初始化四元数和积分反馈
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};  // 初始四元数
float integralFB[3] = {0.0f, 0.0f, 0.0f};  // 积分反馈

// 更新四元数的函数
void Update_Quaternion(acc_raw_data_t *accel, gyro_raw_data_t *gyro) {
    float norm;        // 用于计算归一化值
    float vx, vy, vz;  // 存储当前重力方向
    float ex, ey, ez;  // 存储姿态误差
    float x=accel->x;
	float y=accel->y;
	float z=accel->z;
	
    // 计算加速度计数据的归一化量
    norm = sqrt(x * x + y * y + z * z);
    if (norm > 0) {
        x /= norm;  // 归一化加速度计的x轴数据
        y /= norm;  // 归一化加速度计的y轴数据
        z /= norm;  // 归一化加速度计的z轴数据
    }

    // 从四元数计算当前重力方向
    float q0 = q[0];  
    float q1 = q[1];
    float q2 = q[2];
    float q3 = q[3];

    // 计算重力方向向量（重力误差）
    vx = 2.0f * (q1 * q3 - q0 * q2);  
    vy = 2.0f * (q0 * q1 + q2 * q3);  
//  vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;  //不同
	vz = q0 * q0 - 0.5f + q3 * q3;

    // 计算姿态误差
    ex = (y * vz - z * vy);  
    ey = (z * vx - x * vz);  
    ez = (x * vy - y * vx);  

    // 积分误差反馈
	// 不同
    integralFB[0] += ex * DT; 
    integralFB[1] += ey * DT;  
    integralFB[2] += ez * DT;  

    // 修正陀螺仪的角速度
//    gyro->roll += Kp * ex + Ki * integralFB[0];  
//    gyro->pitch += Kp * ey + Ki * integralFB[1];  
//    gyro->yaw += Kp * ez + Ki * integralFB[2];  

      gyro->roll += Kp * ex;  
      gyro->pitch += Kp * ey;  
      gyro->yaw += Kp * ez;  


    // 更新四元数
    q0 += (-q1 * gyro->roll - q2 * gyro->pitch - q3 * gyro->yaw) * DT;  //不同
    q1 += (q0 * gyro->roll + q2 * gyro->yaw - q3 * gyro->pitch) * DT;
    q2 += (q0 * gyro->pitch - q1 * gyro->yaw + q3 * gyro->roll) * DT;
    q3 += (q0 * gyro->yaw + q1 * gyro->pitch - q2 * gyro->roll) * DT;

    // 归一化四元数
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q[0] = q0 / norm;  
    q[1] = q1 / norm;
    q[2] = q2 / norm;
    q[3] = q3 / norm;
}

// 从四元数计算姿态角的函数
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

// 读取传感器数据的函数
//void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro) {
//    // 假设有函数可以读取加速度计和陀螺仪的数据
//     ReadAccData(accel); 
//	 ReadGyroData(gyro);
//    // 更新四元数
//    Update_Quaternion(accel, gyro);
//}
