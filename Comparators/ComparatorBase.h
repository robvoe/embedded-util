/*
 * ComparatorBase.h
 *
 *  Created on: 03.04.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Abstract base class for tri-state comparators.
 */
#ifndef APPLICATION_USER_UTIL_COMPARATORS_COMPARATORBASE_H_
#define APPLICATION_USER_UTIL_COMPARATORS_COMPARATORBASE_H_

namespace Util {
	namespace Comparators {
		
		/// Defines the three possible state a comparator be in
		enum class State {
			High,        ///< Comparator is initially in high state
			Low,         ///< Comparator is initially in low state
			Undefined    ///< Comparator is initially in undefined state
		};


		/// Abstract base class for tri-state comparators
		template <typename T = float>
		class ComparatorBase {
			public:
				/**
				 * Processes an input value.
				 *
				 * @return  Returns if the output state changed. If so, the
				 *          state can be obtained using @see getState().
				 */
				virtual bool process(T value) = 0;

				/**
				 * Returns the current state of the comparator.
				 */
				virtual State getState(void) = 0;

				
				virtual ~ComparatorBase() {};
		};
	
	} /* namespace Comparators */
} /* namespace Util */

#endif /* APPLICATION_USER_UTIL_COMPARATORS_COMPARATORBASE_H_ */
