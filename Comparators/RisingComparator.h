/*
 * RisingComparator.h
 *
 *  Created on: 03.04.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Implements a simple comparator where
 *    - low values result in low output state,
 *    - high values result in high output state.
 *
 *    The user may refer to the descriptive image file "RisingScheme.png".
 */
#ifndef APPLICATION_USER_UTIL_COMPARATORS_RISINGCOMPARATOR_H_
#define APPLICATION_USER_UTIL_COMPARATORS_RISINGCOMPARATOR_H_

#include "ComparatorBase.h"

namespace Util {
	namespace Comparators {
		
		template <typename T = float>
		class RisingComparator : public ComparatorBase<T> {
			private:
				RisingComparator( const RisingComparator& other ) = delete;  // Copy constructor
				RisingComparator( const RisingComparator&& other ) = delete; // Move constructor

				T     _compareValue;
				T     _compareHysteresis;
				State _currentState;

			public:
				/********************************* CONSTRUCTORS ********************************/

				RisingComparator(T compareValue, T compareHysteresis, State initialState = State::Undefined) {
					_compareValue = compareValue;
					_compareHysteresis = compareHysteresis;
					_currentState = initialState;
				}


				/******************************** OVERRIDDEN LOGIC *****************************/

				bool process(T value) override {
					State oldState = _currentState;

					if ( value >= _compareValue )
						_currentState = State::High;
					else if ( value < _compareValue - _compareHysteresis )
						_currentState = State::Low;

					return (oldState != _currentState);
				}

				State getState(void) override {
					return _currentState;
				}


				/******************************** GETTERS/SETTERS ******************************/

				/**
				 * Returns the hysteresis value
				 */
				T getCompareHysteresis() const {
					return _compareHysteresis;
				}

				/**
				 * Sets the hysteresis value.
				 *
				 * @remark The user might want to call @see process() function afterwards.
				 */
				void setCompareHysteresis( T newHysteresis ) {
					_compareHysteresis = newHysteresis;
				}

				/**
				 * Returns the compare value
				 */
				T getCompareValue() const {
					return _compareValue;
				}

				/**
				 * Sets the compare value.
				 *
				 * @remark The user might want to call @see process() function afterwards.
				 */
				void setCompareValue( T newValue ) {
					_compareValue = newValue;
				}
		}; /*class*/
	
	} /* namespace Comparators */
} /* namespace Util */

#endif /* APPLICATION_USER_UTIL_COMPARATORS_RISINGCOMPARATOR_H_ */
