# ifndef _PID_H
#define  _PID_H


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


extern PID pid_dipan3508[4];
extern PID pid_yuntai6020[2];
extern PID pid_yuntai6020_angle[2];
extern PID pid_yuntai3508[3];
extern PID pid_yuntai3508_angle[3];
extern PID pid_shengmingqiu2006[1];

int16_t pid_output(PID *pid, int16_t feedback, int16_t target) ;



#endif
