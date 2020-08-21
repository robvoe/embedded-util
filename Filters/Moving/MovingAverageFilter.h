/*
 * MovingAverageFilter.h
 *
 *  Created on: 02.04.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Implements a moving average filter. Further information:
 *  	  - no heap will be used
 *  	  - algorithm uses as few mathematical operations as possible.
 */

#ifndef UTIL_FILTERS_MOVING_AVERAGE_FILTER_H_
#define UTIL_FILTERS_MOVING_AVERAGE_FILTER_H_

#include "MovingFilterBase.h"

#include <array>
#include <stdint-gcc.h>
#include <type_traits>

#include "Test/AverageFilterTest.h"


namespace Util {
	namespace Filters {
		namespace Moving {

			template <typename T = float, uint_fast8_t Size = 8> class MovingAverageFilter : public MovingFilterBase<T> {
				friend class MovingAverageFilterTest;


				/* TODO: Check if type T supports the following operations:  addition, subtraction and division. */
				static_assert( std::is_pod<T>::value, "Type T must be POD!" );



				private:
					std::array<T, Size>  _elements;
					uint_fast8_t _oldestElementNum = {0};  // "Points" to the oldest element which is going to be overridden on next invocation of "process(..)" function.
					T            _sum;
					T            _filterOutputValue;
					uint_fast8_t _nonEmptyElementsCount = {0};  // Contains the number of non-empty elements in buffer. Grows (increases) until all elements contain process data.


				public:
					MovingAverageFilter(T initValue = T{}) {
						T sum = T{};
						for (uint_fast8_t i = 0; i<Size; i++) {
							_elements[i] = initValue;
							sum += initValue;
						}
						_sum = _filterOutputValue = sum;
					}

					T process(T value) override {
						const T overriddenElement = _elements[_oldestElementNum];
						_elements[_oldestElementNum] = value;

						// Update sum
						_sum -= overriddenElement;
						_sum += value;

						// Shift "pointer" to next element
						_oldestElementNum++;
						if ( _oldestElementNum >= Size ) {
							_oldestElementNum = {0};
						}

						// Increase the amount of non-empty elements
						if ( _nonEmptyElementsCount < Size ) {
							_nonEmptyElementsCount++;
						}

						_filterOutputValue = _sum / Size;
						return _filterOutputValue;
					}

					T getOutput() override {
						return _filterOutputValue;
					}

					bool containsEmptyElements() override {
						return _nonEmptyElementsCount < Size;
					}

					void invalidate() override {
						_nonEmptyElementsCount = {0};
					}

			};

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */

#endif /* UTIL_FILTERS_MOVING_AVERAGE_FILTER_H_ */
