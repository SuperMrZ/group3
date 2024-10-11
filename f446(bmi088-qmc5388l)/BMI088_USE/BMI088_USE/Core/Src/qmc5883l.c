#include "qmc5883l.h"  // ����QMC5883L��ͷ�ļ�

// ��ʼ��QMC5883L
void QMC5883L_Init(void) {
    uint8_t config[2];  // �����������飬��żĴ�����ַ������
    config[0] = MODE_REGISTER;  // ��һ��Ԫ���ǼĴ�����ַ
    config[1] = 0x01;            // �ڶ���Ԫ�����������ݣ�����Ϊ����ģʽ
    // ͨ��I2C���ͳ�ʼ�����õ�QMC5883L
    HAL_I2C_Master_Transmit(&hi2c1, QMC5883L_ADDRESS << 1, config, 2, HAL_MAX_DELAY);
}

// ��ȡQMC5883L�Ĵ���������
void QMC5883L_ReadData(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t data[6];  // �����������飬���ڴ��X, Y, Z������
    // ��QMC5883L��ȡ6���ֽڵ�����
    HAL_I2C_Mem_Read(&hi2c1, QMC5883L_ADDRESS << 1, X_REGISTER, 1, data, 6, HAL_MAX_DELAY);
    
    // ����ȡ���ֽ�����ת��Ϊ����
    *x = (data[1] << 8) | data[0];  // �ϲ�X��ĸߵ��ֽ�
    *y = (data[3] << 8) | data[2];  // �ϲ�Y��ĸߵ��ֽ�
    *z = (data[5] << 8) | data[4];  // �ϲ�Z��ĸߵ��ֽ�
}

//����
void QMC5883L_ReadDATA(qmc5883l_raw_data_t *qmc5883l_data) {
    uint8_t data[6];  // �����������飬���ڴ��X, Y, Z������
    // ��QMC5883L��ȡ6���ֽڵ�����
    HAL_I2C_Mem_Read(&hi2c1, QMC5883L_ADDRESS << 1, X_REGISTER, 1, data, 6, HAL_MAX_DELAY);
    
    // ����ȡ���ֽ�����ת��Ϊ����
    qmc5883l_data->x  = (data[1] << 8) | data[0];  // �ϲ�X��ĸߵ��ֽ�
    qmc5883l_data->y  = (data[3] << 8) | data[2];  // �ϲ�Y��ĸߵ��ֽ�
    qmc5883l_data->z  = (data[5] << 8) | data[4];  // �ϲ�Z��ĸߵ��ֽ�
}
