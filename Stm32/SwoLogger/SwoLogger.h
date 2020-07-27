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

namespace Stm32 {
	
	class SwoLogger {
		public:
			enum class LogLevel {
				Debug = 0,
				Info = 1,
				Warning = 2,
				Error = 3
			};

		private:
			static LogLevel CurrentLogLevel;   ///< Contains the currently assigned LogLevel. Initially, it contains Debug

			static void swoTransmit(const char *s, int_fast16_t len = -1);
			static void transferMessage(LogLevel logLevel, const char *message);

			static void log(LogLevel logLevel, const char *message, int32_t value);
			static void log(LogLevel logLevel, const char *message, float value);
			static void log(LogLevel logLevel, const char *message);

			/**
			 * This function is used to implement a log-level filter.
			 */
			static bool isLogLevelSufficient(LogLevel logLevel);


			/********************************** CONSTANTS **********************************/
			static constexpr const char* LogLevelStringDebug = "DEBUG";
			static constexpr const char* LogLevelStringInfo = "INFO";
			static constexpr const char* LogLevelStringWarning = "WARNING";
			static constexpr const char* LogLevelStringError = "ERROR";


		public:
			/********************************* CONSTRUCTORS ********************************/

			SwoLogger() = delete;
			SwoLogger( const SwoLogger &other ) = delete;  // Copy constructor
			SwoLogger( const SwoLogger &&other ) = delete; // Move constructor

			
			/********************************* GENERAL LOGIC *******************************/

			static void setLogLevel( LogLevel logLevel );

			static void debug(const char *message);
			static void debug(const char *message, int32_t value);
			static void debug(const char *message, float value);

			static void info(const char *message);
			static void info(const char *message, int32_t value);
			static void info(const char *message, float value);

			static void warning(const char *message);
			static void warning(const char *message, int32_t value);
			static void warning(const char *message, float value);

			static void error(const char *message);
			static void error(const char *message, int32_t value);
			static void error(const char *message, float value);
	};

} /* namespace Stm32 */

#endif /* UTIL_STM32_SWOLOGGER_H_ */
