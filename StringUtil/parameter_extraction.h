/*
 * parameter_extraction.c
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module contains functions to extract parameters from strings.
 */
#ifndef APPLICATION_USER_STRING_FUNCTIONS_PARAMETER_EXTRACTION_H_
#define APPLICATION_USER_STRING_FUNCTIONS_PARAMETER_EXTRACTION_H_


#include <stdint-gcc.h>


namespace StringUtil {

	/**
	 * Determines the amount of parameters within a string.
	 *
	 * The parameters are expected to be separated by spaces. Quotation marks can
	 * be used to indicate continuous "parameter string".
	 * Furthermore, all characters following after a comment symbol (#, //, ;) will be ignored.
	 *
	 * @param parameter_list_string 	..	The (optionally null-terminated) parameter string.
	 * @param string_length           	..	Length of the parameter string. If -1, we iterate until null-terminator.
	 *
	 * @return                      	..	Amount of parameters
	 */
	uint_fast16_t get_parameter_count(const char *parameter_list_string, int_fast16_t string_length = -1);


	/**
	 * Extracts a desired from given parameter string. Surrounding spaces
	 * are being removed.
	 *
	 * The parameters are expected to be separated by spaces. Quotation marks can
	 * be used to indicate continuous "parameter string".
	 * Furthermore, all characters following after a comment symbol (#, //, ;) will be ignored.
	 *
	 * @param parameter_list_string        	..	The (optionally null-terminated) parameter string.
	 * @param parameter_list_string_length	..	Length of the parameter string. If -1, we iterate until null-terminator.
	 * @param index                       	..	Index of parameter we want to extract. Counting starts at 0.
	 * @param out_parameter_length        	..	Contains after successful extraction the length of the extracted parameter.
	 *
	 * @return                           	..	Points to the extracted parameter. Will be "nullptr" in case of error.
	 */
	extern char *get_parameter(const char *parameter_list_string, int_fast16_t parameter_list_string_length, uint_fast16_t index, uint_fast16_t *out_parameter_length);

} /* namespace StringUtil */

#endif /* APPLICATION_USER_STRING_FUNCTIONS_PARAMETER_EXTRACTION_H_ */
