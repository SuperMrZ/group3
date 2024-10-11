#ifndef QMC5883L_H
#define QMC5883L_H

#include "i2c.h"  // 包含I2C接口的头文件

// QMC5883L的I2C地址
#define QMC5883L_ADDRESS 0x0D 
// 寄存器地址定义
#define MODE_REGISTER 0x09  // 模式寄存器
#define X_REGISTER 0x00     // X轴数据寄存器
#define Y_REGISTER 0x02     // Y轴数据寄存器
#define Z_REGISTER 0x04     // Z轴数据寄存器

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} qmc5883l_raw_data_t;


// 函数声明
void QMC5883L_Init(void);       // 初始化QMC5883L
void QMC5883L_ReadData(int16_t* x, int16_t* y, int16_t* z); // 读取磁力计数据
void QMC5883L_ReadDATA(qmc5883l_raw_data_t *qmc5883l_data);
#endif // QMC5883L_H

