/*
 * Definitions.h
 *
 *  Created on: 21.01.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Internally used definitions module. Contains (partially overridable) constants and types.
 */
#ifndef APPLICATION_USER_STM32PERSISTENCE_DEFINITIONS_H_
#define APPLICATION_USER_STM32PERSISTENCE_DEFINITIONS_H_


#include <stdint-gcc.h>
#include <stddef.h>

#include <IncludeStmHal.h>


namespace Util {
	namespace Stm32 {
		namespace Persistence {


			class Definitions {
				friend class Internal;
				private:
					enum StartAddresses : uintptr_t {
						EndOfFlash = 0
					};

				public:
					/// Defines the amount of used flash pages the persistence module uses.
					#ifdef STM32PERSISTENCE__USED_FLASH_PAGE_COUNT
						static constexpr size_t ReservedFlashPageCount = STM32PERSISTENCE__USED_FLASH_PAGE_COUNT;
					#else
						static constexpr size_t ReservedFlashPageCount = 1;
					#endif

					/// Defines the maximum number of instances of @see PersistentStorage
					#ifdef STM32PERSISTENCE__MAX_INSTANCE_COUNT
						static constexpr size_t MaxInstanceCount = STM32PERSISTENCE__MAX_INSTANCE_COUNT;
					#else
						static constexpr size_t MaxInstanceCount = 10;
					#endif

					/// Defines the base address for persistence module.
					#ifdef STM32PERSISTENCE__START_ADDRESS
						static constexpr uintptr_t StartAddress = STM32PERSISTENCE__START_ADDRESS;
					#else
						static constexpr uintptr_t StartAddress = EndOfFlash;
					#endif

					/// Defines if wear leveling algorithm should be used.
					#ifdef STM32PERSISTENCE__DISABLE_WEAR_LEVELING
						static constexpr bool WearLevelingEnabled = false;
					#else
						static constexpr bool WearLevelingEnabled = true;
					#endif


					/// Further definitions, determined from values above.   This happens during runtime within file @see Definitions.cpp
					static constexpr size_t   ReservedFlashBytes = ReservedFlashPageCount * FLASH_PAGE_SIZE;
					static const uint_fast8_t FlashPageCount; 	///< Defines the number of overall flash pages of that µC.
					static const uint32_t     BaseAddress;    	///< The base address that's used by persistence module.
					static const uint32_t     HighestAddress; 	///< Highest possible flash address that will be used for persistence.
			} /*class*/;




			/// Defines a data block that's used for putting data to RAM and flash.
			template <typename T>
			struct UnderlyingMemoryBlock {
				struct {
					uint32_t crc32 : 32;
					uint8_t  Uuid : 8;            	///< Unique number of @see PersistentStorage. Determined internally by instance counting within constructor.
					uint8_t  UserDataSize : 8;    	///< Length of @see UserData. Counted as full 32 bit words.
					uint8_t  WholeStructSize : 8; 	///< Length of complete structure (@see UnderlyingMemoryBlock). Counted as full 32 bit words.
				} Header;
				T UserData __attribute__((aligned (8))); // --> Has to be aligned to 64 bytes (maximum ARM memory alignment). Otherwise, internal data handling problems might arise.
				#if !defined(FLASH_TYPEPROGRAM_WORD)     // --> Sometimes, the STM32 HAL does not support word-wise writing (32 bits). Instead, it only supports double-word-wise writing (64 bits). In this case, we need to introduce a padding.
					uint32_t doubleword_padding __attribute__((aligned (4)));
				#else
					uint8_t padding[ (sizeof(T) % sizeof(uint32_t))  ?  sizeof(uint32_t) - (sizeof(T) % sizeof(uint32_t))  :  0 ];
				#endif
			};


			/// This return type is important when saving instances of @see PersistentStorage
			enum class SaveSingleStorageResult {
				Success,                     //!< Success
				Success_AllStoragesWereSaved,//!< Success_AllStoragesWereSaved
				Error                        //!< Error
			};


		} /* namespace Persistence */
	} /* namespace Stm32 */
} /* namespace Util */



#endif /* APPLICATION_USER_STM32PERSISTENCE_DEFINITIONS_H_ */
