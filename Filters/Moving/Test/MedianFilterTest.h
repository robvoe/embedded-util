/*
 * MedianFilterTest.h
 *
 *  Created on: 23.04.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Tests for moving median filter module.
 */

#ifndef APPLICATION_USER_FILTERS_MEDIANFILTERTEST_H_
#define APPLICATION_USER_FILTERS_MEDIANFILTERTEST_H_

#include "../MovingMedianFilter.h"

#include <array>


namespace Util {
	namespace Filters {
		namespace Moving {

			class MovingMedianFilterTest {
					MovingMedianFilterTest() = delete;

				public:
					static void performAllTests();

				private:
					static void assertTrue( bool value );
					static void assertEquals( uint32_t expected, uint32_t value );
					template <typename T> static void assertEquals( T expected, T value );

					static void performTest_ContainsEmptyElements();

					static void performTest_DeleteInsertAtSameElement1();
					static void performTest_DeleteInsertAtSameElement2();

					static void performTest_FirstDeleteLaterInsert1();
					static void performTest_FirstDeleteLaterInsert2();

					static void performTest_FirstInsertLaterDelete1();
					static void performTest_FirstInsertLaterDelete2();

					static void performTest_ComplexSequence1();
			};

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */

#endif /* APPLICATION_USER_FILTERS_MEDIANFILTERTEST_H_ */
