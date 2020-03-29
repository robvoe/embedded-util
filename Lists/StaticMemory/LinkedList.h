/*
 * LinkedList.h
 *
 *  Created on: 12.04.2019
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	Implements a doubly linked list for POD value types.
 *
 *  	Further information:
 *      - no usage of dynamic memory,
 *      - in order to prevent race conditions, the use of Mutexes is possible.
 */

#ifndef APPLICATION_USER_LISTS_STATICMEMORY_LINKEDLIST_H_
#define APPLICATION_USER_LISTS_STATICMEMORY_LINKEDLIST_H_

#include <stdint-gcc.h>
#include <array>

#include <Mutex/MutexBase.h>
#include <Mutex/NoMutex.h>

namespace Util {
	namespace Lists {
		namespace StaticMemory {

			template <typename T, uint_fast16_t Size, typename MutexImpl = Util::Mutex::NoMutex>
			class LinkedList {
				static_assert( std::is_constructible<MutexImpl>::value, "The given Mutex type must be constructible using the non-arguments-constructor!" );
				static_assert( std::is_pod<T>::value, "T must be Plain-Old-Data!" );

				private:
					/*********************** INTERNAL TYPES *************************/
					using index_t = uint_fast16_t;

					struct element_t {
						bool containsValidData;
						element_t *nextElement;
						element_t *previousElement;
						T data;
					};

					/*************************** FIELDS *****************************/
					std::array<element_t, Size> _elements;
					index_t                     _elementsCount;

					element_t                  *_headElement, *_tailElement;

					/****************************************************************/

					element_t *findNextFreeSlot() {
						for (index_t i = 0; i<Size; i++) {
							if (_elements[i].containsValidData)  return &(_elements[i]);
						}
						return nullptr;
					}

					/****************************************************************/

				public:
					LinkedList() {
						_elementsCount = 0;
						for (index_t i = 0; i<Size; i++) {
							_elements[i].containsValidData = false;
						}
						_headElement = nullptr;
						_tailElement = nullptr;
					}

					uint_fast16_t size() {
						return _elementsCount;
					}

					constexpr uint_fast16_t maxSize() {
						return Size;
					}

					bool addHead(T& value) {
						if ( _elementsCount >= Size )  return false;

						element_t *ptr;
						if ( _headElement == nullptr ) {
							_headElement = _tailElement = _elements;
							ptr = _headElement;
						} else {
							ptr = findNextFreeSlot();
							ptr->nextElement = _headElement;
							_headElement->previousElement = ptr;
							_headElement = ptr;
						}
						ptr->containsValidData = true;
						ptr->data = value;

						_elementsCount++;
						return true;
					}

					bool addTail(T& value) {
						while(1);
						// TODO
					}

					bool add(T& value) {
						return addTail(value);
					}

					void removeHead() {
						while(1);
						// TODO
					}

					void removeTail() {
						while(1);
						// TODO
					}

					void remove(uint_fast16_t index) {
						while(1);
						// TODO
					}

					T* getHead() {
						while(1);
						// TODO
					}

					T* getTail() {
						while(1);
						// TODO
					}

					T* operator[] (uint_fast16_t i) {
						while(1);
						// TODO
					}

			};

		} /* namespace StaticMemory */
	} /* namespace Lists */
} /* namespace Util */


#endif /* APPLICATION_USER_LISTS_STATICMEMORY_LINKEDLIST_H_ */
