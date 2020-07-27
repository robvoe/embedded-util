/*
 * parameter_extraction.c
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module contains functions to extract parameters from strings.
 */

#include "parameter_extraction.h"

#include "general_string_functions.h"

#include <stdint-gcc.h>
#include <string.h>


namespace StringUtil {

	uint_fast16_t get_parameter_count(const char *parameter_list_string, int_fast16_t string_length) {
		bool currently_within_quotation = false;  // Contains if we're currently within a continuous "parameter string".
		uint_fast16_t parameter_count;            // Counts the amount of parameters
		const char *str_ptr;

		if ( string_length < -1 )  return 0U;  // Invalid string length? Let's get out of here!
		if ( string_length == 0 )  return 0U;

		// Determine real string length
		uint_fast8_t xx = strlen( parameter_list_string );
		if ( string_length == -1 )
			string_length = xx;
		else {
			if ( xx < static_cast<uint_fast16_t>(string_length) )  string_length = xx;
		}

		// First pass - remove comments in the end
		str_ptr = parameter_list_string;
		for (uint_fast16_t i = 0; i<static_cast<uint_fast16_t>(string_length); i++) {
			if ( *str_ptr == '"' )
				currently_within_quotation = !currently_within_quotation;

			if ( currently_within_quotation == false ) {
				if ( *str_ptr == ';'  ||  *str_ptr == '#'  ||
					 ( *str_ptr == '/'  &&  i<(static_cast<uint_fast16_t>(string_length)-1)  &&  *(str_ptr+1U) == '/' )
				   ) {
					string_length = i;
					break;
				}
			}
			str_ptr++;
		}

		// Remove spaces ("trim")
		parameter_list_string = trim(parameter_list_string, false, string_length, &string_length);
		if ( string_length == 0U )  return 0U;
		if ( string_length == 1U )  return 1U;  // That's necessary to check for preceding space

		// Second pass - count parameters
		parameter_count = 1U;
		currently_within_quotation = false;

		for ( uint_fast16_t i = 1 /*Start at location 2, to enable check for preceding space*/; i<static_cast<uint_fast16_t>(string_length); i++ ) {
			if ( parameter_list_string[i] == '\"' )
				currently_within_quotation = !currently_within_quotation;
			if ( currently_within_quotation == false  &&  parameter_list_string[i]==' '  &&  parameter_list_string[i-1]!=' ' )
				parameter_count++;
		}

		return parameter_count;
	}

	char *get_parameter(const char *parameter_list_string, int_fast16_t parameter_list_string_length, uint_fast16_t index, uint_fast16_t *out_parameter_length) {
		bool currently_within_quotation = false; // Contains if we're currently within a continuous "parameter string".
		uint_fast16_t current_parameter_index;   // Counts the parameter index
		uint_fast16_t start_pos;                 // Contains start index of return string.
		const char *str_ptr;

		if ( parameter_list_string_length < -1 )  return 0U;  // Invalid string length? Let's get out of here!
		if ( parameter_list_string_length == 0 )  return 0U;

		// Determine real string length
		uint_fast8_t xx = strlen( parameter_list_string );
		if ( parameter_list_string_length == -1 )
			parameter_list_string_length = xx;
		else {
			if ( xx < static_cast<uint_fast16_t>(parameter_list_string_length) )  parameter_list_string_length = xx;
		}

		// First pass - remove comments in the end
		str_ptr = parameter_list_string;
		for (uint_fast16_t i = 0; i<static_cast<uint_fast16_t>(parameter_list_string_length); i++) {
			if ( *str_ptr == '"' )
				currently_within_quotation = !currently_within_quotation;

			if ( currently_within_quotation == false ) {
				if ( *str_ptr == ';'  ||  *str_ptr == '#'  ||
					 ( *str_ptr == '/'  &&  i<(static_cast<uint_fast16_t>(parameter_list_string_length)-1)  &&  *(str_ptr+1U) == '/' )
				   ) {
					parameter_list_string_length = i;
					break;
				}
			}
			str_ptr++;
		}

		// Remove surrounding spaces ("trim")
		parameter_list_string = trim( parameter_list_string, false, parameter_list_string_length, &parameter_list_string_length );
		if ( parameter_list_string_length == 0U )  return 0U;
		if ( parameter_list_string_length == 1U ) {
			*out_parameter_length = 1U;
			return (char *)parameter_list_string;  // That's necessary to check for preceding space
		}

		// Second pass - fetch the parameter
		start_pos = 0U;
		currently_within_quotation = false;
		current_parameter_index = 0U;

		for ( uint_fast16_t i = 1 /*Start at location 2, to enable check for preceding space*/; i<static_cast<uint_fast16_t>(parameter_list_string_length); i++ ) {
			if ( parameter_list_string[i] == '\"' )
				currently_within_quotation = !currently_within_quotation;
			if ( currently_within_quotation == false  &&  parameter_list_string[i]==' '  &&  parameter_list_string[i-1]!=' ' ) {
				current_parameter_index++;
				if ( current_parameter_index == index )  start_pos = i;
				if ( current_parameter_index > index ) {
					int_fast16_t parameter_length = i - start_pos;
					char *ret_val = trim( (const char *)&parameter_list_string[start_pos], false, parameter_length, &parameter_length );
					if ( *ret_val == '\"' ) {
						ret_val++;
						parameter_length--;
					}
					if ( ret_val[parameter_length] == '\"' ) {
						parameter_length--;
					}
					*out_parameter_length = parameter_length;
					return ret_val;
				}
			}
		}

		// If the desired parameter is in the last position (with respect to the string), it needs a special treatment..
		if ( current_parameter_index == index ) {
			int_fast16_t parameter_length = parameter_list_string_length - start_pos;
			char *ret_val = trim( (const char *)&parameter_list_string[start_pos], false, parameter_length, &parameter_length );
			if ( *ret_val == '\"' ) {
				ret_val++;
				parameter_length--;
			}
			if ( ret_val[parameter_length] == '\"' ) {
				parameter_length--;
			}
			*out_parameter_length = parameter_length;
			return ret_val;
		}


		// Didn't find parameter. Return an error.
		*out_parameter_length = 0;
		return 0U;
	}

} /* namespace StringUtil */
