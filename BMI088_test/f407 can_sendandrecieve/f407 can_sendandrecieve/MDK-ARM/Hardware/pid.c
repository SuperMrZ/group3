#include "stm32f4xx.h"                  // Device header
#include "can.h"
#include "can_user.h"
//����6020�����˵����ѹ����ģʽ��10000��ѹ���룬360rpm���


uint8_t kp=2;
uint8_t ki=1;
uint8_t kd=1;

uint16_t pout;
uint16_t iout;
uint16_t dout;

uint16_t error_now=0;
uint16_t error_last=0;
uint16_t voltage_output=0;

extern uint16_t speed;


uint16_t pid_output(uint8_t want_speed)
{
     error_last=error_now;
	error_now=want_speed-speed;
	pout=kp*error_now;
	iout+=ki*error_now;
	dout=kd*(error_now-error_last);
	voltage_output=(10000/360)*(pout+iout);
	CAN_cmd_6020motor(voltage_output,0,0,0);
	
}