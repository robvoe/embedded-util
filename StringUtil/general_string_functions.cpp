/*
 * general_string_functions.c
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    Contains general string functions.
 */

#include "general_string_functions.h"

#include <stdint-gcc.h>
#include <string.h>

namespace StringUtil {

	char *trim(const char *input_string, bool insert_trailing_null_terminator, int_fast16_t string_length, int_fast16_t *out_string_length ) {

		if ( string_length < -1 )  return 0U;  // Invalid string length? Let's get out of here!
		if ( string_length == 0 )  return (char *)input_string;

		// Determine real string length
		uint_fast16_t xx = strlen( input_string );
		if ( string_length == -1 )
			string_length = xx;
		else {
			if ( xx < static_cast<uint_fast16_t>(string_length) )  string_length = xx;
		}

		// Remove preceding spaces
		const char *retVal = input_string;
		while ( *retVal == ' '  &&  string_length > 0 ) {
			retVal++;
			string_length--;
		}

		if ( string_length == 0 ) {
			if ( insert_trailing_null_terminator )   *(char *)(retVal) = 0U;
			if ( out_string_length != nullptr )  *out_string_length = string_length;
			return (char *)retVal;
		}

		// Remove trailing spaces
		const char *str_ptr = &retVal[string_length-1];  // Jumps to last position of string
		while ( *str_ptr == ' '  &&  string_length > 0 ) {
			str_ptr--;
			string_length--;
		}

		// Now, optionally insert null-terminator
		if ( insert_trailing_null_terminator ) {
			if ( string_length > 0 )
				*(char *)(str_ptr+1U) = 0U;
			else /*if ( *string_length == 0 )*/
				*(char *)(str_ptr) = 0U;
		}

		if ( out_string_length != nullptr )  *out_string_length = string_length;
		return (char *)retVal;
	}

} /* namespace StringUtil */
