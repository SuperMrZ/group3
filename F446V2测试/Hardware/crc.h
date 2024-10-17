#ifndef __CRC_H__
#define __CRC_H__

#include "stm32f4xx_hal.h"

/**
* @brief CRC16 Verify function
* @param[in] pchMessage : Data to Verify,
* @param[in] dwLength : Stream length = Data + checksum
* @return : True or False (CRC Verify Result)
*/
uint32_t Verify_CRC16_Check_Sum(const uint8_t * pchMessage, uint32_t dwLength);

/**
* @brief Append CRC16 value to the end of the buffer
* @param[in] pchMessage : Data to Verify,
* @param[in] dwLength : Stream length = Data + checksum
* @return none
*/
void Append_CRC16_Check_Sum(uint8_t * pchMessage, uint32_t dwLength);

#endif