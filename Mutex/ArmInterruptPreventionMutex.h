/*
 * ArmInterruptPreventionMutex.h
 *
 *  Created on: 01.05.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Implements the MutexBase class. Prevents all ARM Cortex-M interrupts (except for NMI and hard fault) from being executed.
 *    Important: Enabling/disabling interrupts can only performed by software which runs in privileged mode.
 */
#ifndef APPLICATION_USER_UTIL_MUTEX_ARMINTERRUPTPREVENTIONMUTEX_H_
#define APPLICATION_USER_UTIL_MUTEX_ARMINTERRUPTPREVENTIONMUTEX_H_


#include "MutexBase.h"


#ifdef __GNUC__
	#include <cmsis_gcc.h>
#else
	#error Take precautions to include CMSIS here!
#endif

namespace Util {
	namespace Mutex {

		class ArmInterruptPreventionMutex : public MutexBase {

				/// The following switch defines if, upon releasing the Mutex, the previous state shall be restored. If not,
				/// the interrupts will just be enabled again - with no regard to whether they were or were not enabled before.
				#if not defined ARM_INTERRUPT_PREVENTION_MUTEX__RESTORE_PREVIOUS_STATE
					static constexpr bool RestorePreviousState = true;
				#else
					static constexpr bool RestorePreviousState = ARM_INTERRUPT_PREVENTION_MUTEX__RESTORE_PREVIOUS_STATE;
				#endif


				uint32_t _previousPrimaskState;

			public:
				ArmInterruptPreventionMutex() {
					if ( RestorePreviousState ) {
						_previousPrimaskState = __get_PRIMASK();
					}
					__disable_irq();
				}

				~ArmInterruptPreventionMutex() {
					if ( RestorePreviousState ) {
						/*bool interruptsWereDisabled = _previousPrimaskState & UINT32_C(1);
						if ( interruptsWereDisabled )   __disable_irq();
						else                            __enable_irq();*/
						__set_PRIMASK(_previousPrimaskState);
					}
					else if ( !RestorePreviousState ) {
						__enable_irq();
					}
				}

		};

	} /* namespace Mutex */
} /* namespace Util */


#endif /* APPLICATION_USER_UTIL_MUTEX_ARMINTERRUPTPREVENTIONMUTEX_H_ */
