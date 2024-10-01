#include "stm32f4xx.h"                  // Device header

typedef struct
{
//p,i,d����ֵ,maxI�����޷���maxO����޷�
float kp;
float ki;
int16_t kd;
int16_t maxI;  //maxI�����޷�
int16_t maxO;  //maxO����޷�

} PID;




//��ͬ�����pid����
 PID pid_dipan3508={5,0.15,0,0x1000,0x5000};
extern PID pid_dipan3508;
  PID pid_yuntai6020={5,0.01,0,3000,5000};
extern PID pid_yuntai6020;
 
 PID pid_yuntai6020_angle={5,0.01,0,8000,8000};
extern PID pid_yuntai6020_angle;

/**
  * @brief  pid_output�˺����������һ��pid���
  * @param  kp,ki,kd,maxI,maxO,�ֱ�ָpid�㷨������targetΪ����ﵽ��Ŀ��ֵ��feedbackΪ��ǰĿ���ֵ������ֵ��
  * @note   ע��feedback��target�ı��������÷���Ҫ����
  * @retval ��
  */
  

int16_t iout = 0; 

int16_t pid_output(PID pid, int16_t feedback,int16_t target) 
{
	
	int16_t error_now = 0;
	int16_t error_last = 0;
	error_last = error_now;
	error_now = target - feedback;
	
	int16_t pout = pid.kp * error_now;
	
	       iout += pid.ki * error_now;
	if (iout > pid.maxI)
	{
		iout = pid.maxI;
	}
	
	int16_t dout = pid.kd * (error_now - error_last);

	int16_t output = pout + iout + dout;
	if (output > pid.maxO)
	{
		output = pid.maxO;
	}
		
	return output;
}




