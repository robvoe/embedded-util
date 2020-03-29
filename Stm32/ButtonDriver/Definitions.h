/*
 * Definitions.h
 *
 *  Created on: 17.12.2016
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	This file contains value definitions that are necessary when interfacing ButtonDriver.
 */
#ifndef APPLICATION_USER_HARDWARE_BUTTONDRIVER_BUTTONDRIVERDEFINITIONS_H_
#define APPLICATION_USER_HARDWARE_BUTTONDRIVER_BUTTONDRIVERDEFINITIONS_H_

#include <stdint-gcc.h>


namespace Util {
	namespace Stm32 {
		namespace ButtonDriver {


			class Definitions {
				public:

					/// The amount of milliseconds the button must be pressed until the first Hold Impulse gets fired.
					#ifdef STM32BUTTONDRIVER__HOLD_IMPULSE_PRE_MILLIS
						static constexpr uint32_t HoldImpulsePreMillis = STM32BUTTONDRIVER__HOLD_IMPULSE_PRE_MILLIS;
					#else
						static constexpr uint32_t HoldImpulsePreMillis = 700;
					#endif

					/// The amount of milliseconds between Hold Impulses.
					#ifdef STM32BUTTONDRIVER__HOLD_IMPULSE_MILLIS
						static constexpr uint32_t HoldImpulseMillis = STM32BUTTONDRIVER__HOLD_IMPULSE_MILLIS;
					#else
						static constexpr uint32_t HoldImpulseMillis = 120;
					#endif

					/// The amount of milliseconds used for debouncing.
					#ifdef STM32BUTTONDRIVER__DEBOUNCE_MILLIS
						static constexpr uint32_t DebounceMillis = STM32BUTTONDRIVER__DEBOUNCE_MILLIS;
					#else
						static constexpr uint32_t DebounceMillis = 20;
					#endif
			} /*class*/;


		} /* namespace ButtonDriver */
	} /* namespace Stm32 */
} /* namespace Util */



#endif /* APPLICATION_USER_HARDWARE_BUTTONDRIVER_BUTTONDRIVERDEFINITIONS_H_ */
