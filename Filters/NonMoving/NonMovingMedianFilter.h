/*
 * NonMovingMedianFilter.h
 *
 *  Created on: 03.05.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Implements a moving median filter. No heap will be used.
 */

#ifndef APPLICATION_USER_UTIL_FILTERS_NONMOVING_NONMOVINGMEDIANFILTER_H_
#define APPLICATION_USER_UTIL_FILTERS_NONMOVING_NONMOVINGMEDIANFILTER_H_

#include "NonMovingFilterBase.h"

#include <array>

namespace Util {
	namespace Filters {
		namespace NonMoving {

			template <typename T, uint_fast8_t ArraySize> class NonMovingMedianFilter : public NonMovingFilterBase<T, ArraySize> {

				T filter( std::array<T, ArraySize> elements ) override {
					while(1);
					return nullptr;
				}

			};

		} /* namespace NonMoving */
	} /* namespace Filters */
} /* namespace Util */



#endif /* APPLICATION_USER_UTIL_FILTERS_NONMOVING_NONMOVINGMEDIANFILTER_H_ */
