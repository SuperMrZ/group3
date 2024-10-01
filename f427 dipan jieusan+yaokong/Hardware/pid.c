#include "stm32f4xx.h"                  // Device header

typedef struct
{
//p,i,d²ÎÊýÖµ,maxI»ý·ÖÏÞ·ù£¬maxOÊä³öÏÞ·ù
float kp;
float ki;
int16_t kd;
int16_t maxI;  //maxI»ý·ÖÏÞ·ù
int16_t maxO;  //maxOÊä³öÏÞ·ù

} PID;




//²»Í¬µç»úµÄpid²ÎÊý
 PID pid_dipan3508={5,0.15,0,0x1000,0x5000};
extern PID pid_dipan3508;


/**
  * @brief  pid_output´Ëº¯ÊýÓÃÓÚÊä³öÒ»¸öpidÊä³ö
  * @param  kp,ki,kd,maxI,maxO,·Ö±ðÖ¸pidËã·¨²ÎÊý£¬targetÎªÄãÏë´ïµ½µÄÄ¿±êÖµ£¬feedbackÎªµ±Ç°Ä¿±êµÄÖµ£¨·´À¡Öµ£©
  * @note   ×¢ÒâfeedbackÓëtargetµÄ±äÁ¿ÀàÐÍÖÃ·ñÐèÒª¸ü¸Ä
  * @retval ÎÞ
  */
  



int16_t pid_output(PID pid, int16_t feedback,int16_t target) //ä¸‰ä¸ªå‚æ•°åˆ†åˆ«ä¸ºpidç³»æ•°å€¼ï¼Œè¾“å‡ºçš„å€¼ï¼Œç›®æ ‡å€¼
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




