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
#ifndef APPLICATION_USER_UTIL_COMPARATORS_SINGLECOMPARATOR_H_
#define APPLICATION_USER_UTIL_COMPARATORS_SINGLECOMPARATOR_H_

#include "ComparatorBase.h"

namespace Util {
	namespace Comparators {
		
		template <typename T = float>
		class SingleComparator : public ComparatorBase<T> {
			private:
				SingleComparator( const SingleComparator& other ) = delete;  // Copy constructor
				SingleComparator( const SingleComparator&& other ) = delete; // Move constructor

				ComparatorState _currentState;

			public:
				T     CompareValue;
				T     CompareHysteresis;


				/********************************* CONSTRUCTORS ********************************/

				SingleComparator(T compareValue, T compareHysteresis, ComparatorState initialState = ComparatorState::Undefined) {
					CompareValue = compareValue;
					CompareHysteresis = compareHysteresis;
					_currentState = initialState;
				}


				/******************************** OVERRIDDEN LOGIC *****************************/

				bool process(T value) override {
					ComparatorState oldState = _currentState;

					if ( value >= CompareValue )
						_currentState = ComparatorState::High;
					else if ( value < CompareValue - CompareHysteresis )
						_currentState = ComparatorState::Low;

					return (oldState != _currentState);
				}

				ComparatorState getState(void) override {
					return _currentState;
				}


		}; /*class*/
	
	} /* namespace Comparators */
} /* namespace Util */

#endif /* APPLICATION_USER_UTIL_COMPARATORS_SINGLECOMPARATOR_H_ */
