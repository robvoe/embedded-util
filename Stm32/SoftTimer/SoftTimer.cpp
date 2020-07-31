/*
 * SoftTimer.cpp
 *
 *  Created on: 07.12.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *		This class provides a software timer that (if enabled) cyclically
 *		invokes a callback function.
 *		For proper functioning, the main() function must be called from main-loop.
 */
#include <IncludeStmHal.h>
#include "SoftTimer.h"


namespace Util {
	namespace Stm32 {

		void SoftTimer::main() {
			if ( !_isEnabled || _intervalMillis == 0 )
				return;
			const uint32_t currentTime = HAL_GetTick();
			if ( currentTime-_countingStartedAt  >=  _intervalMillis ) {
				_countingStartedAt = currentTime;
				volatile const CallbackDefinition callback = _callback;
				if ( callback != nullptr )   callback(*this);
			}
		}

		bool SoftTimer::isEnabled() {
			return _isEnabled;
		}

		void SoftTimer::enable() {
			_countingStartedAt = HAL_GetTick();
			_isEnabled = true;
		}

		void SoftTimer::disable() {
			_isEnabled = false;
		}

		void SoftTimer::setInterval(uint32_t milliseconds) {
			_countingStartedAt = HAL_GetTick();
			_intervalMillis = milliseconds;
		}

		uint32_t SoftTimer::getInterval() {
			return _intervalMillis;
		}

		void SoftTimer::resetCounting() {
			_countingStartedAt = HAL_GetTick();
		}

		void SoftTimer::setCallbackFunction(CallbackDefinition callbackFunction) {
			_callback = callbackFunction;
		}

	} /*namespace Stm32*/
} /*namespace Util*/
