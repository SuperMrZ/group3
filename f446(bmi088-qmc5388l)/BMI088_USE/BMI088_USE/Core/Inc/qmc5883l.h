#ifndef QMC5883L_H
#define QMC5883L_H

#include "i2c.h"  // ����I2C�ӿڵ�ͷ�ļ�

// QMC5883L��I2C��ַ
#define QMC5883L_ADDRESS 0x0D 
// �Ĵ�����ַ����
#define MODE_REGISTER 0x09  // ģʽ�Ĵ���
#define X_REGISTER 0x00     // X�����ݼĴ���
#define Y_REGISTER 0x02     // Y�����ݼĴ���
#define Z_REGISTER 0x04     // Z�����ݼĴ���

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} qmc5883l_raw_data_t;


// ��������
void QMC5883L_Init(void);       // ��ʼ��QMC5883L
void QMC5883L_ReadData(int16_t* x, int16_t* y, int16_t* z); // ��ȡ����������
void QMC5883L_ReadDATA(qmc5883l_raw_data_t *qmc5883l_data);
#endif // QMC5883L_H

