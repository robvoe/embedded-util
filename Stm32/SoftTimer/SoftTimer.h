/*
 * SoftTimer.h
 *
 *  Created on: 07.12.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *		This class provides a software timer that (if enabled) cyclically
 *		invokes a callback function.
 *		For proper functioning, the main() function must be called from main-loop.
 */
#ifndef APPLICATION_USER_STM32SOFTTIMER_TIMER_H_
#define APPLICATION_USER_STM32SOFTTIMER_TIMER_H_

#include <stdint-gcc.h>


namespace Util {
	namespace Stm32 {

		class SoftTimer {

			public:
				typedef void (*CallbackDefinition)(SoftTimer &);

			private:
				SoftTimer(SoftTimer&) = delete;
				uint32_t           _intervalMillis;
				uint32_t           _countingStartedAt;  ///< Contains at which time the interval started.
				bool               _isEnabled;
				CallbackDefinition _callback;


			public:
				/**
				 * Constructor.
				 */
				SoftTimer() : _intervalMillis(0), _countingStartedAt(0), _isEnabled(false), _callback(nullptr) {}

				/**
				 * Constructor.
				 */
				SoftTimer(uint32_t intervalMilliseconds, bool enabled = false, CallbackDefinition callbackFunction = nullptr) : _intervalMillis(intervalMilliseconds), _countingStartedAt(0), _isEnabled(enabled), _callback(callbackFunction) {}

				/**
				 * Main function of the software timer module. Must be called cyclically from main-loop.
				 */
				void main();

				/**
				 * Returns if the timer is currently enabled.
				 */
				bool isEnabled();

				/**
				 * Enables the timer. The timer starts counting as soon as<br>
				 *   - it is enabled, and<br>
				 *   - its interval is set to a non-zero value.
				 */
				void enable();

				/**
				 * Disables the timer.
				 */
				void disable();

				/**
				 * Sets the timer interval. The timer starts counting as soon as<br>
				 *   - it is enabled, and<br>
				 *   - its interval is set to a non-zero value.
				 *
				 * @param milliseconds	..	Holds the milliseconds interval.
				 */
				void setInterval(uint32_t milliseconds);

				/**
				 * Returns the timer interval (milliseconds).
				 */
				uint32_t getInterval();

				/**
				 * Resets counting so that the remaining time equals the interval.
				 */
				void resetCounting();

				/**
				 * Sets the callback function that shall be called by the Timer module.
				 *
				 * @param callbackFunction	..	Holds the function pointer. If nullptr,
				 *                        	  	no function will be called.
				 */
				void setCallbackFunction(CallbackDefinition callbackFunction);

		};

	} /*namespace Stm32*/
} /*namespace Util*/

#endif /* APPLICATION_USER_STM32SOFTTIMER_TIMER_H_ */
