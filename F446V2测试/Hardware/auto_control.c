#include "auto_control.h"
#include "math.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "nuc_control.h"

#define PI 3.14159265
#define TIMHz 1000.0  // ���ò���������

extern int16_t m6020_target[2];
 int16_t i;

float sinoutput;
// ʹ�ö�ʱ��2��10ms����һ��,���ڵ�λ��
void Generate_sine(float period,float up,float down,int16_t *output)
{
	period/=9.6;
	int16_t amplitude = (up-down)/2;
	*output = (int16_t)(amplitude*sin(2.0 * PI * i / TIMHz/period)+((up+down)/2.0));
	i++;
	i %=(int16_t)(TIMHz*period);
}

//void pitch_period_motion(int16_t &m6020[2])
//{
//	
//}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim == (&htim2))
    {
			Generate_sine(2,7200,6700,&m6020_target[1]);

//			Tx_data_to_Nuc();
		}
    
}