#include "stm32f4xx.h"                  // Device header
#include "can.h"
#include "pid.h"




//һЩ��������
 typedef struct 
{
	uint16_t motor_id;
	uint16_t angle;
    int16_t speed;
    
}motor_recieve;

extern motor_recieve motor_recieve_dipan3508[4];
extern PID pid_dipan3508;




/**
  * @brief  can_filter_init �˺���Ϊ���������ú�����ĿǰΪȫͨ
  * @param  ��
  * @retval ��
  */

//can���������ú���
void can_filter_init(void)
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;           // ���ù�����
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;  // ������ģʽΪ��ʶ������ģʽ
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;  // ���������س���Ϊ32λ
    can_filter_st.FilterIdHigh = 0x0000;               // ��ʶ����λ�������ˣ�
    can_filter_st.FilterIdLow = 0x0000;                // ��ʶ����λ�������ˣ�
    can_filter_st.FilterMaskIdHigh = 0x0000;           // �����λ�������ˣ�
    can_filter_st.FilterMaskIdLow = 0x0000;            // �����λ�������ˣ�
    can_filter_st.FilterBank = 0;                      // ���������
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0; // �������洢��FIFO0
    can_filter_st.SlaveStartFilterBank = 0;           // ��CANʵ������ʼ���������У����ڵ�CANʵ�������壩
    
    // ����CAN������
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    
    // ����CAN������
    HAL_CAN_Start(&hcan1);
    
    // ����CAN�����ж�֪ͨ
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}




/**
  * @brief  HAL_CAN_RxFifo0MsgPendingCallback�˺���Ϊfifo0�Ľ����жϺ����������յ����ĺ󣬵��ô��жϺ���
  * @param  
  * @return ����ֵ
  * @retval ��
  * @note   Ŀǰ����ֻ����һ������ĽǶ����ٶ���Ϣ�������ص��id�������Ľ�
  * @note   �����ڼ��������ٶ�speed������Ƕ�angle�������������������ã������Ľ�
  */

//can���������жϻص�����



//接收电机反馈数据并解析的中断函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];  // ����ÿ֡���ݳ������Ϊ8���ֽ�
 
    // ��CAN����FIFO0��ȡ��Ϣ
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
    // �ɹ����յ�CAN��Ϣ
    // ������������洢���յ�������
	int16_t i =rx_header.StdId -0x201;
	motor_recieve_dipan3508[i].motor_id=rx_header.StdId;
	motor_recieve_dipan3508[i].angle =(rx_data[0] << 8) |rx_data[1];// ʾ���������һ���ֽں͵ڶ����ֽ�Ϊ����Ƕ�����
    motor_recieve_dipan3508[i].speed= (rx_data[2] << 8) |rx_data[3];  // ʾ��������������ֽ�Ϊ���ת������
   // ���Ը��ݾ�������������������ֶ�
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);
    

}


/**
  * @brief  CAN_cmd_3508motor�˺������ڿ��Ƶ����ĸ�35098������������
  * @param  motor1-4ָ��������ָ����Ӧ����ĵ�������ֵ������Ϊint_16�Σ�����Ӧ���޸ģ�
  * @retval ��
  */
static CAN_TxHeaderTypeDef  chassis_tx_message;//�������ݵ�����ͷ
static uint8_t              chassis_can_send_data[8];//Ҫ���͵���������
void CAN_cmd_current_3508motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x200;//����C620�ֲᣬIDΪ1-4ʱ���ͱ�ʶΪ0x200
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8; //id1��� ���õ���ֵ��8λ
    chassis_can_send_data[1] = motor1;      //id1��� ���õ���ֵ��8λ
    chassis_can_send_data[2] = motor2 >> 8; //id2��� ���õ���ֵ��8λ
    chassis_can_send_data[3] = motor2;      //id2��� ���õ���ֵ��8λ
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;
 
    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}



/**
  * @brief  CAN_cmd_speed_3508motor�˺������ڿ��Ƶ����ĸ�35098������ٶ�
  * @param  motor1-4ָ��������ָ����Ӧ������ٶ����������Ϊint_16�Σ�����Ӧ���޸ģ�
  * @retval ��
  */



void CAN_cmd_speed_3508motor(int16_t target[4], motor_recieve motor_recieve_info[4])
{
	
	int16_t motor_currnt[4];
	
	for (uint16_t i = 0; i < 4; i++) 
	{
    motor_currnt[i] = pid_output(pid_dipan3508, motor_recieve_info[i].speed, target[i]);
    }

	CAN_cmd_current_3508motor(motor_currnt[0],motor_currnt[1],motor_currnt[2],motor_currnt[3]);

}


