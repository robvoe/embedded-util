/*
 * crc8.c
 *
 *  Created on: 29.11.2017
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module provides functionality to calculate CRC-8.
 */
#include "crc8.h"

#include <stdint-gcc.h>
#include <stddef.h>

#pragma GCC optimize ("Os")


#ifdef __GNUC__
	__attribute__((weak)) /*Allows to override the following CRC-32 implmentation by the application*/
#endif
uint8_t calculateCrc8(uint8_t const bytes[], size_t nbytes) {
	uint8_t remainder = CRC8_STARTVALUE;
	for (int byteNum = 0; byteNum < nbytes; ++byteNum) {
		remainder ^= (bytes[byteNum] << (CRC8_WIDTH - 8));

		for (uint8_t bitNum = 8; bitNum > 0; --bitNum) {
			if (remainder & CRC8_TOPBIT) {
				remainder = (remainder << 1) ^ CRC8_POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
	}
	return remainder;
}
