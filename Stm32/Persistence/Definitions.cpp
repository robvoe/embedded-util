/*
 * Definitions.cpp
 *
 *  Created on: 21.01.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Internally used definitions module. Contains (partially overridable) constants and types.
 */
#include <IncludeStmHal.h>
#include "Definitions.h"

namespace Util {
	namespace Stm32 {
		namespace Persistence {

			const uint_fast8_t Definitions::FlashPageCount = (*(uint16_t*)FLASH_SIZE_DATA_REGISTER) / (FLASH_PAGE_SIZE/1024);
			const uint32_t     Definitions::BaseAddress = (Definitions::StartAddress==EndOfFlash)  ?
																		(FLASH_BASE + (FlashPageCount-ReservedFlashPageCount)*FLASH_PAGE_SIZE)  :
																		(Definitions::StartAddress);
			const uint32_t     Definitions::HighestAddress = Definitions::BaseAddress + Definitions::ReservedFlashBytes - 1;

		} /* namespace Persistence */
	} /* namespace Stm32 */
} /* namespace Util */
