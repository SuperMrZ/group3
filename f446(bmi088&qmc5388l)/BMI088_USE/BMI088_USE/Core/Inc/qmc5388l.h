// QMC5883L的7位I2C地址（0x1A），通信时需要左移一位变为8位地址。
// 在STM32 HAL库中，I2C地址在传输时需要左移1位，因此使用 (QMC5883L_ADDR << 1)。
#define QMC5883L_ADDR  0x1A

void QMC5883L_Init(void); //初始化函数，将磁力计设置为连续测量模式
void QMC5883L_Read(int16_t* x, int16_t* y, int16_t* z);//读取磁力计三个轴的磁场数据