/*
 * crc8.c
 *
 *  Created on: 29.11.2017
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module provides functionality to calculate CRC-8.
 */
#ifndef APPLICATION_USER_PERSISTENCEMANAGER_CRC8_H_
#define APPLICATION_USER_PERSISTENCEMANAGER_CRC8_H_


#include <stdint-gcc.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


	#define CRC8_WIDTH       (8 * sizeof(uint8_t))
	#define CRC8_TOPBIT      (1 << (CRC8_WIDTH - 1))
	#define CRC8_POLYNOMIAL  UINT8_C(0x31)
	#define CRC8_STARTVALUE  UINT8_C(0xFF)

	/**
	 * Determines the CRC-8 over an amount of data bytes at the following conditions:
	 *   - Polynomial  = CRC8_POLYNOMIAL
	 *   - Start value = CRC8_STARTVALUE
	 *
	 * @param words   	..	Data bytes
	 * @param nWords  	..	Number of data bytes
	 * @return         	..	Returns the CRC-8
	 */
	extern uint8_t calculateCrc8(uint8_t const bytes[], size_t nbytes);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APPLICATION_USER_PERSISTENCEMANAGER_CRC8_H_ */
