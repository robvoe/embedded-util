/*
 * Fifo.h
 *
 *  Created on: 28.04.2017
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This class contains a circular buffer for POD value types.
 *
 *    Further information:
 *      - no usage of dynamic memory,
 *      - in order to prevent race conditions, the use of Mutexes is possible.
 */
#ifndef APPLICATION_USER_FIFO_FIFO_H_
#define APPLICATION_USER_FIFO_FIFO_H_


#include <array>
#include <type_traits>

#include <Mutex/MutexBase.h>
#include <Mutex/NoMutex.h>

namespace Util {
	namespace Lists {
		namespace StaticMemory {

			template <typename T, bool OverwriteOldestElementIfFull = false, uint_fast8_t TArraySize = 15, typename MutexImpl = Util::Mutex::NoMutex>
			class Fifo {
				static_assert( std::is_constructible<MutexImpl>::value, "The given Mutex type must be constructible using the non-arguments-constructor!" );
				static_assert( std::is_pod<T>::value, "T must be Plain-Old-Data!" );

				public:

					Fifo() {
						m_nextReadIndex = m_nextWriteIndex = m_elementCount = 0;
					}

					~Fifo(void) {
						// Nothing happens here..
					}

					/**
					 * Returns if the circular buffer is empty.
					 */
					inline bool isEmpty(void) const {
						if ( m_elementCount == 0 )  return true;
						return false;
					}

					/**
					 * Returns if the circular buffer is full.
					 */
					inline bool isFull(void) const {
						if ( m_elementCount >= TArraySize ) return true;
						return false;
					}

					/**
					 * Returns the maximum size of the circular buffer.
					 */
					inline uint_fast8_t size() const {
						return TArraySize;
					}

					/**
					 * Returns the number of present elements.
					 */
					inline uint_fast8_t Count() const {
						return m_elementCount;
					}

					/**
					 * Returns the number of free element slots.
					 */
					inline uint_fast8_t freeElementCount() const {
						return (TArraySize - m_elementCount);
					}

					/**
					 * Clears the circular buffer.
					 */
					void clear(void) {
						MutexImpl _mutex;  // Locks the following code section; upon destruction the section gets unlocked automatically.
						m_nextReadIndex = m_nextWriteIndex = m_elementCount = 0;
					}

					/**
					 * Applies (copies) one element to the circular buffer.
					 *
					 * @remark In case the circular buffer is full, the element won't be copied.
					 *
					 * @param copyFromElement 	..	The element that will be applied.
					 * @return                	..	Returns if the operation was successful.
					 */
					template <bool Overwrite = OverwriteOldestElementIfFull, typename std::enable_if<!Overwrite>::type* = nullptr>  /* Spezialisierung für "OverwriteOldestElementIfFull = false" */
					bool enqueue( const T& copyFromElement ) {
						MutexImpl _mutex;  // Locks the following code section; upon destruction the section gets unlocked automatically.
						if ( isFull() )  {
							return false;
						}
						m_elementArray[m_nextWriteIndex] = copyFromElement;
						m_nextWriteIndex++;     if ( m_nextWriteIndex >= TArraySize )  m_nextWriteIndex = 0;
						m_elementCount++;
						return true;
					}

					/**
					 * Applies (copies) one element to the circular buffer.
					 *
					 * @remark In case the circular buffer is full, the oldest element will be overwritten.
					 *
					 * @param copyFromElement 	..	The element that will be applied.
					 * @return                	..	Returns if the operation was successful.
					 */
					template <bool Overwrite = OverwriteOldestElementIfFull, typename std::enable_if<Overwrite>::type* = nullptr>  /* Spezialisierung für "OverwriteOldestElementIfFull = true" */
					bool enqueue( const T& copyFromElement ) {
						MutexImpl _mutex;  // Locks the following code section; upon destruction the section gets unlocked automatically.
						m_elementArray[m_nextWriteIndex] = copyFromElement;
						m_nextWriteIndex++;     if ( m_nextWriteIndex >= TArraySize )  m_nextWriteIndex = 0;
						if ( !isFull() ) {
							m_elementCount++;
						} else /*if ( isFull() )*/ {
							m_nextReadIndex++;  if ( m_nextReadIndex >= TArraySize )  m_nextReadIndex = 0;
						}
						return true;
					}

					/**
					 * Fetches the oldest element and removes it from circular buffer.
					 *
					 * @param copyDestination 	..	Holds the destination the element should be copied to. May be NULL!
					 * @return                	..	Returns if the operation was successful.
					 *                    	      	  true:  The operation was successful.
					 *                    	      	  false: The circular buffer was empty; operation not successful.
					 */
					bool dequeue( T *copyDestination ) {
						MutexImpl _mutex;  // Locks the following code section; upon destruction the section gets unlocked automatically.
						if ( isEmpty() )  {
							return false;
						}
						if (copyDestination)  *copyDestination = m_elementArray[m_nextReadIndex];
						m_nextReadIndex++;  if ( m_nextReadIndex >= TArraySize )  m_nextReadIndex = 0;
						m_elementCount--;
						return true;
					}

					/**
					 * Verwirft das älteste Element aus dem Ringpuffer.
					 */
					void dequeue(void) {
						dequeue(nullptr);
					}

					/**
					 * Returns a pointer to the oldest element, without removing it from buffer.
					 *
					 * @return	..	Pointer to the oldest element. Will be NULL if the circular buffer is empty.
					 */
					T* peek() {
						MutexImpl _mutex;  // Locks the following code section; upon destruction the section gets unlocked automatically.
						if ( isEmpty() )  {
							return 0;
						}
						T* retPtr = &m_elementArray[m_nextReadIndex];
						return retPtr;
					}

					/**
					 * Returns a pointer to a certain element, without removing it from buffer.
					 *
					 * @param index   	..	Index of the desired element. Index 0 corresponds to the pointer returned by @see peek().
					 * @return        	..	Pointer to the element. Will be NULL if there is no element at the given index.
					 */
					T* peek( uint_fast8_t index ) {
						MutexImpl _mutex;  // Locks the following code section; upon destruction the section gets unlocked automatically.
						if ( index >= m_elementCount )  {
							return 0;
						}
						uint_fast8_t returnedArrayElement = m_nextReadIndex;
						while (index) {
							returnedArrayElement++;  if ( returnedArrayElement >= TArraySize )   returnedArrayElement = 0;
							index--;
						}
						T* retPtr = &m_elementArray[returnedArrayElement];
						return retPtr;
					}


				private:
					std::array<T, TArraySize> m_elementArray; 	///< Contains all elements.
					volatile uint_fast8_t m_nextReadIndex;     	///< Always points to the field containing the oldest element.
					volatile uint_fast8_t m_nextWriteIndex;    	///< Always points to the next free field.
					volatile uint_fast8_t m_elementCount;      	///< Contains the amount of used elements.

			}; /* class */

		} /*namespace StaticMemory*/
	} /* namespace Lists */
} /* namespace Util */

#endif /* APPLICATION_USER_FIFO_FIFO_H_ */
