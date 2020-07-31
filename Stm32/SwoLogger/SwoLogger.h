/*
 * SwoLogger.h
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Implements simple logging functionality which can be used on SWO output channel.
 */
#ifndef UTIL_STM32_SWOLOGGER_H_
#define UTIL_STM32_SWOLOGGER_H_

#include <stdint-gcc.h>

namespace Util {
	namespace Stm32 {

		class SwoLogger {
			private:
				/******************************* SOME CONSTANTS ********************************/

				/// Definition whether to enable the log output
				static constexpr bool LoggerEnabled =
				#ifndef SWO_LOGGER__ENABLED
					#ifdef DEBUG
						true;
					#else
						false;
					#endif
				#else
					SWO_LOGGER__ENABLED;
				#endif

	
				/// Definition whether to output a milliseconds timestamp
				static constexpr bool EnableTimestampOutput =
				#ifndef SWO_LOGGER__ENABLE_TIMESTAMP_OUTPUT
					true;
				#else
					SWO_LOGGER__ENABLE_TIMESTAMP_OUTPUT;
				#endif


				/**************************** SOME PRIVATE FUNCTIONS ***************************/

				static bool swoEnabled();                                                 ///< Returns if SWO is currently enabled
				static void swoTransmit(const char *s, int_fast16_t len = -1);            ///< Transmits a string to SWO
				static void transferMessage(const char *message);                         ///< Builds up a log message and transfers it to SWO


			public:
				/********************************* CONSTRUCTORS ********************************/

				SwoLogger() = delete;
				SwoLogger( const SwoLogger &other ) = delete;  // Copy constructor
				SwoLogger( const SwoLogger &&other ) = delete; // Move constructor


				/********************************* GENERAL LOGIC *******************************/

				static void log(const char *message, int32_t value);
				static void log(const char *message, float value);
				static void log(const char *message);
		};

	} /* namespace Stm32 */
} /* namespace Util */

#endif /* UTIL_STM32_SWOLOGGER_H_ */
