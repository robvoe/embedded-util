/*
 * MovingFilterBase.h
 *
 *  Created on: 11.04.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Abstract base class for moving filters.
 */

#ifndef APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_FILTERBASE_H_
#define APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_FILTERBASE_H_

namespace Util {
	namespace Filters {
		namespace Moving {

			template <typename T = float>
			class MovingFilterBase {
				public:
					virtual T process(T value) = 0;
					virtual T getOutput() = 0;
					virtual ~MovingFilterBase() {};

			}; /* class FilterBase */

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */


#endif /* APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_FILTERBASE_H_ */
