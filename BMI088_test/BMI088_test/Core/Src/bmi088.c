#include "bmi088.h"
#include <math.h>
#include "bmi088reg.h"
#include "gpio.h"
#include "spi.h"

// 函数声明:用于向BMI088加速度传感器写入数据   
// addr: 要写入的寄存器的地址（可能通过位操作与写命令合并）  
// data: 要写入的数据  
void WriteDataToAcc(uint8_t addr, uint8_t data) {
	// 将CS（片选）引脚拉低，以选中BMI088设备  
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_RESET);
	
    uint8_t pTxData = (addr & BMI088_SPI_WRITE_CODE);
	// 发送包含写命令和寄存器地址的字节
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
	// 准备发送的数据字节 
    pTxData = data;
	
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_Delay(1);
	
    // 将CS引脚拉高，以取消选中BMI088设备  
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_SET);
}

// 函数定义：通过SPI向BMI088陀螺仪写入数据    
//   uint8_t addr: 要写入的寄存器地址  
//   uint8_t data: 要写入的数据  
void WriteDataToGyro(uint8_t addr, uint8_t data) {  
    // 将陀螺仪的CS引脚拉低，表示开始通信  
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_RESET);  
      
    // 准备要发送的SPI数据包。这里假设BMI088_SPI_WRITE_CODE是一个与写操作相关的位掩码或值  
    // 它通常用于在地址字节中设置写操作的位（例如，某些SPI设备可能要求地址的最高位指示读/写操作）  
    uint8_t pTxData = (addr & BMI088_SPI_WRITE_CODE);  
      
    // 发送地址字节  
    // 使用HAL库函数HAL_SPI_Transmit发送地址到SPI总线  
    // &BMI088_SPI是SPI句柄，&pTxData是指向要发送的数据的指针，1是发送的字节数，1000是超时时间（毫秒）  
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);  
      
    // 等待SPI传输完成  
    // 这是一个忙等待循环，直到SPI状态不再是忙碌状态  
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)  
        ;  
      
    // 准备要发送的数据字节  
    pTxData = data;  
      
    // 发送数据字节  
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);  
      
    // 再次等待SPI传输完成  
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)  
        ;  
      
    // 发送完数据后，将陀螺仪的CS引脚拉高，表示通信结束  
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);  
      
    // 等待一小段时间，以确保数据被正确接收和处理  
    // 注意：这个延时可能不是必需的，具体取决于陀螺仪的规格和时序要求  
    HAL_Delay(1);  
}

// 函数声明，用于从BMI088加速度传感器读取单个数据字节  
// addr: 要读取的寄存器的地址（与读命令位合并）  
// data: 指向用于存储读取数据的变量的指针  
void ReadSingleDataFromAcc(uint8_t addr, uint8_t *data) {
	// 将CS（片选）引脚拉低，以选中BMI088设备  
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_RESET);
	
	// 准备发送的SPI数据：寄存器地址与读命令位合并 
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
	
	// 发送包含读命令和寄存器地址的字节 
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, data, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, data, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
	
	// 将CS引脚拉高，以取消选中BMI088设备
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_SET);
}

// 函数定义：从BMI088陀螺仪读取单个数据字节   
//   uint8_t addr: 要读取的寄存器地址  
//   uint8_t *data: 指向存储读取数据的变量的指针 
void ReadSingleDataFromGyro(uint8_t addr, uint8_t *data) {  
    // 将陀螺仪的CS引脚拉低，开始通信  
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_RESET);  
      
    // 准备要发送的SPI数据包。这里通过OR操作将地址与读操作码组合  
    // BMI088_SPI_READ_CODE是一个位掩码或值，用于在地址字节中设置读操作的位  
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);  
      
    // 发送地址字节，这个地址包含了读操作码  
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);  
      
    // 等待SPI传输完成  
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)  
        ;  
      
    // 接收一个数据字节  
    HAL_SPI_Receive(&BMI088_SPI, data, 1, 1000);  
      
    // 等待SPI接收完成  
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)  
        ;  
      
    // 将陀螺仪的CS引脚拉高，结束通信  
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);  
}

// 函数定义：用于从BMI088加速度传感器读取多个数据字节  
// addr: 要读取的寄存器的地址  
// len: 要读取的字节长度  
// data: 指向存储读取数据的缓冲区的指针 
void ReadMultiDataFromAcc(uint8_t addr, uint8_t len, uint8_t *data) {
	// 将CS（片选）引脚拉低，以选中BMI088设备
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_RESET);
	
	// 准备发送的SPI数据：寄存器地址 | 读命令（通常是最高位设置为1）  
	uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
	
	// 临时变量，用于接收SPI传输的数据
    uint8_t pRxData;
	
	// 发送读命令（包含寄存器地址）  
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);
	
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)
        ;
    HAL_SPI_Receive(&BMI088_SPI, &pRxData, 1, 1000);
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
        ;
	// 循环读取剩余的数据字节
    for (int i = 0; i < len; i++) {
        HAL_SPI_Receive(&BMI088_SPI, &pRxData, 1, 1000);
        while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)
            ;
	// 将接收到的数据存储在提供的缓冲区中
        data[i] = pRxData;
    }
	// 将CS引脚拉高，以取消选中BMI088设备 
    HAL_GPIO_WritePin(BMI088_ACC_GPIOx, BMI088_ACC_GPIOp, GPIO_PIN_SET);
}

// 函数定义：从BMI088读取多个数据字节  
//   uint8_t addr: 要读取的寄存器地址  
//   uint8_t len: 要读取的数据字节长度  
//   uint8_t *data: 指向存储读取数据的数组的指针  
void ReadMultiDataFromGyro(uint8_t addr, uint8_t len, uint8_t *data) {  
    // 将陀螺仪的CS引脚拉低，开始通信  
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_RESET);  
      
    // 准备要发送的SPI数据包，地址与读操作码组合  
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);  
      
    // 发送地址字节  
    HAL_SPI_Transmit(&BMI088_SPI, &pTxData, 1, 1000);  
      
    // 等待SPI传输完成  
    while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_TX)  
        ;  
      
    // 循环读取指定长度的数据  
    for (int i = 0; i < len; i++) {  
        // 接收一个数据字节  
        uint8_t pRxData; // 使用局部变量存储临时接收的数据  
        HAL_SPI_Receive(&BMI088_SPI, &pRxData, 1, 1000);  
          
        // 等待SPI接收完成  
        while (HAL_SPI_GetState(&BMI088_SPI) == HAL_SPI_STATE_BUSY_RX)  
            ;  
          
        // 将接收到的数据存储到输出数组中  
        data[i] = pRxData;  
    }  
      
    // 将陀螺仪的CS引脚拉高，结束通信  
    HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);  
}

void BMI088_CONF_INIT(void) {
    // 加速度计初始化
    // 先软重启，清空所有寄存器
    WriteDataToAcc(ACC_SOFTRESET_ADDR, ACC_SOFTRESET_VAL);
    HAL_Delay(50);
	
    // 打开加速度计电源
    WriteDataToAcc(ACC_PWR_CTRL_ADDR, ACC_PWR_CTRL_ON);
	
    // 加速度计变成正常模式
    WriteDataToAcc(ACC_PWR_CONF_ADDR, ACC_PWR_CONF_ACT);

    // 陀螺仪初始化
    // 先软重启，清空所有寄存器
    WriteDataToGyro(GYRO_SOFTRESET_ADDR, GYRO_SOFTRESET_VAL);
    HAL_Delay(50);
    // 陀螺仪变成正常模式
    WriteDataToGyro(GYRO_LPM1_ADDR, GYRO_LPM1_NOR);

    // 加速度计配置写入
    // 写入范围，+-3g的测量范围
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_3G);
    // 写入配置，正常带宽，1600hz输出频率
    WriteDataToAcc(ACC_CONF_ADDR,
                   (ACC_CONF_RESERVED << 7) | (ACC_CONF_BWP_NORM << 6) | (ACC_CONF_ODR_1600_Hz));

    // 陀螺仪配置写入
    // 写入范围，+-500°/s的测量范围
    WriteDataToGyro(GYRO_RANGE_ADDR, GYRO_RANGE_500_DEG_S);
    // 写入带宽，2000Hz输出频率，532Hz滤波器带宽
    WriteDataToGyro(GYRO_BANDWIDTH_ADDR, GYRO_ODR_2000Hz_BANDWIDTH_532Hz);
}

//读取加速度函数
void ReadAccData(acc_raw_data_t *data) {
    uint8_t buf[ACC_XYZ_LEN], range;// 定义一个缓冲区来存储加速度的原始数据，以及一个变量来存储加速度范围 
    int16_t acc[3];                 // 数组，存储x, y, z轴的加速度原始值 
	
	// 读取加速度传感器的范围设置
    ReadSingleDataFromAcc(ACC_RANGE_ADDR, &range);
	
	// 读取x, y, z轴的原始数据  
    ReadMultiDataFromAcc(ACC_X_LSB_ADDR, ACC_XYZ_LEN, buf);
	
	// 将原始数据转换为16位整数
    acc[0] = ((int16_t)buf[1] << 8) + (int16_t)buf[0];
    acc[1] = ((int16_t)buf[3] << 8) + (int16_t)buf[2];
    acc[2] = ((int16_t)buf[5] << 8) + (int16_t)buf[4];
	// 根据加速度传感器的灵敏度（假设为BMI088_ACCEL_3G_SEN），将原始值转换为g单位的加速度
    data->x = (float)acc[0] * BMI088_ACCEL_3G_SEN;
    data->y = (float)acc[1] * BMI088_ACCEL_3G_SEN;
    data->z = (float)acc[2] * BMI088_ACCEL_3G_SEN;
}

//读取角度函数
void ReadGyroData(gyro_raw_data_t *data) {
    uint8_t buf[GYRO_XYZ_LEN], range;// 定义一个缓冲区来存储陀螺仪的原始数据，以及一个变量来存储陀螺仪范围  
    int16_t gyro[3];                 // 数组,存储x, y, z轴的陀螺仪原始值  
    float unit;                      // 存储陀螺仪的灵敏度单位 
	
	// 读取陀螺仪传感器的范围设置
    ReadSingleDataFromGyro(GYRO_RANGE_ADDR, &range);
	
	// 根据范围设置确定陀螺仪的灵敏度单位 
    switch (range) {
        case 0x00:
            unit = 16.384;
            break;
        case 0x01:
            unit = 32.768;
            break;
        case 0x02:
            unit = 65.536;
            break;
        case 0x03:
            unit = 131.072;
            break;
        case 0x04:
            unit = 262.144;
            break;
        default:
            unit = 16.384;
            break;
    }
	// 从陀螺仪传感器读取x, y, z轴的原始数据  
    ReadMultiDataFromGyro(GYRO_RATE_X_LSB_ADDR, GYRO_XYZ_LEN, buf);
	
	// 将原始数据转换为16位整数  
    gyro[0] = ((int16_t)buf[1] << 8) + (int16_t)buf[0];
    gyro[1] = ((int16_t)buf[3] << 8) + (int16_t)buf[2];
    gyro[2] = ((int16_t)buf[5] << 8) + (int16_t)buf[4];
	
	// 将原始值转换为度/秒的角速度值
    data->roll = (float)gyro[0] / unit * DEG2SEC;
    data->pitch = (float)gyro[1] / unit * DEG2SEC;
    data->yaw = (float)gyro[2] / unit * DEG2SEC;
}







