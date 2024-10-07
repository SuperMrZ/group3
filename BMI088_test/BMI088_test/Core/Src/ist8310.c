#include "ist8310.h"

#include <string.h>

#include "i2c.h"
#include "ist8310reg.h"

void IST8310_INIT() { //初始化磁力计
//    memset(ist8310_data, 0, sizeof(ist8310_data_t));

//    ist8310_data->meg_error = IST8310_NO_ERROR;

    // 把磁力计重启
    HAL_GPIO_WritePin(IST8310_GPIOx, IST8310_GPIOp, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(IST8310_GPIOx, IST8310_GPIOp, GPIO_PIN_SET);
    HAL_Delay(5);

    // 基础配置
    // 不使能中断，直接读取
    WriteSingleDataFromIST8310(IST8310_CNTL2_ADDR, IST8310_STAT2_NONE_ALL);
    // 平均采样四次
    WriteSingleDataFromIST8310(IST8310_AVGCNTL_ADDR, IST8310_AVGCNTL_FOURTH);
    // 200Hz的输出频率
    WriteSingleDataFromIST8310(IST8310_CNTL1_ADDR, IST8310_CNTL1_CONTINUE);

//    ist8310_data->meg_error |= VerifyMegId(&ist8310_data->chip_id);
}

// 读取单个字节数据  
uint8_t ReadSingleDataFromIST8310(uint8_t addr) {  
    uint8_t data;  
    // 使用HAL库的I2C接口从IST8310的指定地址读取数据  
    HAL_I2C_Mem_Read(&IST8310_I2C, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);  
    return data;  
}  
  
// 写入单个字节数据  
void WriteSingleDataFromIST8310(uint8_t addr, uint8_t data) {  
    // 使用HAL库的I2C接口向IST8310的指定地址写入数据  
    HAL_I2C_Mem_Write(&IST8310_I2C, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);  
}  
  
// 读取多个字节数据  
void ReadMultiDataFromIST8310(uint8_t addr, uint8_t* data, uint8_t len) {  
    // 使用HAL库的I2C接口从IST8310的指定地址读取多个字节数据  
    HAL_I2C_Mem_Read(&IST8310_I2C, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, data, len, 10);  
}  
  
// 写入多个字节数据  
void WriteMultiDataFromIST8310(uint8_t addr, uint8_t* data, uint8_t len) {  
    // 使用HAL库的I2C接口向IST8310的指定地址写入多个字节数据  
    HAL_I2C_Mem_Write(&IST8310_I2C, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, data, len, 10);  
}  
  
// 从IST8310读取磁场数据  
void ReadIST8310Data(ist8310_raw_data_t* meg_data) {  
    uint8_t buf[6];  
    int16_t temp_ist8310_data = 0;  
    // 从IST8310的数据寄存器读取6个字节的原始磁场数据  
    ReadMultiDataFromIST8310(IST8310_DATA_XL_ADDR, buf, 6);  
    // 将读取到的原始数据转换为有符号的16位整数，并乘以灵敏度系数得到实际的磁场值  
    temp_ist8310_data = (int16_t)((buf[1] << 8) | buf[0]);  
    meg_data->x = MAG_SEN * temp_ist8310_data;  
    temp_ist8310_data = (int16_t)((buf[3] << 8) | buf[2]);  
    meg_data->y = MAG_SEN * temp_ist8310_data;  
    temp_ist8310_data = (int16_t)((buf[5] << 8) | buf[4]);  
    meg_data->z = MAG_SEN * temp_ist8310_data;  
}  
  

ist8310_error_e VerifyMegId(uint8_t* id) {
    *id = ReadSingleDataFromIST8310(IST8310_CHIP_ID_ADDR);
    if (*id != IST8310_CHIP_ID_VAL) {
        return MEG_ID_ERROR;
    } else {
        return IST8310_NO_ERROR;
    }
}

