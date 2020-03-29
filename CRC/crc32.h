/*
 * crc32.h
 *
 *  Created on: 29.11.2017
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module provides functionality to calculate CRC-32.
 */
#ifndef APPLICATION_USER_PERSISTENCEMANAGER_CRC32_H_
#define APPLICATION_USER_PERSISTENCEMANAGER_CRC32_H_


#include <stdint-gcc.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


	#define CRC32_WIDTH       (8 * sizeof(uint32_t))
	#define CRC32_TOPBIT      (1 << (CRC32_WIDTH - 1))
	#define CRC32_POLYNOMIAL  UINT32_C(0x04C11DB7)   /*!<  X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 + X^10 +X^8 + X^7 + X^5 + X^4 + X^2+ X +1 */
	#define CRC32_STARTVALUE  UINT32_C(0xFFFFFFFF)

	/**
	 * Determines the CRC-32 over an amount of data words at the following conditions:
	 *   - Polynomial  = CRC32_POLYNOMIAL
	 *   - Start value = CRC32_STARTVALUE
	 *
	 * @param words   	..	Data words (each of 32 bits)
	 * @param nWords  	..	Number of data words
	 * @return         	..	Returns the CRC-32
	 */
	extern uint32_t calculateCrc32(uint32_t const words[], size_t nWords);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APPLICATION_USER_PERSISTENCEMANAGER_CRC32_H_ */
