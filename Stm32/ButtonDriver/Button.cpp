/*
 * Button.cpp
 *
 *  Created on: 17.12.2016
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	This class provides state management of hardware buttons. This includes
 *  	  - debouncing,
 *  	  - hold recignition ("hold impulse")
 *  	 and lots more.
 *
 *  	 Further information:
 *  	 - each instance of this class represents one button,
 *  	 - each instance's main() function must be called cyclically from main loop,
 *  	 - no usage of dynamic (heap) memory.
 */
#include <IncludeStmHal.h>

#include <stdint-gcc.h>

#include "Button.h"
#include "Definitions.h"


namespace Util {
	namespace Stm32 {
		namespace ButtonDriver {

			Button::Button( GPIO_TypeDef *gpioPort, uint16_t gpioPin, GPIO_PinState idleGpioState, bool holdImpulseEnabled, KeyEventCallbackFunction keyDownCallbackFunction, KeyEventCallbackFunction keyUpCallbackFunction ) : State(*this) {
				m_gpioPort = gpioPort;
				m_gpioPin = gpioPin;
				m_idleGpioState = idleGpioState;
				m_holdImpulseEndabled = holdImpulseEnabled;
				m_keyDownCallbackFunction = keyDownCallbackFunction;
				m_keyUpCallbackFunction = keyUpCallbackFunction;

				m_state = ButtonState::Idle;
				m_stateCounter = 0U;
				m_impulseAlreadyTriggered = 0U;
				m_previousCallMillis = 0U;
			}


			void Button::main() {
				if ( m_state == ButtonState::Down ) {
					m_state = ButtonState::Post_Down_Debounce;
					m_stateCounter=0;
				}
				else if ( m_state == ButtonState::Up ) {
					m_state = ButtonState::Post_Up_Debounce;
					m_stateCounter=0;
				}
				else if ( m_state == ButtonState::HoldImpulse ) {
					m_state  =ButtonState::Hold;
					m_impulseAlreadyTriggered=1U;
					m_stateCounter=0;
				}


				///////////////// The following will be executed after at least one millisecond..

				uint32_t millis_passed = HAL_GetTick()-m_previousCallMillis;
				if ( millis_passed == 0U ) return;
				m_previousCallMillis = HAL_GetTick();


				// Button ist Idle --> Wechsel zu Down?
				if ( m_state==ButtonState::Idle  &&  HAL_GPIO_ReadPin(m_gpioPort, m_gpioPin) != m_idleGpioState ) {
					m_state=ButtonState::Down;
					if ( m_keyDownCallbackFunction != 0U )  m_keyDownCallbackFunction(*this);
					return;
				}

				// Button ist im Down-Warte-Modus --> Zähle Zähler hoch. Nach Ablauf der Prellzeit wechselt er in Hold
				if ( m_state==ButtonState::Post_Down_Debounce ) {
					m_stateCounter+=millis_passed;
					if ( m_stateCounter > Definitions::DebounceMillis ) {
						m_state=ButtonState::Hold;
						m_impulseAlreadyTriggered=0;
						m_stateCounter=0;
					}
					return;
				}

				// Button ist Hold --> Prüfe ob der Hold-Impuls an der Reihe ist
				if ( m_state==ButtonState::Hold  &&  m_holdImpulseEndabled ) {
					m_stateCounter+=millis_passed;
					if ( !m_impulseAlreadyTriggered  &&  m_stateCounter > Definitions::HoldImpulsePreMillis ) {
						m_state=ButtonState::HoldImpulse;
						if ( m_keyDownCallbackFunction != 0U )  m_keyDownCallbackFunction(*this);
						return;
					}
					if ( m_impulseAlreadyTriggered  &&  m_stateCounter > Definitions::HoldImpulseMillis ) {
						m_state=ButtonState::HoldImpulse;
						if ( m_keyDownCallbackFunction != 0U )  m_keyDownCallbackFunction(*this);
						return;
					}
				}

				// Button ist Hold, wurde aber soeben losgelassen? --> Wechsel zu Up
				if ( m_state==ButtonState::Hold  &&  HAL_GPIO_ReadPin(m_gpioPort, m_gpioPin) == m_idleGpioState ) {
					m_state=ButtonState::Up;
					if ( m_keyUpCallbackFunction != 0U )  m_keyUpCallbackFunction(*this);
					return;
				}

				// Button ist im Up-Warte-Modus --> Zähle Zähler hoch. Nach Ablauf der Prellzeit wechselt er in Idle
				if ( m_state==ButtonState::Post_Up_Debounce ) {
					m_stateCounter+=millis_passed;
					if ( m_stateCounter > Definitions::DebounceMillis ) {
						m_state=ButtonState::Idle;
					}
				}

			}




			void Button::setKeyDownCallbackFunction( KeyEventCallbackFunction keyDownCallbackFunction ) {
				m_keyDownCallbackFunction = keyDownCallbackFunction;
			}


			void Button::setKeyUpCallbackFunction( KeyEventCallbackFunction keyUpCallbackFunction ) {
				m_keyUpCallbackFunction = keyUpCallbackFunction;
			}

		} /* namespace ButtonDriver */
	} /* namespace Stm32 */
} /* namespace Util */
