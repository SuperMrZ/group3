#ifndef __RGB_H
#define __RGB_H
#include "main.h"
#define Hight_Data            ( 75  )                           //1 ����Լ���ֵ
#define Low_Data              ( 38  )                           //0 ����Լ���ֵ
#define Reste_Data            ( 80  )                           //80 ��λ��ƽ��Լ���ֵ
#define Led_Num               (  4  )                           //WS2812�Ƹ���
#define Led_Data_Len          ( 24  )                           //WS2812���ݳ��ȣ�������Ҫ24���ֽ�
#define WS2812_Data_Len   (Led_Num * Led_Data_Len)              //ws2812��������Ҫ�����鳤��
 
//uint16_t RGB_buffur[Reste_Data + WS2812_Data_Len] = { 0 }; //���ݻ�������
 
 
void WS2812_Display_1(uint32_t Color, uint16_t num);
void WS2812_Display_2( uint8_t red, uint8_t green, uint8_t blue,uint16_t num);
 
void WS2812_Number_4(uint32_t Color1,uint32_t Color2,uint32_t Color3,uint32_t Color4);//��װ�õ��ĸ��ƺ�����ֻ��Ҫ�ֱ������ĸ��Ƶ���ɫ����
 
 
#endif
 
 