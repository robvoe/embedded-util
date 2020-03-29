/*
 * CallbackDispatcher.h
 *
 *  Created on: 06.12.2018
 *      Author: Robert Voelckner
 *
 *	Description:
 *		Enables modules to interface a queue of callbacks. Applicating modules may register/unregister to that queue.
 */
#ifndef APPLICATION_USER_HARDWARE_TIMER_CALLBACKDISPATCHER_H_
#define APPLICATION_USER_HARDWARE_TIMER_CALLBACKDISPATCHER_H_

#include <type_traits>
#include <array>
#include <stdint-gcc.h>

namespace Util {

	/**
	 * This class provides a callback dispatcher. The callback functions may have either no or one parameter.
	 */
	template <int TCallbackParameterCount=0, typename T1=int, std::size_t TMemorySize = 5>
	class CallbackDispatcher {

		static_assert( TCallbackParameterCount == 0 || TCallbackParameterCount == 1, "Parameter TCallbackParameterCount must equal 0 or 1!" );
		static_assert( !std::is_same<T1, void>::value, "Parameter T1 must not equal 'void'!" );

		public:
			/// The following lines define the callback function
			template <int Count = TCallbackParameterCount>
			using FunctionPointer = typename std::conditional<Count==0,
															  void (*)(),
															  void (*)(T1)>::type;

		private:
			struct CallbackElement {
				FunctionPointer<> Callback;
				bool isValid;
			};
			std::array<CallbackElement, TMemorySize> _callbackElements;


			void addCallback_(FunctionPointer<> callbackHandler) {
				if ( callbackHandler == nullptr )   return;
				for (std::size_t i = 0; i<TMemorySize; i++)
					if ( !_callbackElements[i].isValid ) {
						_callbackElements[i].isValid = true;
						_callbackElements[i].Callback = callbackHandler;
						return;
					}
				while(1);  // Looping forever as there is not enough space for another callback to register..
			}

			void removeCallback_(FunctionPointer<> callbackHandler) {
				for (std::size_t i = 0; i<TMemorySize; i++) {
					if ( _callbackElements[i].isValid  &&  _callbackElements[i].Callback == callbackHandler )
						_callbackElements[i].isValid = false;
				}

			}

		public:
			CallbackDispatcher(){
				removeAllCallbacks();
			}

			template <int ParCount = TCallbackParameterCount, typename std::enable_if<ParCount == 0>::type* = nullptr>
			void addCallback( void(*functionPointer)() ) {
				addCallback_( functionPointer );
			}

			template <int ParCount = TCallbackParameterCount, typename std::enable_if<ParCount == 1>::type* = nullptr>
			void addCallback( void(*functionPointer)(T1) ) {
				addCallback_( functionPointer );
			}

			template <int ParCount = TCallbackParameterCount, typename std::enable_if<ParCount == 0>::type* = nullptr>
			void removeCallback( void(*functionPointer)() ) {
				removeCallback_( functionPointer );
			}

			template <int ParCount = TCallbackParameterCount, typename std::enable_if<ParCount == 1>::type* = nullptr>
			void removeCallback( void(*functionPointer)(T1) ) {
				removeCallback_( functionPointer );
			}

			void removeAllCallbacks() {
				for (std::size_t i = 0; i<TMemorySize; i++)
					_callbackElements[i].isValid = false;
			}

			template <int ParCount = TCallbackParameterCount, typename std::enable_if<ParCount == 0>::type* = nullptr>
			void invoke() {
				for (std::size_t i = 0; i<TMemorySize; i++)
					if ( _callbackElements[i].isValid )
						_callbackElements[i].Callback();
			}

			template <int ParCount = TCallbackParameterCount, typename std::enable_if<ParCount == 1>::type* = nullptr>
			void invoke( T1 argument ) {
				for (std::size_t i = 0; i<TMemorySize; i++)
					if ( _callbackElements[i].isValid )
						_callbackElements[i].Callback( argument );
			}


	}  /* class CallbackDispatcher */;

} /* namespace Util */



#endif /* APPLICATION_USER_HARDWARE_TIMER_CALLBACKDISPATCHER_H_ */
