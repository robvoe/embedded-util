/*
 * NoMutex.h
 *
 *  Created on: 01.05.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Implements the MutexBase class. No special Mutex actions will be done here.
 */
#ifndef APPLICATION_USER_UTIL_MUTEX_NOMUTEX_H_
#define APPLICATION_USER_UTIL_MUTEX_NOMUTEX_H_


namespace Util {
	namespace Mutex {

		class NoMutex : public MutexBase {
			// Nothing happens in here
		};

	} /* namespace Mutex */
} /* namespace Util */


#endif /* APPLICATION_USER_UTIL_MUTEX_NOMUTEX_H_ */
