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
					/**
					 * Processes an element and returns the filter output.
					 */
					virtual T process(T value) = 0;

					/**
					 * Returns the current filter output value.
					 */
					virtual T getOutput() = 0;

					/**
					 * Returns if the filter contains empty elements (i.e. zero values right after starting).
					 *
					 * @remark May be used to determine if the filter is in steady state.
					 */
					virtual bool containsEmptyElements() = 0;

					/**
					 * Invalidates current filter contents. @see ContainsEmptyElements() returns false until all
					 * filter elements were updated/overwritten by new values.
					 */
					virtual void invalidate() = 0;

					virtual ~MovingFilterBase() {};

			}; /* class FilterBase */

		} /* namespace Moving */
	} /* namespace Filters */
} /* namespace Util */


#endif /* APPLICATION_USER_HARDWARE_ZEROCROSSINGDETECTOR_FILTER_FILTERBASE_H_ */
