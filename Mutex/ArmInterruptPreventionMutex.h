/*
 * ArmInterruptPreventionMutex.h
 *
 *  Created on: 01.05.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Implements the MutexBase class. Prevents all ARM interrupts from being executed.
 */
#ifndef APPLICATION_USER_UTIL_MUTEX_ARMINTERRUPTPREVENTIONMUTEX_H_
#define APPLICATION_USER_UTIL_MUTEX_ARMINTERRUPTPREVENTIONMUTEX_H_

#ifdef __GNUC__
	#include <cmsis_gcc.h>
#else
	#error Take precautions to include CMSIS here!
#endif

namespace Util {
	namespace Mutex {

		class ArmInterruptPreventionMutex : public MutexBase {

			public:
				ArmInterruptPreventionMutex() {
					__disable_irq();
				}

				~ArmInterruptPreventionMutex() {
					__enable_irq();
				}

		};

	} /* namespace Mutex */
} /* namespace Util */


#endif /* APPLICATION_USER_UTIL_MUTEX_ARMINTERRUPTPREVENTIONMUTEX_H_ */
