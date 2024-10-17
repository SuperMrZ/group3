#include "auto_control.h"
#include "math.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "nuc_control.h"
#include "string.h"

#define PI 3.14159265
#define TIMHz 1000.0  // 设置采样点数量
int16_t i;
int16_t output6020;



//static uint8_t lifestate[3];
//static uint8_t last_lifestate[3];

extern uint8_t life_ball_num;
uint8_t Work_State=0;
uint8_t Dead_flg=0;	// 1是死了
uint16_t Dead_cnt=0;

uint8_t cnt=0;

// 使用定时器2，10ms调用一次,周期单位秒
void Generate_sine(float period,float up,float down,int16_t* output)
{

	period/=9.6;
	int16_t amplitude = (up-down)/2;
	*output = (int16_t)(amplitude*sin(2.0 * PI * i / TIMHz/period)+((up+down)/2.0));
	i++;
	i %=(int16_t)(TIMHz*period);
}

// Dead_flg：死没死过
// Work_state:工作状态 工作/回家/死亡
/*
* 生命球初始化与逻辑状态初始化
* 
*	Work_State为全局使用的逻辑标志位，宏定义见auto_control.h
* 
*/
void LLC_Init()
{
//	memset(lifestate,1,3);
//	memset(last_lifestate,1,3);
	Work_State=GoWork;
}

/*
* 剩余生命球数量检测
* 
*	
* 
*/
uint8_t Left_lifeball_check()
{
//	lifestate[0] = HAL_GPIO_ReadPin(ADC_IN2_GPIO_Port,ADC_IN2_Pin);
//	lifestate[1] =HAL_GPIO_ReadPin(ADC_IN1_GPIO_Port,ADC_IN1_Pin);
//	lifestate[2] =HAL_GPIO_ReadPin(IO4_GPIO_Port,IO4_Pin);
//	for (uint8_t cnt=0;cnt<3;cnt++)
//	{
//		if(lifestate[cnt]<last_lifestate[cnt])life_ball_num-=1;
//		if(lifestate[cnt]>last_lifestate[cnt])life_ball_num+=1;
//	}
//	last_lifestate[0] =lifestate[0];
//	last_lifestate[1] =lifestate[1];
//	last_lifestate[2] =lifestate[2];
	
	// 0.5秒更新一次球数量
	if(HAL_GPIO_ReadPin(ADC_IN2_GPIO_Port,ADC_IN2_Pin)+HAL_GPIO_ReadPin(ADC_IN1_GPIO_Port,ADC_IN1_Pin)+ HAL_GPIO_ReadPin(IO4_GPIO_Port,IO4_Pin)!=life_ball_num)
	{
		cnt++;
		if(cnt==49)
		{
			cnt=0;
			if(HAL_GPIO_ReadPin(ADC_IN2_GPIO_Port,ADC_IN2_Pin)+HAL_GPIO_ReadPin(ADC_IN1_GPIO_Port,ADC_IN1_Pin)+ HAL_GPIO_ReadPin(IO4_GPIO_Port,IO4_Pin)==life_ball_num);
			else life_ball_num=HAL_GPIO_ReadPin(ADC_IN2_GPIO_Port,ADC_IN2_Pin)+HAL_GPIO_ReadPin(ADC_IN1_GPIO_Port,ADC_IN1_Pin)+ HAL_GPIO_ReadPin(IO4_GPIO_Port,IO4_Pin);
		}
	}
	
	return life_ball_num;
}

// Dead_flg：死没死过
// Work_state:工作状态 工作/回家/死亡
/*
* 生命球逻辑判断
* 
*	读取生命球数据并更新Work_State
* 
*/
void Auto_Logic_ctrl()
{
	 if(Work_State==GoWork)
	 {
			if(life_ball_num==1)
			{Work_State=GoHome;}
			else if (life_ball_num==0)
				Work_State=DEAD;
	 }else if(Work_State==DEAD)
	 {
			Dead_cnt++;
		 if(Dead_cnt==100*32) // 100*32
		 {	
				Dead_flg=1;
				Dead_cnt=0;
				Work_State=GoHome;
		 }
	 }else if(Work_State==GoHome)
	 {
			if(Dead_flg==1)
			{
					if(life_ball_num==3)
					{
						Dead_flg=0;
						Work_State=GoWork;
					}
			}else if(Dead_flg==0)
			{
				if(life_ball_num==0)
					Work_State=DEAD;
				else if(life_ball_num==3)
					Work_State=GoWork;
			}
	 }
}
/*
* 判断是否需要回家
* 
*	用于发送数据给nuc
* 
*/
float GoHome_judge()
{
	if(Work_State!=GoWork)
	{
		return 1.0f;
	}
			return 0.0f;
}

/*
* 定时回调函数
* 10ms调用一次，自动重装填
*	用于生命球检测，逻辑判断与产生6020点头用的正弦波
* 
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim == (&htim2))
    {
			Generate_sine(2,7200,6700,&output6020);
			Left_lifeball_check();
			Auto_Logic_ctrl();
		}
    
}