/*
 * NonMovingFilterBase.h
 *
 *  Created on: 03.05.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Abstract base class for non-moving filters.
 */

#ifndef APPLICATION_USER_UTIL_FILTERS_NONMOVING_NONMOVINGFILTERBASE_H_
#define APPLICATION_USER_UTIL_FILTERS_NONMOVING_NONMOVINGFILTERBASE_H_

#include <array>

namespace Util {
	namespace Filters {
		namespace NonMoving {

			template <typename T, uint_fast8_t ArraySize> class NonMovingFilterBase {
				public:

					/**
					 * Performs the filter function.
					 *
					 * @param elements The elements that shall be filtered.
					 * @return Returns the filtered value.
					 */
					virtual T filter( std::array<T, ArraySize> elements ) = 0;

					virtual ~NonMovingFilterBase() {}

			};

		} /* namespace NonMoving */
	} /* namespace Filters */
}  /* Util */


#endif /* APPLICATION_USER_UTIL_FILTERS_NONMOVING_NONMOVINGFILTERBASE_H_ */
