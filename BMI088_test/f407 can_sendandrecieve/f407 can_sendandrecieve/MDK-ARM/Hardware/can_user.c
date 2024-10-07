#include "stm32f4xx.h"                  // Device header
#include "can.h"

static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];

void can_filter_init(void)
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;           
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;  
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;              
    can_filter_st.FilterIdLow = 0x0000;               
    can_filter_st.FilterMaskIdHigh = 0x0000;         
    can_filter_st.FilterMaskIdLow = 0x0000;           
    can_filter_st.FilterBank = 0;                      
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0; 
    can_filter_st.SlaveStartFilterBank = 0;       
    
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}
void CAN_cmd_6020motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x1FE;//
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8; //
    chassis_can_send_data[1] = motor1;      //
    chassis_can_send_data[2] = motor2 >> 8; //
    chassis_can_send_data[3] = motor2;      //
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;
 
    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

uint16_t angle;
uint16_t speed;



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];  // 
 
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
   
	angle =(rx_data[0] << 8) |rx_data[1];// 
    speed= (rx_data[2] << 8) |rx_data[3];  // 
 
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);
    

}
