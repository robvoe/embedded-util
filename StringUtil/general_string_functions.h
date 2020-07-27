/*
 * general_string_functions.c
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Contains general string functions.
 */
#ifndef APPLICATION_USER_STRING_FUNCTIONS_GENERAL_STRING_FUNCTIONS_H_
#define APPLICATION_USER_STRING_FUNCTIONS_GENERAL_STRING_FUNCTIONS_H_

#include <stdint-gcc.h>


namespace StringUtil {

	/**
	 * Removes preceding and trailing spaces of a string.
	 *
	 * @param input_string                   	..	Input string, optionally null-terminated.
	 * @param insert_trailing_null_terminator  	..	Determines whether to insert a null-terminator after the end.
	 * @param string_length                   	..	Length of input string. If -1, we iterate until null-terminator.
	 * @param out_string_length                 ..  Pure output parameter. Contains the length of the resulting cleaned string, if desired.
	 *
	 * @return                                	..	Points to the cleaned-up string. Will be nullptr in case of an error.
	 */
	extern char *trim(const char *input_string, bool insert_trailing_null_terminator = true, int_fast16_t string_length = -1, int_fast16_t *out_string_length = nullptr );

} /* namespace StringUtil */


#endif /* APPLICATION_USER_STRING_FUNCTIONS_GENERAL_STRING_FUNCTIONS_H_ */
