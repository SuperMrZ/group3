#ifndef AUTO_CONTROL
#define AUTO_CONTROL
#include "stm32f4xx.h"

#define DEAD 3
#define GoHome 2
#define GoWork 1

void Generate_sine(float period,float up,float down,int16_t* output);
void LLC_Init();

uint8_t Left_lifeball_check();
void Auto_Logic_ctrl();
float GoHome_judge();

#endif