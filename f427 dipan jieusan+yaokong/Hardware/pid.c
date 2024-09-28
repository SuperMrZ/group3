#include "stm32f4xx.h"                  // Device header

typedef struct
{
//p,i,d����ֵ,maxI�����޷���maxO����޷�
int16_t kp;
float ki;
int16_t kd;
int16_t maxI;  //maxI�����޷�
int16_t maxO;  //maxO����޷�

} PID;




//��ͬ�����pid����
 PID pid_dipan3508={5,0.1,0,0x500,0x2000};
extern PID pid_dipan3508;


/**
  * @brief  pid_output�˺����������һ��pid���
  * @param  kp,ki,kd,maxI,maxO,�ֱ�ָpid�㷨������targetΪ����ﵽ��Ŀ��ֵ��feedbackΪ��ǰĿ���ֵ������ֵ��
  * @note   ע��feedback��target�ı��������÷���Ҫ����
  * @retval ��
  */
  



int16_t pid_output(PID pid, int16_t feedback,int16_t target) 
{
	
	int16_t error_now = 0;
	int16_t error_last = 0;
	error_last = error_now;
	error_now = target - feedback;
	
	int16_t pout = pid.kp * error_now;
	
	int16_t iout = 0;  
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




