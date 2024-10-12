#include "qmc5883l.h"  // 包含QMC5883L的头文件

// 初始化QMC5883L
void QMC5883L_Init(void) {
    uint8_t config[2];  // 定义配置数组，存放寄存器地址和数据
    config[0] = MODE_REGISTER;  // 第一个元素是寄存器地址
    config[1] = 0x01;            // 第二个元素是配置数据，设置为正常模式
    // 通过I2C发送初始化配置到QMC5883L
    HAL_I2C_Master_Transmit(&hi2c1, QMC5883L_ADDRESS << 1, config, 2, HAL_MAX_DELAY);
}

// 读取QMC5883L的磁力计数据
void QMC5883L_ReadData(float * x, float * y, float * z) {
    uint8_t data[6];  // 定义数据数组，用于存放X, Y, Z轴数据
	int16_t temp=0; 
    // 从QMC5883L读取6个字节的数据
    HAL_I2C_Mem_Read(&hi2c1, QMC5883L_ADDRESS << 1, X_REGISTER, 1, data, 6, HAL_MAX_DELAY);
    
    // 将读取的字节数据转换为整数
   
	temp= (int16_t)((data[1] << 8) | data[0]);// 合并X轴的高低字节
	*x = MAG_SEN *temp;
    temp  = (int16_t)((data[3] << 8) | data[2]);  // 合并Y轴的高低字节
	*y = MAG_SEN *temp;
	temp  = (int16_t)((data[5] << 8) | data[4]);
    *z =MAG_SEN * temp; // 合并Z轴的高低字节
}



//问题:此函数读取到的磁力计数据不对
void QMC5883L_ReadDATA(qmc5883l_raw_data_t *qmc5883l_data) {
    uint8_t data[6];  // 定义数据数组，用于存放X, Y, Z轴数据
	int16_t tempx=0,tempy=0,tempz=0;  
    // 从QMC5883L读取6个字节的数据
    HAL_I2C_Mem_Read(&hi2c1, QMC5883L_ADDRESS << 1, X_REGISTER, 1, data, 6, HAL_MAX_DELAY);
    
    // 将读取的字节数据转换为整数
    tempx  = (int16_t)((data[1] << 8) | data[0]);// 合并X轴的高低字节
	qmc5883l_data->x = tempx;
    tempy  = (int16_t)((data[3] << 8) | data[2]);  // 合并Y轴的高低字节
	qmc5883l_data->y = tempy;
	tempz  = (int16_t)((data[5] << 8) | data[4]);
    qmc5883l_data->z = tempz; // 合并Z轴的高低字节
	
//	qmc5883l_data->x  = (data[1] << 8) | data[0];  // 合并X轴的高低字节
//  qmc5883l_data->y  = (data[3] << 8) | data[2];  // 合并Y轴的高低字节
//  qmc5883l_data->z  = (data[5] << 8) | data[4];  // 合并Z轴的高低字节
}

