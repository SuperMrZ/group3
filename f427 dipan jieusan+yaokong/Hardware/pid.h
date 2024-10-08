# ifndef _PID_H
#define  _PID_H


typedef struct
{
//p,i,d����ֵ,maxI�����޷���maxO����޷�
float kp;
float ki;
int16_t kd;
int16_t maxI;  //maxI�����޷�
int16_t maxO;  //maxO����޷�
int16_t error_now;
int16_t error_last;

} PID;


extern PID pid_dipan3508[4];
extern PID pid_yuntai6020[2];
extern PID pid_yuntai6020_angle[2];
extern PID pid_yuntai3508[3];
extern PID pid_yuntai3508_angle[3];

int16_t pid_output(PID *pid, int16_t feedback, int16_t target) ;



#endif
