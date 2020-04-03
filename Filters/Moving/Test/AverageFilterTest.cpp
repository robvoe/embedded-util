/*
 * AverageFilterTest.cpp
 *
 *  Created on: 02.04.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Tests for moving average filter module.
 */

#include "../MovingAverageFilter.h"
#include "AverageFilterTest.h"

namespace Util {
	namespace Filters {
		namespace Moving {

			void MovingAverageFilterTest::assertTrue( bool value ) {
				if ( !value )  while(1){}
			}

			void MovingAverageFilterTest::assertEquals( uint32_t expected, uint32_t value ) {
				if ( expected != value )  while(1){}
			}

			template <typename T> void MovingAverageFilterTest::assertEquals( T expected, T value ) {
				if ( !(expected == value) )  while(1){}
			}


			void MovingAverageFilterTest::performAllTests() {
				performTest_ContainsEmptyElements();
				performTest_SimpleFloatInsertion();
				performTest_SimpleUint8Insertion();
			}

			void MovingAverageFilterTest::performTest_ContainsEmptyElements() {
				MovingAverageFilter<uint8_t, 4> averageFilter(0);

				assertEquals( true, averageFilter.containsEmptyElements() );
				assertEquals( 0, averageFilter._nonEmptyElementsCount );

				averageFilter.process(1);
				assertEquals( true, averageFilter.containsEmptyElements() );
				assertEquals( 1, averageFilter._nonEmptyElementsCount );

				averageFilter.process(2);
				assertEquals( true, averageFilter.containsEmptyElements() );
				assertEquals( 2, averageFilter._nonEmptyElementsCount );

				averageFilter.process(3);
				assertEquals( true, averageFilter.containsEmptyElements() );
				assertEquals( 3, averageFilter._nonEmptyElementsCount );

				averageFilter.process(4);
				assertEquals( false, averageFilter.containsEmptyElements() );
				assertEquals( 4, averageFilter._nonEmptyElementsCount );

				averageFilter.process(5);
				assertEquals( false, averageFilter.containsEmptyElements() );
				assertEquals( 4, averageFilter._nonEmptyElementsCount );

				averageFilter.process(6);
				assertEquals( false, averageFilter.containsEmptyElements() );
				assertEquals( 4, averageFilter._nonEmptyElementsCount );

				averageFilter.process(7);
				assertEquals( false, averageFilter.containsEmptyElements() );
				assertEquals( 4, averageFilter._nonEmptyElementsCount );
			}

			void MovingAverageFilterTest::performTest_SimpleFloatInsertion() {
				MovingAverageFilter<float, 4> averageFilter;

				assertEquals( 0, averageFilter.getOutput() );

				averageFilter.process(1);
				assertEquals( 0.25f, averageFilter.getOutput() );

				averageFilter.process(2);
				assertEquals( 0.75f, averageFilter.getOutput() );

				averageFilter.process(3);
				assertEquals( 1.5f, averageFilter.getOutput() );

				averageFilter.process(4);
				assertEquals( 2.5f, averageFilter.getOutput() );

				averageFilter.process(5);
				assertEquals( 3.5f, averageFilter.getOutput() );
			}

			void MovingAverageFilterTest::performTest_SimpleUint8Insertion() {
				MovingAverageFilter<uint8_t, 4> averageFilter;

				assertEquals( 0, averageFilter.getOutput() );

				averageFilter.process(1);
				assertEquals( 0, averageFilter.getOutput() );

				averageFilter.process(2);
				assertEquals( 0, averageFilter.getOutput() );

				averageFilter.process(3);
				assertEquals( 1, averageFilter.getOutput() );

				averageFilter.process(4);
				assertEquals( 2, averageFilter.getOutput() );

				averageFilter.process(5);
				assertEquals( 3, averageFilter.getOutput() );

				averageFilter.process(6);
				assertEquals( 4, averageFilter.getOutput() );
			}



		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */
