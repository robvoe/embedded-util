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
			/******************************* SOME CONSTANTS ********************************/

			/// The assigned LogLevel. Only messages with LogLevel above of this will be output.
			#ifndef SWO_LOGGER__LOG_LEVEL
				static constexpr LogLevel CurrentLogLevel = LogLevel::Debug;
			#else
				static constexpr LogLevel CurrentLogLevel = SWO_LOGGER__LOG_LEVEL;
			#endif


			#ifndef SWO_LOGGER__ENABLE_TIMESTAMP_OUTPUT
				static constexpr bool EnableTimestampOutput = true;
			#else
				static constexpr bool EnableTimestampOutput = SWO_LOGGER__ENABLE_TIMESTAMP_OUTPUT;
			#endif


			#ifndef SWO_LOGGER__ENABLE_LOG_LEVEL_OUTPUT
				static constexpr bool EnableLogLevelOutput = true;
			#else
				static constexpr bool EnableLogLevelOutput = SWO_LOGGER__ENABLE_LOG_LEVEL_OUTPUT;
			#endif

			/// Definition whether to enable the log output
			#ifndef SWO_LOGGER__ENABLED
				#ifdef DEBUG
					static constexpr bool Enabled = true;
				#else
					static constexpr bool Enabled = false;
				#endif
			#else
				static constexpr bool Enabled = SWO_LOGGER__ENABLED;
			#endif


			/**************************** SOME PRIVATE FUNCTIONS ***************************/

			static bool swoEnabled();                                                 ///< Returns if SWO is currently enabled
			static void swoTransmit(const char *s, int_fast16_t len = -1);            ///< Transmits a string to SWO
			static void transferMessage(LogLevel logLevel, const char *message);      ///< Builds up a log message and transfers it to SWO
			static bool isLogLevelSufficient(LogLevel logLevel);                      ///< Used to check if current log-level allows to output a certain message

			static void log(LogLevel logLevel, const char *message, int32_t value);
			static void log(LogLevel logLevel, const char *message, float value);
			static void log(LogLevel logLevel, const char *message);


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
