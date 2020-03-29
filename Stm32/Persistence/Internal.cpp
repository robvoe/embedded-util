/*
 * Internal.cpp
 *
 *  Created on: 21.01.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *      Provides internally used functionality. This module should not be included externally.
 */
#include <stdint-gcc.h>
#include <string.h>
#include <array>

#include <IncludeStmHal.h>

#include "Definitions.h"
#include "Internal.h"
#include "PersistentStorage.h"

extern "C" {
	#include "CRC/crc32.h"
}


namespace Util {
	namespace Stm32 {
		namespace Persistence {

			/// Static variables that keep track of existing instances of class @see PersistantStorage
			std::array<PersistentStorage<void*>*, Definitions::MaxInstanceCount> Internal::Instances;
			std::uint_fast8_t                                                    Internal::InstanceCount = 0;
			std::size_t                                                          Internal::InstancesByteSum = 0;
			bool                                                                 Internal::CrcErrorOccurred = false;



			/**
			 * Checks if a @see PersistentStorage's CRC matches the CRC value in header field.
			 */
			static bool hasValidCrc( const UnderlyingMemoryBlock<void*> &underlyingMemoryBlock ) {
				uint32_t crc32 = calculateCrc32( (uint32_t*)&underlyingMemoryBlock.UserData, underlyingMemoryBlock.Header.UserDataSize );
				if ( crc32 == underlyingMemoryBlock.Header.crc32 )
					return true;
				else {
					for (uint32_t i = 0; i<underlyingMemoryBlock.Header.WholeStructSize; i++) {
						uint32_t *address = (uint32_t*)&underlyingMemoryBlock;
						if ( *address != UINT32_C(0xFFFFFFFF) )   Internal::CrcErrorOccurred = true;
					}
					return false;
				}
			}


			/**
			 * This class keeps track of the next unused Flash memory address.
			 */
			static class {
				private:
					std::uint32_t NextUnusedAddress = Definitions::BaseAddress;
				public:
					void set( void *address) { NextUnusedAddress = (uint32_t)address; }
					void set( std::uint32_t address) { NextUnusedAddress = address; }
					void *get(void) {
						while(1) {
							if ( NextUnusedAddress + sizeof(UnderlyingMemoryBlock<void*>*)  >  Definitions::HighestAddress + 1 )  break;
							auto *iterator = (UnderlyingMemoryBlock<void*>*)NextUnusedAddress;
							if ( NextUnusedAddress + iterator->Header.WholeStructSize*sizeof(uint32_t)  >  Definitions::HighestAddress + 1 )  break;
							if ( (uint32_t)(&(iterator->UserData)) + iterator->Header.UserDataSize*sizeof(uint32_t)  >  Definitions::HighestAddress + 1 )  break;
							if ( !iterator->Header.UserDataSize || !iterator->Header.WholeStructSize )  break;
							if ( !hasValidCrc( *iterator ) )  break;
							NextUnusedAddress += iterator->Header.WholeStructSize*sizeof(uint32_t);
						}
						return (void*)NextUnusedAddress;
					}
			} NextUnusedFlashAddress;



			/**
			 * Registers an instance of class @see PersistentStorage to the central management.
			 *
			 * @remark This function gets automatically called of constructor or class @see PersistentStorage
			 * @remark Determines the UUID for each instance of @see PersistentStorage
			 */
			void Internal::registerPersistentStorageInstance( PersistentStorage<void*> *persistentStorage ) {
				InstanceCount++;
				if ( InstanceCount > Definitions::MaxInstanceCount )  InstanceCount = Definitions::MaxInstanceCount;

				// Eintragen der Klasseninstanz   UND   ermitteln der zugehörigen UUID
				persistentStorage->RamContents.Header.Uuid = InstanceCount;
				Instances[InstanceCount-1] = persistentStorage;

				// Prüfen, ob sämtliche Instanzen in die verfügbaren Flash-Pages passen
				InstancesByteSum += persistentStorage->RamContents.Header.WholeStructSize * sizeof(uint32_t);
				if ( InstancesByteSum > Definitions::ReservedFlashBytes )  while(1);  ///<--------- If the program is stuck here, that's because the data do not fit into reserved Flash pages.  The value of @see Definitions::ReservedFlashPageCount should be increased!
			}


			/**
			 * Loads the most recent version from Flash to RAM.
			 *
			 * @param copyTo  	..	Reference to the @see UnderlyingMemoryBlock the data shall be copied to.
			 *
			 * @return           	Returns if valid data were found and were written to @see copyTo
			 */
			bool Internal::loadMostRecentImageFromFlash( UnderlyingMemoryBlock<void*> &copyTo ) {
				UnderlyingMemoryBlock<void*> *mostRecentImage = nullptr;
				UnderlyingMemoryBlock<void*> *iterator = (UnderlyingMemoryBlock<void*>*)Definitions::BaseAddress;

				while(1) {
					if ( (uint32_t)iterator + sizeof(UnderlyingMemoryBlock<void*>*)  >  Definitions::HighestAddress + 1 )  break;
					if ( (uint32_t)iterator + iterator->Header.WholeStructSize*sizeof(uint32_t)  >  Definitions::HighestAddress + 1 )  break;
					if ( (uint32_t)(&(iterator->UserData)) + iterator->Header.UserDataSize*sizeof(uint32_t)  >  Definitions::HighestAddress + 1 )  break;
					if ( !iterator->Header.UserDataSize || !iterator->Header.WholeStructSize )  break;
					if ( !hasValidCrc( *iterator ) )  break;
					if ( iterator->Header.Uuid == copyTo.Header.Uuid  &&  iterator->Header.UserDataSize == copyTo.Header.UserDataSize )  mostRecentImage = iterator;
					iterator = (UnderlyingMemoryBlock<void*>*)(  (uint32_t)iterator + iterator->Header.WholeStructSize*sizeof(uint32_t)  );
					NextUnusedFlashAddress.set( iterator );
				}
				if ( mostRecentImage == nullptr )  return false;

				memcpy( &copyTo, mostRecentImage, mostRecentImage->Header.WholeStructSize*sizeof(uint32_t) );
				return true;
			}

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
			Persistence::SaveSingleStorageResult Internal::saveSinglePersistentStorage( PersistentStorage<void*> *persistentStorage ) {
				// Is the instance in undefined state?
				if ( !persistentStorage->isInitialized )   return  SaveSingleStorageResult::Success;

				// Fetch base address from where we start writing
				void *writeAddress = NextUnusedFlashAddress.get();

				// Have there been CRC errors during reading? If so, the whole Flash must be rewritten now. Otherwise there might occur hardware errors when writing.
				if ( CrcErrorOccurred ) {
					bool result = saveAllPersistentStorages( true );
					return result ?  SaveSingleStorageResult::Success_AllStoragesWereSaved  :  SaveSingleStorageResult::Error;
				}

				// Does the block fit to remaining free Flash?
				if ( (uint32_t)writeAddress + persistentStorage->RamContents.Header.WholeStructSize*sizeof(uint32_t)  >  Definitions::HighestAddress + 1 ) {
					bool result = saveAllPersistentStorages( true );
					return result ?  SaveSingleStorageResult::Success_AllStoragesWereSaved  :  SaveSingleStorageResult::Error;
				}

				// Save the data
				bool result = eraseNextFlashPageIfNecessary( persistentStorage->RamContents, writeAddress );
				if ( !result )  return SaveSingleStorageResult::Error;
				result = writeUnderlyingMemoryBlockToFlash( persistentStorage->RamContents, writeAddress );
				if ( !result )  return SaveSingleStorageResult::Error;

				return SaveSingleStorageResult::Success;
			}
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
			bool Internal::saveAllPersistentStorages( bool startFromBeginning ) {
				#ifdef STM32PERSISTENCE__DISABLE_WEAR_LEVELING
				startFromBeginning = true;
				#endif
				// Fetch the base address we start to write from.
				void *writeAddress = startFromBeginning ? (void*)Definitions::BaseAddress : NextUnusedFlashAddress.get();

				if ( !startFromBeginning ) {
					// Have there been CRC errors during reading? If so, the whole Flash must be rewritten now. Otherwise there might occur hardware errors when writing.
					if ( CrcErrorOccurred ) {
						return saveAllPersistentStorages( true );
					}
					// Check if remaining free storage is enough to fit all instances of @see PersistentStorage. If no, we start from the beginning.
					if ( (uint32_t)writeAddress + InstancesByteSum  >  Definitions::HighestAddress + 1 ) {
						return saveAllPersistentStorages( true );
					}
				}

				// Now iterate over all known instances of @see PersistentStorage. If there are no valid data in RAM yet, load them from Flash. Otherwise we risk data loss.
				if ( startFromBeginning ) {
					for ( uint32_t i = 0; i<InstanceCount; i++ ) {
						// Fetch the instance of @see PersistentStorage
						auto *persistentStorage = (PersistentStorage<void*>*)Instances[i];
						auto &underlyingMemoryBlock = persistentStorage->RamContents;
						// If instance data are not present in RAM yet, do load them now!
						if ( !persistentStorage->isInitialized ) {
							bool foundExistingImage = loadMostRecentImageFromFlash(underlyingMemoryBlock);
							if ( foundExistingImage )  persistentStorage->isInitialized = true;
						}
					}
					CrcErrorOccurred = false;
				}  /* if ( startFromBeginning ) */

				// Now we iterate over all known instances of @see PersistentStorage and write them to Flash
				for ( uint32_t i = 0; i<InstanceCount; i++ ) {
					// Fetch the instance of @see PersistentStorage
					auto *persistentStorage = (PersistentStorage<void*>*)Instances[i];
					auto &underlyingMemoryBlock = persistentStorage->RamContents;
					if ( !persistentStorage->isInitialized ) continue; // --> Die Instanz beinhaltet noch keine gültigen Daten. Dann muss sie auch nicht in den Flash geschrieben werden!
					// Save its data
					bool result = eraseNextFlashPageIfNecessary( underlyingMemoryBlock, writeAddress );
					if ( !result )  return false;
					result = writeUnderlyingMemoryBlockToFlash( underlyingMemoryBlock, writeAddress );
					if ( !result )  return false;
					// Determine next free address in Flash
					writeAddress = (void*)(  (uint32_t)writeAddress + (underlyingMemoryBlock.Header.WholeStructSize * sizeof(uint32_t))  );
				}
				return true;
			}

			/**
			 * Calculates CRC-32 over an @see UnderlyingMemoryBlock and writes it to a certain address in Flash.
			 *
			 * @param dataSource          	..	Source block
			 * @param destinationAddress  	..	Flash address where to put the block.
			 *
			 * @return                    	  	Returns if writing was successful.
			 */
			bool Internal::writeUnderlyingMemoryBlockToFlash( UnderlyingMemoryBlock<void*> &dataSource, void *destinationAddress ) {
				// Have there been CRC errors during reading? If so, all further processing might be error prone. That's because there might occur hardware errors when writing.
				if ( CrcErrorOccurred )  return false;  // <-- This condition should never be true, since checked by all calling functions.

				// Calculate CRC-32
				uint32_t crc32 = calculateCrc32( (uint32_t*)&dataSource.UserData, dataSource.Header.UserDataSize );
				dataSource.Header.crc32 = crc32;
				// Write to Flash
				HAL_FLASH_Unlock();
				HAL_StatusTypeDef result;
				#if defined(FLASH_TYPEPROGRAM_WORD)
					for ( uint32_t address_word_offset = 0; address_word_offset<dataSource.Header.WholeStructSize; address_word_offset++ ) {
						result = HAL_FLASH_Program( FLASH_TYPEPROGRAM_WORD, (uint32_t)destinationAddress+address_word_offset*sizeof(uint32_t), ((uint32_t *)&dataSource)[address_word_offset] );
						if ( result != HAL_StatusTypeDef::HAL_OK )  {  HAL_FLASH_Lock();  return false;  }
					}
				#elif defined(FLASH_TYPEPROGRAM_DOUBLEWORD)
					for ( uint32_t address_doubleword_offset = 0; address_doubleword_offset<dataSource.Header.WholeStructSize/2; address_doubleword_offset++ ) {
						result = HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)destinationAddress+address_doubleword_offset*sizeof(uint64_t), ((uint64_t *)&dataSource)[address_doubleword_offset] );
						if ( result != HAL_StatusTypeDef::HAL_OK )  {  HAL_FLASH_Lock();  return false;  }
					}
				#else
					#error Need to implement an alternative flash write routine!
				#endif
				HAL_FLASH_Lock();
				// Determine next free address in Flash
				NextUnusedFlashAddress.set(  (uint32_t)destinationAddress + dataSource.Header.WholeStructSize * sizeof(uint32_t)  );
				return true;
			}

			/**
			 * Prepares, if necessary, the next Flash page for rewriting.
			 *
			 * @param dataBlockToBeWritten	..	Data block we subsequently want to write to Flash.
			 * @param addressOfNextWrite 	..	Address we want to write the data block to.
			 *
			 * @return                    	  	Returns if flash page was erased successfully (if necessary).
			 */
			bool Internal::eraseNextFlashPageIfNecessary( UnderlyingMemoryBlock<void*> &dataBlockToBeWritten, void *addressOfNextWrite ) {
				uint32_t relativeAddress = (uint32_t)addressOfNextWrite - FLASH_BASE;
				uint32_t flashPageCounter = 0;
				while (relativeAddress>FLASH_PAGE_SIZE) {
					relativeAddress -= FLASH_PAGE_SIZE;
					flashPageCounter++;
				}
				// Now we have the posisition of the block we want to write relative to a Flash page (0 <= relativeAddress <= FLASH_PAGE_SIZE). Using this value we check if erasing is necessary.
				bool erasingNecessary = false;
				if ( relativeAddress == 0 )  erasingNecessary = true;
				if ( relativeAddress + dataBlockToBeWritten.Header.WholeStructSize*sizeof(uint32_t)  >  FLASH_PAGE_SIZE )  { erasingNecessary = true; flashPageCounter+=1; }
				// Now erase Flash page if necessary.
				if ( erasingNecessary ) {
					HAL_FLASH_Unlock();
					FLASH_EraseInitTypeDef EraseInitStruct;
					uint32_t PageError;
					#if defined(STM32L0) || defined(STMF3)
						EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
						EraseInitStruct.PageAddress = FLASH_BASE + (flashPageCounter * FLASH_PAGE_SIZE);
						EraseInitStruct.NbPages     = 1U;
					#elif defined(STM32L4)
						EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
						EraseInitStruct.Page = flashPageCounter;
						EraseInitStruct.NbPages     = 1U;
					#else
						#error Probably the correct Flash Write routine needs to be chosen/implemented!
					#endif

					HAL_StatusTypeDef result = HAL_FLASHEx_Erase( &EraseInitStruct, &PageError );
					if ( result != HAL_StatusTypeDef::HAL_OK ) {
						HAL_FLASH_Lock();
						return false;
					}
					HAL_FLASH_Lock();
				}
				return true;
			}

		} /* namespace Persistence */
	} /* namespace Stm32 */
} /* namespace Util */

