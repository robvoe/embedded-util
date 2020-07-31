/*
 * SwoLogger.cpp
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Implements simple logging functionality which can be used on SWO output channel.
 */
#include "SwoLogger.h"

#include <string.h>

#include <StringUtil/number_to_string_conversions.h>
#include <IncludeStmHal.h>


namespace Util {
	namespace Stm32 {

		bool SwoLogger::swoEnabled() {
			if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&      /* ITM enabled */
				((ITM->TER & 1UL               ) != 0UL)   )     /* ITM Port #0 enabled */
				return true;
			return false;
		}
	
		void SwoLogger::swoTransmit(const char *s, int_fast16_t len) {
			if ( len == -1 ) {
				while (*s) {
					ITM_SendChar(*s);
					s++;
				}
			} else if ( len > 0 ) {
				for (int_fast16_t i = 0; i < len; i++) {
					ITM_SendChar(*s++);
				}
			}
		}

		void SwoLogger::transferMessage(const char *message) {
			// Write current milliseconds time
			if ( EnableTimestampOutput ) {
				using namespace StringUtil;
				char timeBuffer[10];  // Reserves 10 places for the highest-possible uint32_t
				uint32_t writtenCharCount = int32_to_string(HAL_GetTick(), timeBuffer, 10, false, ValueRadix::Decimal, false, false);
				swoTransmit(timeBuffer, writtenCharCount);
				swoTransmit("ms - ");
			}

			swoTransmit(message);
		}


		void SwoLogger::log(const char *message) {
			if ( !LoggerEnabled )  return;
			if ( !swoEnabled() )  return;

			transferMessage(message);
			swoTransmit("\r\n");
		}

		void SwoLogger::log(const char *message, int32_t value) {
			if ( !LoggerEnabled )  return;
			if ( !swoEnabled() )  return;

			transferMessage(message);

			using namespace StringUtil;
			char strBuffer[10];  // Reserves 10 places for the highest-possible uint32_t
			uint32_t writtenCharCount = int32_to_string(value, strBuffer, 10, false, ValueRadix::Decimal, false, false);
			swoTransmit(strBuffer, writtenCharCount);

			swoTransmit("\r\n");
		}

		void SwoLogger::log(const char *message, float value) {
			if ( !LoggerEnabled )  return;
			if ( !swoEnabled() )  return;

			transferMessage(message);

			using namespace StringUtil;
			char strBuffer[10 /*pre-comma places*/ + 3 /*post-comma places*/ + 1 /*separator*/];
			uint32_t writtenCharCount = float32_to_string(value, strBuffer, 10, 3, false, false, false, false);
			swoTransmit(strBuffer, writtenCharCount);

			swoTransmit("\r\n");
		}

	} /* namespace Stm32 */
} /* namespace Util */
