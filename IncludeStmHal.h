/*
 * IncludeStmHal.h
 *
 *  Created on: 21.01.2018
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Uniform interface to include the STM32 Hardware Abstraction Layer (Stm32Cube).
 */
#ifndef APPLICATION_USER_STM32PERSISTENCE_INCLUDESTMHAL_H_
#define APPLICATION_USER_STM32PERSISTENCE_INCLUDESTMHAL_H_


#if defined(STM32L051xx)
	#include <stm32l0xx.h>
	#define STML0
#elif defined(STM32F3xx) || defined(STM32F302x8) || defined(STM32F301x8) || defined(STM32F303xE)
	#include <stm32f3xx_hal.h>
	#define STMF3
#elif defined(STM32L431xx) || defined(STM32L412xx)
	#include "stm32l4xx_hal.h"
	#define STML4
#else
	#error No proper STM32 HAL interface found!
#endif


#endif /* APPLICATION_USER_STM32PERSISTENCE_INCLUDESTMHAL_H_ */
