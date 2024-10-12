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
void QMC5883L_ReadData(float * x, float * y, float * z) {
    uint8_t data[6];  // �����������飬���ڴ��X, Y, Z������
	int16_t temp=0; 
    // ��QMC5883L��ȡ6���ֽڵ�����
    HAL_I2C_Mem_Read(&hi2c1, QMC5883L_ADDRESS << 1, X_REGISTER, 1, data, 6, HAL_MAX_DELAY);
    
    // ����ȡ���ֽ�����ת��Ϊ����
   
	temp= (int16_t)((data[1] << 8) | data[0]);// �ϲ�X��ĸߵ��ֽ�
	*x = MAG_SEN *temp;
    temp  = (int16_t)((data[3] << 8) | data[2]);  // �ϲ�Y��ĸߵ��ֽ�
	*y = MAG_SEN *temp;
	temp  = (int16_t)((data[5] << 8) | data[4]);
    *z =MAG_SEN * temp; // �ϲ�Z��ĸߵ��ֽ�
}



//����:�˺�����ȡ���Ĵ��������ݲ���
void QMC5883L_ReadDATA(qmc5883l_raw_data_t *qmc5883l_data) {
    uint8_t data[6];  // �����������飬���ڴ��X, Y, Z������
	int16_t tempx=0,tempy=0,tempz=0;  
    // ��QMC5883L��ȡ6���ֽڵ�����
    HAL_I2C_Mem_Read(&hi2c1, QMC5883L_ADDRESS << 1, X_REGISTER, 1, data, 6, HAL_MAX_DELAY);
    
    // ����ȡ���ֽ�����ת��Ϊ����
    tempx  = (int16_t)((data[1] << 8) | data[0]);// �ϲ�X��ĸߵ��ֽ�
	qmc5883l_data->x = tempx;
    tempy  = (int16_t)((data[3] << 8) | data[2]);  // �ϲ�Y��ĸߵ��ֽ�
	qmc5883l_data->y = tempy;
	tempz  = (int16_t)((data[5] << 8) | data[4]);
    qmc5883l_data->z = tempz; // �ϲ�Z��ĸߵ��ֽ�
	
//	qmc5883l_data->x  = (data[1] << 8) | data[0];  // �ϲ�X��ĸߵ��ֽ�
//  qmc5883l_data->y  = (data[3] << 8) | data[2];  // �ϲ�Y��ĸߵ��ֽ�
//  qmc5883l_data->z  = (data[5] << 8) | data[4];  // �ϲ�Z��ĸߵ��ֽ�
}

