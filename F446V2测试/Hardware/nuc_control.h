#ifndef NUC_CONTROL_H
#define NUC_CONTROL_H

#include "stm32f4xx.h" 

typedef struct 
{
	uint8_t header;
	float pitch;
	float yaw;
	float position_x;
	float position_y;
	float shoot;
} Nuc_manu_cmd;

typedef struct
{
	uint8_t header;
	float yaw_yuntai;
	float pitch_yuntai;
	float homeornot;
} Tx_Data_toNuc;

typedef struct 
{
	uint8_t header;
	float angle_error;
	float linear_speed;
	float pitch_flag;
	float yaw_flag;
	
} Nuc_auto_cmd;

void Nuc_Tele_Init();
void Nuc_Tele_CRC_Init();

void Nuc_Manu_Decode(uint8_t* rx_buf,Nuc_manu_cmd* cmd);
void Nuc_Auto_Decode(uint8_t* rx_buf, Nuc_auto_cmd* cmd,uint8_t data_length);

void Tx_data_to_Nuc(void);

void real_to_dipan();
void Motion_transverse(float linearspeed,float anglespeed);

void Nuc_ctrl();
void Nuc_data_limit(float linear_limit,float angle_limit);
float code_to_hudu_6020(uint16_t* encoder_num);

#endif