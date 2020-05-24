/*
 * PersistentStorage.h
 *
 *  Created on: 29.11.2017
 *      Author: Robert Voelckner
 *
 *  Description:
 *  	This class manages data structures and their persistence in emulated
 *  	EEPROM. Hereby, it uses wear leveling as much as possible.
 *
 *  	By default, it writes the contents of all class instances to the last
 *  	pages of Flash memory. For further settings, check "Definitions.h".
 *
 *  	Further information:
 *  	- no usage of dynamic (heap) memory,
 *  	- data members may be accessed using the "->" operator.  Example: mem1->a = 10;
 */
#ifndef APPLICATION_USER_PERSISTENCEMANAGER_STM32_STM32PERSISTENCEMANAGER_H_
#define APPLICATION_USER_PERSISTENCEMANAGER_STM32_STM32PERSISTENCEMANAGER_H_

#include "Definitions.h"
#include "Internal.h"


#include <stdint-gcc.h>
#include <string.h>
#include <type_traits>


namespace Util {
	namespace Stm32 {
		namespace Persistence {

			/// This class represents a persistent storage. It may be instanciated as often as desired. Calling its destructor is not allowed.
			template <typename T> class PersistentStorage {
				friend class Internal;

				public:
					/// Definition of function pointer that (optionally) is used to obtain default values.
					typedef void (*RestoreToDefaultsFunctionPointer)(T &reference);



				private:
					/// Internal definitions
					enum class DefaultValueGenerationMethods {
						CopyDefaultUserDataSet,
						CallFunctionPointer,
						ZeroFill
					};

					/// Some constants
					const void *DefaultUserDataSet;                               	///< Points to   a) data structure containing defaults   or    b) function to obtain default values.
					const DefaultValueGenerationMethods DefaultValueGeneration;  	///< Denotes how we obtain default for the field @see UserData

					/// Runtime variables
					bool isInitialized = false;           	///< Denotes if RAM content was already filled with data from Flash. This is done when reading from RAM for the first time (--> lazy loading).
					UnderlyingMemoryBlock<T>  RamContents;	///< Will be filled with data from Flash when read-accessed by application for the first time.


					/**
					 * Internal constructor. Is called by public constructors to perform first value initializations.
					 */
					PersistentStorage( const void *defaultUserDataSet, DefaultValueGenerationMethods defaultValueGeneration ) : DefaultUserDataSet(defaultUserDataSet), DefaultValueGeneration(defaultValueGeneration) {
						static_assert( std::is_pod<T>::value, "T must be Plain-Old-Data!" );
						static_assert( sizeof(T)<=(UINT8_MAX*sizeof(uint32_t)), "Size of T must be <=1000!" );

						// Initialisierungen durchf�hren
						RamContents.Header.WholeStructSize = sizeof(UnderlyingMemoryBlock<T>)/sizeof(uint32_t);
						RamContents.Header.UserDataSize = sizeof(T)/sizeof(uint32_t)  +  (/*The following "rounds" to full 32 bit words*/  sizeof(T)%sizeof(uint32_t) ? 1 : 0);
						Internal::registerPersistentStorageInstance( (PersistentStorage<void*>*)this );
					}



				public:
					/** Forbids the copy constructor **/
					PersistentStorage( PersistentStorage<T> const & ) = delete;

					/**
					 * Public constructor.
					 *
					 * @remark For obtaining defaults, we will use a DefaultDataSet.
					 * @remark The instance may be used right after constructing. Nothing else must be done by user!
					 */
					PersistentStorage(const T& defaultDataSet) : PersistentStorage(&defaultDataSet, DefaultValueGenerationMethods::CopyDefaultUserDataSet) { }

					/**
					 * Public constructor.
					 *
					 * @remark For obtaining defaults, we will use a function which is provided by the application.
					 * @remark The instance may be used right after constructing. Nothing else must be done by user!
					 */
					PersistentStorage(RestoreToDefaultsFunctionPointer functionPointer) : PersistentStorage((const void*)functionPointer, DefaultValueGenerationMethods::CallFunctionPointer) { }

					/**
					 * Public constructor.
					 *
					 * @remark For obtaining defaults, we fill user data space with zeroes.
					 * @remark The instance may be used right after constructing. Nothing else must be done by user!
					 */
					PersistentStorage(void) : PersistentStorage(nullptr, DefaultValueGenerationMethods::ZeroFill) { }

					/**
					 * Loads Flash content to RAM.
					 * If CRC is invalid, default data set will be loaded instead.
					 */
					void reloadFlashToRam(void) {
						bool imageFoundAndLoaded = Internal::loadMostRecentImageFromFlash( (UnderlyingMemoryBlock<void*>&)RamContents );
						if ( !imageFoundAndLoaded )   restoreDefaultDataSet();
						isInitialized = true;
					}

					/**
					 * Loads the default data set to RAM.
					 */
					void restoreDefaultDataSet(void) {
						switch ( DefaultValueGeneration ) {
							case DefaultValueGenerationMethods::CallFunctionPointer:
								((RestoreToDefaultsFunctionPointer)DefaultUserDataSet)( RamContents.UserData );
								break;
							case DefaultValueGenerationMethods::CopyDefaultUserDataSet:
								RamContents.UserData = *(T*)DefaultUserDataSet;
								break;
							case DefaultValueGenerationMethods::ZeroFill:
							default:
								memset( &RamContents.UserData, 0, sizeof(T) );
								break;
						}
						isInitialized = true;
					}

					/**
					 * Writes this instance of @see PersistenceStorage to Flash.
					 *
					 * @remark It might occur that all known instances will be written to Flash in "one go". The subsequent writing of
					 *         other instances should be avoided since they were already written.
					 *
					 * @return	Returns the result of write process.
					 *        	  - Success                     	..	Instance was successfully written.
					 *        	  - Success_AllStoragesWereSaved	..	All instances of @see PersistenceStorage were written in "one go". The application should
					 *                                                  avoid writing other instances afterwards.
					 *            - Error                         	..	There was an error.
					 */
					Persistence::SaveSingleStorageResult saveToFlash(void) {
						if ( !isInitialized )  return SaveSingleStorageResult::Success;  // --> Es gibt nix zu speichern? Dann ist die Arbeit bereits getan..
						#ifdef STM32PERSISTENCE__DISABLE_WEAR_LEVELING
							bool result = Internal::saveAllPersistentStorages( true );
							return result  ?  SaveSingleStorageResult::Success_AllStoragesWereSaved  :  SaveSingleStorageResult::Error;
						#else
							return Internal::saveSinglePersistentStorage( (PersistentStorage<void*>*)this );
						#endif
					}

					/**
					 * Writes all known instances of @see PersistentStorage
					 *
					 * @return  	             	  	Returns if writing was successful.
					 *
					 * @return   	..	Returns if writing was successful.
					 *            	  	 -  true: Writing was successful.
					 *            	  	 - false: An error occurred.
					 */
					bool saveAllPersistentStorages(void) {
						//if ( !isInitialized )  reloadFlashToRam();  ---->  Muss hier nicht erfolgen, da es in der Funktion "saveAllPersistentStorages()" f�r alle PersistentStorages durchgef�hrt wird
						#ifdef STM32PERSISTENCE__DISABLE_WEAR_LEVELING
							return Internal::saveAllPersistentStorages( true );
						#else
							return Internal::saveAllPersistentStorages( false );
						#endif
					}

					/**
					 * Access to data members of T.
					 *
					 * Application example:  auto value = persistentStorage->field1;
					 */
					T* operator->() {
						if ( !isInitialized )  reloadFlashToRam();
						return &RamContents.UserData;
					}

					/**
					 * "Address of" conversion operator. Returns a pointer to user data.
					 *
					 * Application example:  auto *ptr = &persistentStorage;
					 */
					T* operator&() {
						if ( !isInitialized )  reloadFlashToRam();
						return &RamContents.UserData;
					}

					/**
					 * Assignment operator.
					 */
					PersistentStorage<T>& operator= (PersistentStorage<T> &rhs) {
						if ( *this != rhs ) {
							if ( !rhs.isInitialized )  rhs.reloadFlashToRam();
							RamContents.UserData = rhs.RamContents.UserData;
							isInitialized = true;  // Prevents lazy loading (of stale memory state) at next read access.
						}
						return *this;
					}

					/**
					 * Assignment operator.
					 */
					PersistentStorage<T>& operator= (T const &rhs) {
						RamContents.UserData = rhs;
						isInitialized = true;  // Prevents lazy loading (of stale memory state) at next read access.
						return *this;
					}

					/**
					 * Destructor.
					 */
					~PersistentStorage() {
						while(1);  // ---> A PersistentStorage should exist during whole runtime and must never be destroyed!
					}

			} /*class PersistentStorage*/;

		} /* namespace Persistence */
	} /* namespace Stm32 */
} /* namespace Util */



#endif /* APPLICATION_USER_PERSISTENCEMANAGER_STM32_STM32PERSISTENCEMANAGER_H_ */
