/*
 * MutexBase.h
 *
 *  Created on: 01.05.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Abstract Mutex base class. Upon construction, the regarding code section gets blocked; at destruction
 *    the section gets unlocked again.
 */
#ifndef APPLICATION_USER_UTIL_MUTEX_MUTEXBASE_H_
#define APPLICATION_USER_UTIL_MUTEX_MUTEXBASE_H_


namespace Util {
	namespace Mutex {

		class MutexBase {
			public:
				virtual ~MutexBase() {}
		};

	} /* namespace Mutex */
} /* namespace Util */



#endif /* APPLICATION_USER_UTIL_MUTEX_MUTEXBASE_H_ */
