/*
 * MedianFilterTest.cpp
 *
 *  Created on: 23.04.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Tests for moving median filter module.
 */

#include "../MovingMedianFilter.h"
#include "MedianFilterTest.h"

namespace Util {
	namespace Filters {
		namespace Moving {

			void MovingMedianFilterTest::assertTrue( bool value ) {
				if ( !value )  while(1){}
			}

			void MovingMedianFilterTest::assertEquals( uint32_t expected, uint32_t value ) {
				if ( expected != value )  while(1){}
			}

			template <typename T> void MovingMedianFilterTest::assertEquals( T expected, T value ) {
				if ( !(expected == value) )  while(1){}
			}


			void MovingMedianFilterTest::performAllTests() {
				performTest_DeleteInsertAtSameElement1();
				performTest_DeleteInsertAtSameElement2();

				performTest_FirstDeleteLaterInsert1();
				performTest_FirstDeleteLaterInsert2();

				performTest_FirstInsertLaterDelete1();
				performTest_FirstInsertLaterDelete2();

				performTest_ComplexSequence1();
			}

			void MovingMedianFilterTest::performTest_DeleteInsertAtSameElement1() {
				MovingMedianFilter<uint8_t, 5> medianFilter(0);

				medianFilter.process(1);
				assertEquals( 0, medianFilter.getOutput() );

				// Check filter contents
				assertEquals( { /*age*/ 2, /*data*/ 0 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 3, /*data*/ 0 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 4, /*data*/ 0 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 5, /*data*/ 0 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 1, /*data*/ 1 }, medianFilter._elements[4] );
			}

			void MovingMedianFilterTest::performTest_DeleteInsertAtSameElement2() {
				MovingMedianFilter<uint8_t, 5> medianFilter(0);

				// Set filter contents
				medianFilter._elements[0] = { /*age*/ 1, /*data*/ 0 };
				medianFilter._elements[1] = { /*age*/ 2, /*data*/ 0 };
				medianFilter._elements[2] = { /*age*/ 5, /*data*/ 0 };
				medianFilter._elements[3] = { /*age*/ 3, /*data*/ 2 };
				medianFilter._elements[4] = { /*age*/ 4, /*data*/ 2 };

				medianFilter.process(1);
				assertEquals( 1, medianFilter.getOutput() );

				// Check filter contents
				assertEquals( { /*age*/ 2, /*data*/ 0 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 3, /*data*/ 0 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 1, /*data*/ 1 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 4, /*data*/ 2 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 5, /*data*/ 2 }, medianFilter._elements[4] );
			}

			void MovingMedianFilterTest::performTest_FirstDeleteLaterInsert1() {
				MovingMedianFilter<uint8_t, 5> medianFilter(0);

				// Set filter contents
				medianFilter._elements[0] = { /*age*/ 5, /*data*/ 0 };
				medianFilter._elements[1] = { /*age*/ 4, /*data*/ 0 };
				medianFilter._elements[2] = { /*age*/ 3, /*data*/ 0 };
				medianFilter._elements[3] = { /*age*/ 2, /*data*/ 0 };
				medianFilter._elements[4] = { /*age*/ 1, /*data*/ 0 };

				medianFilter.process(1);
				assertEquals( 0, medianFilter.getOutput() );

				// Check filter contents
				assertEquals( { /*age*/ 5, /*data*/ 0 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 4, /*data*/ 0 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 3, /*data*/ 0 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 2, /*data*/ 0 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 1, /*data*/ 1 }, medianFilter._elements[4] );
			}

			void MovingMedianFilterTest::performTest_FirstDeleteLaterInsert2() {
				MovingMedianFilter<uint8_t, 5> medianFilter(0);

				// Set filter contents
				medianFilter._elements[0] = { /*age*/ 2, /*data*/ 1 };
				medianFilter._elements[1] = { /*age*/ 5, /*data*/ 2 };
				medianFilter._elements[2] = { /*age*/ 1, /*data*/ 3 };
				medianFilter._elements[3] = { /*age*/ 3, /*data*/ 5 };
				medianFilter._elements[4] = { /*age*/ 4, /*data*/ 6 };

				medianFilter.process(4);
				assertEquals( 4, medianFilter.getOutput() );

				// Check filter contents
				assertEquals( { /*age*/ 3, /*data*/ 1 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 2, /*data*/ 3 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 1, /*data*/ 4 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 4, /*data*/ 5 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 5, /*data*/ 6 }, medianFilter._elements[4] );
			}

			void MovingMedianFilterTest::performTest_FirstInsertLaterDelete1() {
				MovingMedianFilter<uint8_t, 5> medianFilter(1);

				medianFilter.process(0);
				assertEquals( 1, medianFilter.getOutput() );

				// Check filter contents
				assertEquals( { /*age*/ 1, /*data*/ 0 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 2, /*data*/ 1 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 3, /*data*/ 1 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 4, /*data*/ 1 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 5, /*data*/ 1 }, medianFilter._elements[4] );
			}

			void MovingMedianFilterTest::performTest_FirstInsertLaterDelete2() {
				MovingMedianFilter<uint8_t, 5> medianFilter(0);

				// Set filter contents
				medianFilter._elements[0] = { /*age*/ 1, /*data*/ 1 };
				medianFilter._elements[1] = { /*age*/ 2, /*data*/ 2 };
				medianFilter._elements[2] = { /*age*/ 3, /*data*/ 4 };
				medianFilter._elements[3] = { /*age*/ 5, /*data*/ 5 };
				medianFilter._elements[4] = { /*age*/ 4, /*data*/ 6 };

				medianFilter.process(3);
				assertEquals( 3, medianFilter.getOutput() );

				// Check filter contents
				assertEquals( { /*age*/ 2, /*data*/ 1 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 3, /*data*/ 2 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 1, /*data*/ 3 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 4, /*data*/ 4 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 5, /*data*/ 6 }, medianFilter._elements[4] );
			}

			void MovingMedianFilterTest::performTest_ComplexSequence1() {
				MovingMedianFilter<uint8_t, 5> medianFilter(0);

				// Set filter contents
				medianFilter._elements[0] = { /*age*/ 2, /*data*/ 1 };
				medianFilter._elements[1] = { /*age*/ 5, /*data*/ 2 };
				medianFilter._elements[2] = { /*age*/ 1, /*data*/ 3 };
				medianFilter._elements[3] = { /*age*/ 3, /*data*/ 5 };
				medianFilter._elements[4] = { /*age*/ 4, /*data*/ 6 };

				medianFilter.process(4);
				assertEquals( 4, medianFilter.getOutput() );
				assertEquals( { /*age*/ 3, /*data*/ 1 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 2, /*data*/ 3 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 1, /*data*/ 4 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 4, /*data*/ 5 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 5, /*data*/ 6 }, medianFilter._elements[4] );

				medianFilter.process(1);
				assertEquals( 3, medianFilter.getOutput() );
				assertEquals( { /*age*/ 4, /*data*/ 1 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 1, /*data*/ 1 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 3, /*data*/ 3 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 2, /*data*/ 4 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 5, /*data*/ 5 }, medianFilter._elements[4] );

				medianFilter.process(3);
				assertEquals( 3, medianFilter.getOutput() );
				assertEquals( { /*age*/ 5, /*data*/ 1 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 2, /*data*/ 1 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 4, /*data*/ 3 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 1, /*data*/ 3 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 3, /*data*/ 4 }, medianFilter._elements[4] );

				medianFilter.process(4);
				assertEquals( 3, medianFilter.getOutput() );
				assertEquals( { /*age*/ 3, /*data*/ 1 }, medianFilter._elements[0] );
				assertEquals( { /*age*/ 5, /*data*/ 3 }, medianFilter._elements[1] );
				assertEquals( { /*age*/ 2, /*data*/ 3 }, medianFilter._elements[2] );
				assertEquals( { /*age*/ 4, /*data*/ 4 }, medianFilter._elements[3] );
				assertEquals( { /*age*/ 1, /*data*/ 4 }, medianFilter._elements[4] );
			}

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */
