#include "stm32f4xx.h"                  // Device header
#include "can.h"
#include "pid.h"




//一些变量声明
 typedef struct 
{
	uint16_t motor_id;
	uint16_t angle;
    int16_t speed;
    
}motor_recieve;

extern motor_recieve motor_recieve_dipan3508[4];
extern PID pid_dipan3508;




/**
  * @brief  can_filter_init 此函数为过滤器配置函数，目前为全通
  * @param  无
  * @retval 无
  */

//can过滤器配置函数
void can_filter_init(void)
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;           // 启用过滤器
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;  // 过滤器模式为标识符掩码模式
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;  // 过滤器比特长度为32位
    can_filter_st.FilterIdHigh = 0x0000;               // 标识符高位（不过滤）
    can_filter_st.FilterIdLow = 0x0000;                // 标识符低位（不过滤）
    can_filter_st.FilterMaskIdHigh = 0x0000;           // 掩码高位（不过滤）
    can_filter_st.FilterMaskIdLow = 0x0000;            // 掩码低位（不过滤）
    can_filter_st.FilterBank = 0;                      // 过滤器编号
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0; // 过滤器存储到FIFO0
    can_filter_st.SlaveStartFilterBank = 0;           // 从CAN实例的起始过滤器银行（对于单CAN实例无意义）
    
    // 配置CAN过滤器
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    
    // 启动CAN控制器
    HAL_CAN_Start(&hcan1);
    
    // 激活CAN接收中断通知
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}




/**
  * @brief  HAL_CAN_RxFifo0MsgPendingCallback此函数为fifo0的接受中断函数，当接收到报文后，调用此中断函数
  * @param  
  * @return 返回值
  * @retval 无
  * @note   目前函数只返回一个电机的角度与速度信息，不返回电机id，后续改进
  * @note   函数内计算出电机速度speed，电机角度angle，需在主函数声明调用，后续改进
  */

//can接受数据中断回调函数



//鎺ユ敹鐢垫満鍙嶉鏁版嵁骞惰В鏋愮殑涓柇鍑芥暟
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];  // 假设每帧数据长度最多为8个字节
 
    // 从CAN接收FIFO0读取消息
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
    // 成功接收到CAN消息
    // 在这里解析并存储接收到的数据
	int16_t i =rx_header.StdId -0x201;
	motor_recieve_dipan3508[i].motor_id=rx_header.StdId;
	motor_recieve_dipan3508[i].angle =(rx_data[0] << 8) |rx_data[1];// 示例：假设第一个字节和第二个字节为电机角度数据
    motor_recieve_dipan3508[i].speed= (rx_data[2] << 8) |rx_data[3];  // 示例：假设第三个字节为电机转速数据
   // 可以根据具体情况解析更多数据字段
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);
    

}


/**
  * @brief  CAN_cmd_3508motor此函数用于控制底盘四个35098电机的输入电流
  * @param  motor1-4指的是你想指定相应电机的电流输入值，类型为int_16形（后续应该修改）
  * @retval 无
  */
static CAN_TxHeaderTypeDef  chassis_tx_message;//发送数据的数据头
static uint8_t              chassis_can_send_data[8];//要发送的数据数组
void CAN_cmd_current_3508motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x200;//查阅C620手册，ID为1-4时发送标识为0x200
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8; //id1电机 设置电流值高8位
    chassis_can_send_data[1] = motor1;      //id1电机 设置电流值低8位
    chassis_can_send_data[2] = motor2 >> 8; //id2电机 设置电流值高8位
    chassis_can_send_data[3] = motor2;      //id2电机 设置电流值低8位
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;
 
    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}



/**
  * @brief  CAN_cmd_speed_3508motor此函数用于控制底盘四个35098电机的速度
  * @param  motor1-4指的是你想指定相应电机的速度输出，类型为int_16形（后续应该修改）
  * @retval 无
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


