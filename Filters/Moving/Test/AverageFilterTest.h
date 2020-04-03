/*
 * AverageFilterTest.h
 *
 *  Created on: 02.04.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Tests for average median filter module.
 */

#ifndef UTIL_FILTERS_TEST_MOVING_AVERAGE_FILTER_H_
#define UTIL_FILTERS_TEST_MOVING_AVERAGE_FILTER_H_

#include "../MovingAverageFilter.h"

#include <array>


namespace Util {
	namespace Filters {
		namespace Moving {

			class MovingAverageFilterTest {
					MovingAverageFilterTest() = delete;

				public:
					static void performAllTests();

				private:
					static void assertTrue( bool value );
					static void assertEquals( uint32_t expected, uint32_t value );
					template <typename T> static void assertEquals( T expected, T value );

					static void performTest_ContainsEmptyElements();
					static void performTest_SimpleFloatInsertion();
					static void performTest_SimpleUint8Insertion();

			};

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */

#endif /* UTIL_FILTERS_TEST_MOVING_AVERAGE_FILTER_H_ */
