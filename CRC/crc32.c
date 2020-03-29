/*
 * crc32.c
 *
 *  Created on: 29.11.2017
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module provides functionality to calculate CRC-32.
 */
#include "crc32.h"

#include <stdint-gcc.h>
#include <stddef.h>

#pragma GCC optimize ("Os")


#ifdef __GNUC__
	__attribute__((weak)) /*Allows to override the following CRC-32 implmentation by the application*/
#endif
uint32_t calculateCrc32(uint32_t const words[], size_t nWords) {
	uint32_t remainder = CRC32_STARTVALUE;
	for (int wordNum = 0; wordNum < nWords; ++wordNum) {
		remainder ^= (words[wordNum] << (CRC32_WIDTH - 32));

		for (uint8_t bitNum = 8; bitNum > 0; --bitNum) {
			if (remainder & CRC32_TOPBIT) {
				remainder = (remainder << 1) ^ CRC32_POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
	}
	return remainder;
}
