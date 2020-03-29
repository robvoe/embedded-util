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
#ifndef APPLICATION_USER_HARDWARE_BUTTONDRIVER_BUTTONDRIVER_H_
#define APPLICATION_USER_HARDWARE_BUTTONDRIVER_BUTTONDRIVER_H_

#include <IncludeStmHal.h>


#include <stdint-gcc.h>



namespace Util {
	namespace Stm32 {
		namespace ButtonDriver {

			class Button {
				private:

					/// This construct allows the application to access the button state as follows: 'buttonInstance.State.xx'
					class StateAccessor {
						private:
							Button &m_button;
						public:
							StateAccessor(Button &button) : m_button(button) {}
							bool isDown(void) { return m_button.m_state == ButtonState::Down; }
							bool isUp(void) { return m_button.m_state == ButtonState::Up; }
							bool isIdle(void) { return m_button.m_state == ButtonState::Idle; }
							bool isHold(void) { return m_button.m_state == ButtonState::Hold; }
							bool isHoldImpulse(void) { return m_button.m_state == ButtonState::HoldImpulse; }
					};



				public:

					StateAccessor State;  	///< Allows the application to read the button state.


					/// Declaration of callback function pointer.
					typedef void (*KeyEventCallbackFunction)(Button &button);


					/**
					 * Constructor, initializes a button.
					 *
					 * Important:
					 * Each button instance's main() function must be called cyclically from main loop!
					 *
					 * @param gpioPort                	..	The GPIO port that the button is connected to. The data structure expected here matches the Stm32Cube port structure.
					 * @param gpioPin                  	..	The GPIO pin that the button is connected to.
					 * @param idleGpioState            	..	The pin state (GPIO_PIN_RESET | GPIO_PIN_SET) the button is NOT pressed. The data structure expected here matches the Stm32Cube port structure.
					 * @param holdImpulseEnabled       	..	Denotes if Hold Impulse shall be enabled for this button. If true, the driver cyclically fires an event as long as the button is held down.
					 * @param keyDownCallbackFunction  	..	Pointer to the callback function that handles Button Down (and Hold Impulse) events. May be nullptr in case no function shall be called.
					 * @param keyUpCallbackFunction   	..	Pointer to the callback function that handles Button Up events. May be nullptr in case no function shall be called.
					 */
					Button(GPIO_TypeDef *gpioPort, uint16_t gpioPin, GPIO_PinState idleGpioState, bool holdImpulseEnabled, KeyEventCallbackFunction keyDownCallbackFunction = nullptr, KeyEventCallbackFunction keyUpCallbackFunction = nullptr);


					/**
					 * Main function. Must be called cyclically from main loop FOR EACH INSTANCE.
					 */
					void main();


					/**
					 * Assigns the callback function that handles Button Down and Hold Impulse events.
					 *
					 * @param keyDownCallbackFunction 	..	New callback function. If nullptr, no callback function will be called.
					 */
					void setKeyDownCallbackFunction( KeyEventCallbackFunction keyDownCallbackFunction );


					/**
					 * Assigns the callback function that handles Button Up events.
					 *
					 * @param keyUpCallbackFunction 	..	New callback function. If nullptr, no callback function will be called.
					 */
					void setKeyUpCallbackFunction( KeyEventCallbackFunction keyUpCallbackFunction );




				private:
					Button();

					/// Describes the possible button states. Internally used. The application should use @see State to access the button state.
					enum class ButtonState {
						Idle = 0,
						Down,       //!< Down
						Post_Down_Debounce,
						Hold,       //!< Hold
						HoldImpulse,//!< HoldImpulse
						Up,         //!< Up
						Post_Up_Debounce
					} ;

					GPIO_TypeDef *m_gpioPort;
					uint16_t      m_gpioPin;
					GPIO_PinState m_idleGpioState;
					bool          m_holdImpulseEndabled;
					KeyEventCallbackFunction  m_keyDownCallbackFunction;
					KeyEventCallbackFunction  m_keyUpCallbackFunction;

					uint32_t      m_stateCounter;
					uint32_t      m_impulseAlreadyTriggered;
					ButtonState   m_state;
					uint32_t      m_previousCallMillis;
			};


		} /* namespace ButtonDriver */
	} /* namespace Stm32 */
} /* namespace Util */


#endif /* APPLICATION_USER_HARDWARE_BUTTONDRIVER_BUTTONDRIVER_H_ */
