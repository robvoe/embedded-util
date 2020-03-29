/*
 * MedianFilter.h
 *
 *  Created on: 11.04.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Implements a moving median filter. Further information:
 *  	  - no heap will be used,
 *  	  - values are processed highly performant by using insert sort.
 */

#ifndef APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_MEDIANFILTER_H_
#define APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_MEDIANFILTER_H_

#include "MovingFilterBase.h"

#include <array>
#include <stdint-gcc.h>
#include <type_traits>

#include "Test/MedianFilterTest.h"


namespace Util {
	namespace Filters {
		namespace Moving {

			template <typename T = int, uint_fast8_t Size = 5> class MovingMedianFilter : public MovingFilterBase<T> {
				friend class MovingMedianFilterTest;


				/* TODO: Check if type T supports comparing to itself. */
				static_assert( std::is_pod<T>::value, "Type T must be POD!" );



				private:
					static constexpr uint_fast8_t FilterOutputIndex = Size / 2;
					static constexpr uint_fast8_t MinAge = 1;
					static constexpr uint_fast8_t MaxAge = Size;
					static_assert( MaxAge-MinAge+1 == Size, "The age range must include exactly 'n = Size' elements!" );

					struct ElementType {
						uint_fast8_t age; ///< Denotes the age of this element. Range: [1 .. Size], where 1 is the 'youngest' element.
						T data;
						bool operator ==(const ElementType &other) { return age == other.age && data == other.data; } ///< Implemented equality check as support for @see MedianFilterTest
					};
					std::array<ElementType, Size>  _elements;

				public:
					MovingMedianFilter(T initValue = T{}) {
						uint_fast8_t ageCounter = MinAge;
						for (uint_fast8_t i = 0; i<Size; i++) {
							_elements[i].data = initValue;
							_elements[i].age = ageCounter++;
						}
					}

					T process(T value) override {
						bool movingRightAfterInserting = false;
						bool movingLeftAfterDeleting = false;
						ElementType tempElement;

						for ( uint_fast8_t i = 0; i<Size; i++) {
							const bool isLastCycle = (i == (Size - 1));

							// Move right after insertion..
							if ( movingRightAfterInserting  &&  !movingLeftAfterDeleting ) {
								ElementType x = _elements[i];
								_elements[i] = tempElement;
								tempElement = x;
							}

							// Check if deleting this element is desired
							if ( !movingLeftAfterDeleting /* Allow deleting only once.. */ ) {
								const bool startDeleteCondition = _elements[i].age >= MaxAge  ||  ( movingRightAfterInserting && (tempElement.age >= MaxAge) );
								if ( startDeleteCondition ) {
									movingLeftAfterDeleting = true;
								}
							}

							// Move left after deleting..
							if ( movingLeftAfterDeleting  &&  !movingRightAfterInserting ) {
								if ( !isLastCycle )  _elements[i] = _elements[i+1];
							}

							// Check if insertion at this index is desired
							if ( !movingRightAfterInserting /* Allow inserting only once.. */ ) {
								const bool startInsertCondition = (value < _elements[i].data)  ||  isLastCycle;
								if ( startInsertCondition ) {
									movingRightAfterInserting = true;
									tempElement = _elements[i];  // Cache current element for insertion at next index
									_elements[i].data = value;
									_elements[i].age = MinAge - 1;
								}
							}

							_elements[i].age++;

						} /* for ( uint_fast8_t i = 0; i<Size; i++) */

						return _elements[FilterOutputIndex].data;
					}

					T getOutput() override {
						return _elements[FilterOutputIndex].data;
					}

			};

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */

#endif /* APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_MEDIANFILTER_H_ */
