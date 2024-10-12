#include "MahonyAHRS.h"
#include <math.h>


//---------------------------------------------------------------------------------------------------
// Definitions

#define sampleFreq	1000.0f			// sample frequency in Hz
#define twoKpDef	(50.0f * 5.0f)	// 比例增益的两倍
#define twoKiDef	(5.0f * 1.0f)	// 积分增益的两倍

//---------------------------------------------------------------------------------------------------
// Variable definitions

volatile float twoKp = twoKpDef;											// 可变比例增益
volatile float twoKi = twoKiDef;											// 可变积分增益
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// 四元数，表示传感器坐标系相对于辅助坐标系的姿态
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// 积分误差项

//---------------------------------------------------------------------------------------------------


float invSqrt(float x);


//---------------------------------------------------------------------------------------------------
// AHRS算法更新（暂未使用）
void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float * mx, float * my, float * mz) {
	float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((*mx == 0.0f) && (*my == 0.0f) && (*mz == 0.0f)) {
		MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az);
		return;
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;     

		// Normalise magnetometer measurement
		recipNorm = invSqrt(*mx * *mx + *my * *my + *mz * *mz);
		*mx *= recipNorm;
		*my *= recipNorm;
		*mz *= recipNorm;   

        // Auxiliary variables to avoid repeated arithmetic
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;   

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (*mx * (0.5f - q2q2 - q3q3) + *my * (q1q2 - q0q3) + *mz * (q1q3 + q0q2));
        hy = 2.0f * (*mx * (q1q2 + q0q3) + *my * (0.5f - q1q1 - q3q3) + *mz * (q2q3 - q0q1));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (*mx * (q1q3 - q0q2) + *my * (q2q3 + q0q1) + *mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);  
	
		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex = (ay * halfvz - az * halfvy) + (*my * halfwz - *mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (*mz * halfwx - *mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (*mx * halfwy - *my * halfwx);

		// Compute and apply integral feedback if enabled
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// apply integral feedback
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// prevent integral windup
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------

//IMU算法（使用）
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// 如果加速度计数据有效，则进行反馈计算
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// 归一化加速度计数据
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// 估计重力方向
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
	
		// 计算误差为估计和实际重力方向的差
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// 计算并应用积分反馈
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// 积分误差按Ki缩放
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// 应用积分反馈
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// 防止积分饱和
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// 应用比例反馈
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// 积分四元数的变化率
	gx *= (0.5f * (1.0f / sampleFreq));	 // 预先乘以公因子
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);// 更新四元数
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// 归一化四元数
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------

//快速逆平方根算法
//参考：http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

//读取bmi088数据函数
void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro,float * x, float * y, float * z) {

     ReadAccData(accel); 
	 ReadGyroData(gyro); 
	 QMC5883L_ReadData(x, y, z);
	
    // 更新四元数
    // MahonyAHRSupdateIMU(gyro->roll,gyro->pitch,gyro->yaw,accel->x,accel->y,accel->z);
	 MahonyAHRSupdate(gyro->roll,gyro->pitch,gyro->yaw,accel->x,accel->y,accel->z, x, y, z);
}

//姿态角计算
void Calculate_Angles(float *roll, float *pitch, float *yaw) {
	

	*yaw = (atan2f(2.0f*(q0*q3+q1*q2), 2.0f*(q0*q0+q1*q1)-1.0f))*(180.0f/ M_PI); 
    *pitch = (asinf(-2.0f*(q1*q3-q0*q2)))*(180.0f/ M_PI); 
    *roll = (atan2f(2.0f*(q0*q1+q2*q3),2.0f*(q0*q0+q3*q3)-1.0f))*(180.0f/ M_PI); 
}
//====================================================================================================
// END OF CODE
//====================================================================================================
