# ifndef _PID_H
#define  _PID_H


typedef struct
{
//p,i,d����ֵ,maxI�����޷���maxO����޷�
int16_t kp;
int16_t ki;
int16_t kd;
int16_t maxI;  //maxI�����޷�
int16_t maxO;  //maxO����޷�
int16_t target;
int16_t feedback;
} PID;

extern PID pid_dipan3508;
extern PID pid_yuntai6020;


int16_t pid_output(PID pid, int16_t feedback,int16_t target) ;



#endif
