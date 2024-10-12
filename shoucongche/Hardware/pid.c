#include "stm32f4xx.h"                  // Device header

typedef struct
{
//p,i,d参数值,maxI积分限幅，maxO输出限幅
float kp;
float ki;
float kd;
int16_t maxI;  //maxI积分限幅
int16_t maxO;  //maxO输出限幅
int16_t error_now;
int16_t error_last;
int16_t iout;
int16_t output;

} PID;




//不同电机的pid参数
PID pid_dipan3508[4] = {
    {5, 0.15, 0, 0x1000, 0x5000, 0, 0,0,0},
    {5, 0.15, 0, 0x1000, 0x5000, 0, 0,0,0},
    {5, 0.15, 0, 0x1000, 0x5000, 0, 0,0,0},
    {5, 0.15, 0, 0x1000, 0x5000, 0, 0,0,0}
};
extern PID pid_dipan3508[4];

 PID pid_yuntai6020[2] = {
    {5, 0.01, 0, 3000, 5000, 0, 0,0,0},
    {5, 0.01, 0, 3000, 5000, 0, 0,0,0}
};
extern PID pid_yuntai6020[2];
 
PID pid_yuntai6020_angle[2] = {
    {5, 0.01, 0, 8000, 8000, 0, 0,0,0},
    {5, 0.05, 0, 10000, 16000, 0, 0,0,0}
};

extern PID pid_yuntai6020_angle[2];

PID pid_yuntai3508[3] = {
    {5, 0.01,0, 0x1000, 0x5000, 0, 0,0 ,0},
    {5, 0.1, 0, 0x1000, 0x5000, 0, 0,0,0},
    {5, 0.1, 0, 0x1000, 0x5000, 0, 0,0,0}
};
extern PID pid_yuntai3508[3];

PID pid_yuntai3508_angle[3] = {
    {0.3,0.01,0, 0x95, 0x5000, 0, 0,0  ,0},
    {0.5, 0.01, 0, 0x1000, 0x5000, 0, 0,0,0},
    {5, 0.01, 0, 0x1000, 0x5000, 0, 0,0,0}
};
extern PID pid_yuntai3508_angle[3];
/**
  * @brief  pid_output此函数用于输出一个pid输出
  * @param  kp,ki,kd,maxI,maxO,分别指pid算法参数，target为你想达到的目标值，feedback为当前目标的值（反馈值）
  * @note   注意feedback与target的变量类型置否需要更改
  * @retval 无
  */
  


int16_t pid_output(PID *pid, int16_t feedback, int16_t target) 
{
    // 更新误差
    pid->error_last = pid->error_now;
    pid->error_now = target - feedback;

    int16_t a = pid->error_now;
    int16_t b = pid->error_last;

    // 计算P部分
    int16_t pout = pid->kp * pid->error_now;

    // 计算并限制I部分
    pid->iout += pid->ki * pid->error_now;
    int16_t c = pid->iout;
    if (pid->iout > pid->maxI) {
        pid->iout = pid->maxI;
    }
	else if (pid->iout < - pid->maxI) {
        pid->iout =  - pid->maxI;
    }

    // 计算D部分
    int16_t dout = pid->kd * (pid->error_now - pid->error_last);

    // 计算输出并限制
    int16_t output = pout - dout + pid->iout;
    if (output > pid->maxO) {
        output = pid->maxO;
		pid->output = output;
    }

    return output;
}





