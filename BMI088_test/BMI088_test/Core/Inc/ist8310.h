#pragma once

#include <stdint.h>
#include <ist8310reg.h>
typedef struct ist8310_raw_data_t {
    float x;
    float y;
    float z;
} ist8310_raw_data_t;

typedef enum ist8310_error_e {
    IST8310_NO_ERROR = 0x00,
    MEG_ID_ERROR = 0x01,
} ist8310_error_e;

typedef struct ist8310_data_t {
    uint8_t chip_id;
    ist8310_raw_data_t meg_data;
    ist8310_error_e meg_error;
} ist8310_data_t;

/*-----������uTת��-----*/
#define MAG_SEN 0.3f

/*-----I2C�ӿڶ���-----*/
#define IST8310_I2C_ADDR 0x0E
#define IST8310_I2C hi2c3

/*-----GPIO�ڶ���-----*/
#define IST8310_GPIOx GPIOG
#define IST8310_GPIOp GPIO_PIN_6

void IST8310_INIT();

// ������ȡ����
uint8_t ReadSingleDataFromIST8310(uint8_t addr);
void WriteSingleDataFromIST8310(uint8_t addr, uint8_t data);
void ReadMultiDataFromIST8310(uint8_t addr, uint8_t* data, uint8_t len);
void WriteMultiDataFromIST8310(uint8_t addr, uint8_t* data, uint8_t len);

// ���ܺ���
void ReadIST8310Data(ist8310_raw_data_t* meg_data);

// У�麯��
ist8310_error_e VerifyMegId(uint8_t* id);

