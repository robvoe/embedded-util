/*
 * Internal.h
 *
 *  Created on: 21.01.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *      Provides internally used functionality. This module should not be included externally.
 */
#ifndef APPLICATION_USER_STM32PERSISTENCE_INTERNAL_H_
#define APPLICATION_USER_STM32PERSISTENCE_INTERNAL_H_

#include <stdint-gcc.h>
#include <array>

#include "Definitions.h"


namespace Util {
	namespace Stm32 {
		namespace Persistence {

			template <typename T> class PersistentStorage;


			class Internal {
				private:
					/// Static variables that keep track of existing instances of class @see PersistantStorage
					static std::array<PersistentStorage<void*>*, Definitions::MaxInstanceCount> Instances;
					static std::uint_fast8_t                                                    InstanceCount;
					static std::size_t                                                          InstancesByteSum;         	///< Zählt die durch Instanzen bereits genutzten Bytes

				public:
					static bool                                                                 CrcErrorOccurred;         	///< Protokolliert, ob bisher CRC-Fehler beim Datenlesen aufgetreten sind.


					/**
					 * Registers an instance of class @see PersistentStorage to the central management.
					 *
					 * @remark This function gets automatically called of constructor or class @see PersistentStorage
					 * @remark Determines the UUID for each instance of @see PersistentStorage
					 */
					static void registerPersistentStorageInstance( PersistentStorage<void*> *persistentStorage );

					/**
					 * Loads the most recent version from Flash to RAM.
					 *
					 * @param copyTo  	..	Reference to the @see UnderlyingMemoryBlock the data shall be copied to.
					 *
					 * @return           	Returns if valid data were found and were written to @see copyTo
					 */
					static bool loadMostRecentImageFromFlash( UnderlyingMemoryBlock<void*> &copyTo );

					#ifndef STM32PERSISTENCE__DISABLE_WEAR_LEVELING
					/**
					 * Writes a single instance of @see PersistenceStorage to Flash.
					 *
					 * @param persistentStorage	..	The instance that shall be written to Flash. Must not be nullptr.
					 *
					 * @return                 	  	Returns the result of write process.
					 *                               - Success                     	..	Instance was successfully written.
					 *                               - Success_AllStoragesWereSaved	..	All instances of @see PersistenceStorage were written in "one go". The application should
					 *                                                            	  	avoid writing other instances afterwards.
					 *                               - Error                      	..	There was an error.
					 */
					static Persistence::SaveSingleStorageResult saveSinglePersistentStorage( PersistentStorage<void*> *persistentStorage );
					#endif

					/**
					 * Writes all known instances of @see PersistentStorage
					 *
					 * @param startFromBeginning	..	Denotes which Flash address writing should start from.
					 *                            	  	 - true : Writing starts from "beginning of persistence memory". Wear leveling won't be used here.
					 *                            	  	 - false: Writing starts right after last found memory cell. Wear leveling will be used here.
					 *
					 * @return  	             	  	Returns if writing was successful.
					 */
					static bool saveAllPersistentStorages( bool startFromBeginning );


				private:
					/**
					 * Calculates CRC-32 over an @see UnderlyingMemoryBlock and writes it to a certain address in Flash.
					 *
					 * @param dataSource          	..	Source block
					 * @param destinationAddress  	..	Flash address where to put the block.
					 *
					 * @return                    	  	Returns if writing was successful.
					 */
					static bool writeUnderlyingMemoryBlockToFlash( UnderlyingMemoryBlock<void*> &dataSource, void *destinationAddress );

					/**
					 * Prepares, if necessary, the next Flash page for rewriting.
					 *
					 * @param dataBlockToBeWritten	..	Data block we subsequently want to write to Flash.
					 * @param addressOfNextWrite 	..	Address we want to write the data block to.
					 *
					 * @return                    	  	Returns if flash page was erased successfully (if necessary).
					 */
					static bool eraseNextFlashPageIfNecessary( UnderlyingMemoryBlock<void*> &dataBlockToBeWritten, void *addressOfNextWrite );
			};

		} /* namespace Persistence */
	} /* namespace Stm32 */
} /* namespace Util */



#endif /* APPLICATION_USER_STM32PERSISTENCE_INTERNAL_H_ */
