#include "i2c.h"
#include "qmc5388l.h"

// QMC5883L的初始化函数，用于将磁力计配置为连续测量模式
// 该函数通过I2C发送配置命令，写入控制寄存器，设置测量模式和数据输出速率。

void QMC5883L_Init(void) {
    uint8_t init_data[2];    // 创建一个2字节的数组，用于存储要发送给QMC5883L的初始化数据
    
    init_data[0] = 0x09;     // 0x09 是QMC5883L控制寄存器1的地址
    init_data[1] = 0x1D;     // 0x1D = 011101，表示连续测量模式、输出速率10Hz、全量程2G
    
    // 通过I2C向QMC5883L的控制寄存器1发送初始化数据
    // 参数解释：hi2c1 是 I2C 句柄，QMC5883L_ADDR << 1 是设备地址，
    // init_data 是要发送的数据，2 是数据的长度，HAL_MAX_DELAY 设置为最大延时
    HAL_I2C_Master_Transmit(&hi2c1, QMC5883L_ADDR << 1, init_data, 2, HAL_MAX_DELAY);
}



// 该函数用于从QMC5883L读取X、Y、Z三个轴的磁场强度数据。
// 输入参数：x、y、z 分别是存储X轴、Y轴、Z轴数据的指针（16位整数类型）。

void QMC5883L_Read(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t reg = 0x00;    // 数据寄存器起始地址，QMC5883L的磁场数据从地址 0x00 开始
    uint8_t buffer[6];     // 用于存储从QMC5883L读取的6个字节数据（X、Y、Z轴各2个字节）

    // 向QMC5883L发送寄存器地址，告诉它我们要从0x00开始读取数据
    // HAL_I2C_Master_Transmit 函数用于I2C主机模式下发送数据
    HAL_I2C_Master_Transmit(&hi2c1, QMC5883L_ADDR << 1, &reg, 1, HAL_MAX_DELAY);
    
    // 从QMC5883L读取6个字节的数据（X、Y、Z轴各两个字节）
    // HAL_I2C_Master_Receive 函数用于接收从从机设备传输过来的数据
    HAL_I2C_Master_Receive(&hi2c1, QMC5883L_ADDR << 1, buffer, 6, HAL_MAX_DELAY);
    
    // 将从磁力计读取的2个字节数据合并为一个16位的有符号整数
    // 低字节在buffer数组的偶数索引位置，高字节在奇数索引位置
    *x = (int16_t)(buffer[0] | (buffer[1] << 8));   // X轴数据，buffer[0]是低字节，buffer[1]是高字节
    *y = (int16_t)(buffer[2] | (buffer[3] << 8));   // Y轴数据，buffer[2]是低字节，buffer[3]是高字节
    *z = (int16_t)(buffer[4] | (buffer[5] << 8));   // Z轴数据，buffer[4]是低字节，buffer[5]是高字节
}
