# ifndef _PID_H
#define  _PID_H


typedef struct
{
//p,i,d参数值,maxI积分限幅，maxO输出限幅
int16_t kp;
int16_t ki;
int16_t kd;
int16_t maxI;  //maxI积分限幅
int16_t maxO;  //maxO输出限幅
int16_t target;
int16_t feedback;
} PID;

extern PID pid_dipan3508;
extern PID pid_yuntai6020;


int16_t pid_output(PID pid, int16_t feedback,int16_t target) ;



#endif
