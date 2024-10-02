#include "i2c.h"
#include "qmc5388l.h"

// QMC5883L�ĳ�ʼ�����������ڽ�����������Ϊ��������ģʽ
// �ú���ͨ��I2C�����������д����ƼĴ��������ò���ģʽ������������ʡ�

void QMC5883L_Init(void) {
    uint8_t init_data[2];    // ����һ��2�ֽڵ����飬���ڴ洢Ҫ���͸�QMC5883L�ĳ�ʼ������
    
    init_data[0] = 0x09;     // 0x09 ��QMC5883L���ƼĴ���1�ĵ�ַ
    init_data[1] = 0x1D;     // 0x1D = 011101����ʾ��������ģʽ���������10Hz��ȫ����2G
    
    // ͨ��I2C��QMC5883L�Ŀ��ƼĴ���1���ͳ�ʼ������
    // �������ͣ�hi2c1 �� I2C �����QMC5883L_ADDR << 1 ���豸��ַ��
    // init_data ��Ҫ���͵����ݣ�2 �����ݵĳ��ȣ�HAL_MAX_DELAY ����Ϊ�����ʱ
    HAL_I2C_Master_Transmit(&hi2c1, QMC5883L_ADDR << 1, init_data, 2, HAL_MAX_DELAY);
}



// �ú������ڴ�QMC5883L��ȡX��Y��Z������Ĵų�ǿ�����ݡ�
// ���������x��y��z �ֱ��Ǵ洢X�ᡢY�ᡢZ�����ݵ�ָ�루16λ�������ͣ���

void QMC5883L_Read(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t reg = 0x00;    // ���ݼĴ�����ʼ��ַ��QMC5883L�Ĵų����ݴӵ�ַ 0x00 ��ʼ
    uint8_t buffer[6];     // ���ڴ洢��QMC5883L��ȡ��6���ֽ����ݣ�X��Y��Z���2���ֽڣ�

    // ��QMC5883L���ͼĴ�����ַ������������Ҫ��0x00��ʼ��ȡ����
    // HAL_I2C_Master_Transmit ��������I2C����ģʽ�·�������
    HAL_I2C_Master_Transmit(&hi2c1, QMC5883L_ADDR << 1, &reg, 1, HAL_MAX_DELAY);
    
    // ��QMC5883L��ȡ6���ֽڵ����ݣ�X��Y��Z��������ֽڣ�
    // HAL_I2C_Master_Receive �������ڽ��մӴӻ��豸�������������
    HAL_I2C_Master_Receive(&hi2c1, QMC5883L_ADDR << 1, buffer, 6, HAL_MAX_DELAY);
    
    // ���Ӵ����ƶ�ȡ��2���ֽ����ݺϲ�Ϊһ��16λ���з�������
    // ���ֽ���buffer�����ż������λ�ã����ֽ�����������λ��
    *x = (int16_t)(buffer[0] | (buffer[1] << 8));   // X�����ݣ�buffer[0]�ǵ��ֽڣ�buffer[1]�Ǹ��ֽ�
    *y = (int16_t)(buffer[2] | (buffer[3] << 8));   // Y�����ݣ�buffer[2]�ǵ��ֽڣ�buffer[3]�Ǹ��ֽ�
    *z = (int16_t)(buffer[4] | (buffer[5] << 8));   // Z�����ݣ�buffer[4]�ǵ��ֽڣ�buffer[5]�Ǹ��ֽ�
}
