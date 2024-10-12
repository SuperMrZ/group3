#ifndef MahonyAHRS_h
#define MahonyAHRS_h
#include "bmi088.h"
#include "qmc5883l.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//----------------------------------------------------------------------------------------------------
// Variable declaration

//×ËÌ¬½Ç½á¹¹Ìå
typedef struct zitai_angle {
    float roll;     //ºá¹ö½Ç
    float pitch;    //¸©Ñö½Ç
    float yaw;      //º½Ïò½Ç
} zitai_angle;

extern volatile float twoKp;			// 2 * proportional gain (Kp)
extern volatile float twoKi;			// 2 * integral gain (Ki)
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame

//---------------------------------------------------------------------------------------------------
// Function declarations


void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float * mx, float * my, float * mz);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro,float * x, float * y, float * z);
//void Read_Sensors(acc_raw_data_t *accel, gyro_raw_data_t *gyro,ist8310_raw_data_t *ist8310_raw_data);
void Calculate_Angles(float *roll, float *pitch, float *yaw);
#endif
//=====================================================================================================
// End of file
//=====================================================================================================
