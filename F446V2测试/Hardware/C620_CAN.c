//
// Created by 20125 on 2024/7/9.
//

#include "C620_CAN.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "stdio.h"
#include "string.h"

uint8_t Rx_Temp[8]={0};
uint8_t Rx_data1[8]={0};
uint8_t Rx_data2[8]={0};
uint8_t Rx_data3[8]={0};
uint8_t Rx_data4[8]={0};
static CAN_RxHeaderTypeDef RxHeader;

/**
 * 开启CAN通信，需要在过滤器配置好后调用
 * @param hcan 开启的CAN句柄
 */
void My_can_start(CAN_HandleTypeDef* hcan)
{
    HAL_CAN_Start(hcan);
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING); //开启中断接收
}


	
/**
 * 配置并开启CAN过滤器，
 * @param hcan 配置过滤器的CAN句柄
 */
void My_can_flt_init(CAN_HandleTypeDef *hcan)
{
    CAN_FilterTypeDef		CAN_FilterConfigStructure;

    CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN_FilterConfigStructure.FilterBank = 0;
    CAN_FilterConfigStructure.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(hcan, &CAN_FilterConfigStructure);

//    CAN_FilterConfigStructure.SlaveStartFilterBank = 14;//can1(0-13)和can2(14-27)分别得到一半的filter
//    CAN_FilterConfigStructure.FilterBank = 14;
//    HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterConfigStructure);
//    HAL_CAN_Start(&hcan2);
//    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void Can12_Start(void)
{
	CAN_FilterTypeDef		CAN_FilterConfigStructure;

    CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN_FilterConfigStructure.FilterBank = 0;
    CAN_FilterConfigStructure.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterConfigStructure);
		CAN_FilterConfigStructure.SlaveStartFilterBank = 14;//can1(0-13)和can2(14-27)分别得到一半的filter
    CAN_FilterConfigStructure.FilterBank = 14;
		HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterConfigStructure);
	
	My_can_start(&hcan1);
	My_can_start(&hcan2);
}

/**
 * 向CAN总线上发送控制C620电调的报文
 * @param hcan  句柄
 * @param iq1   电调上ID为1的电机的控制电流
 * @param iq2   电调上ID为2的电机的控制电流
 * @param iq3   ...
 * @param iq4
 */
void CAN_Set_c620_current(CAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{

    static CAN_TxHeaderTypeDef Tx_Header;
    uint8_t Txdata[8];
    uint32_t send_mail_box; //定义发送邮箱
    if(hcan->Instance==CAN1)
    {
        Tx_Header.StdId=0x200;
        Tx_Header.ExtId=0;
        Tx_Header.IDE = CAN_ID_STD; //ID为标准ID
        Tx_Header.RTR=0;            //数据帧
        Tx_Header.DLC=0x08;         //8字节内容
    }
    else if(hcan->Instance==CAN2)
    {
        Tx_Header.StdId=0x1FF;
        Tx_Header.ExtId=0;
        Tx_Header.IDE = CAN_ID_STD; //ID为标准ID
        Tx_Header.RTR=0;            //数据帧
        Tx_Header.DLC=0x08;         //8字节内容
    }

   Txdata[0] = iq1 >> 8;
   Txdata[1] = iq1;
   Txdata[2] = iq2 >> 8;
   Txdata[3] = iq2;
   Txdata[4] = iq3 >> 8;
   Txdata[5] = iq3;
   Txdata[6] = iq4 >> 8;
   Txdata[7] = iq4;
    HAL_CAN_AddTxMessage(hcan, &Tx_Header, Txdata, &send_mail_box);
}
/**
 * 返回CAN总线上电调C620报文中的电机速度
 * @param hcan
 * @param ID
 * @param Rxdata
 * @return
 */
int16_t C620_GetSpeed(CAN_HandleTypeDef *hcan,uint16_t ID)
{
   if(hcan->Instance==CAN1)
   {
       switch (0x200|ID)
       {
           case 0x201:
               return (Rx_data1[2]<<8)|Rx_data1[3];
           case 0x202:
               return (Rx_data2[2]<<8)|Rx_data2[3];
           case 0x203:
               return (Rx_data3[2]<<8)|Rx_data3[3];
           case 0x204:
               return (Rx_data4[2]<<8)|Rx_data4[3];
           default:
               break;
       }
   }
//   else if (hcan->Instance == CAN2)
//   {
//
//   }
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance== CAN1)
    {
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, Rx_Temp);
        if(RxHeader.StdId==0x201)
        {
            memcpy(Rx_data1,Rx_Temp,8);
        }
        else if(RxHeader.StdId==0x202)
        {
            memcpy(Rx_data2,Rx_Temp,8);
        }else if(RxHeader.StdId==0x203)
        {
            memcpy(Rx_data3,Rx_Temp,8);
        }else if(RxHeader.StdId==0x204)
        {
            memcpy(Rx_data4,Rx_Temp,8);
        }
    }
}

