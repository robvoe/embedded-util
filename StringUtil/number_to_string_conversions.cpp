/*
 * number_to_string_conversions.c
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module offers auxiliary functions to convert numeric values to strings.
 */

#include "number_to_string_conversions.h"

#include <stdint-gcc.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>


namespace StringUtil {


	uint32_t int32_to_string( int32_t input_value, char *destination_array, uint32_t maximum_resulting_digit_count, bool reserve_preceding_space_for_sign, ValueRadix radix, bool inflate_with_zeros, bool add_trailing_null_terminator ) {
		static char temp_string[31 /*for binary radix*/ + 1 /*eventual sign*/ + 1 /*null terminator*/];
		uint32_t written_char_count=0;

		// Sign?
		if ( input_value < 0 ) {
			input_value = -input_value;
			*temp_string = (char)'-';
			written_char_count = 1;
		} else if ( input_value>=0 && reserve_preceding_space_for_sign ) {
			*temp_string = (char)' ';
			written_char_count = 1;
		}

		// Generate string to a separated buffer to prevent overflows
		switch ( radix ) {
			case ValueRadix::Decimal:
				if ( inflate_with_zeros ) {
					sprintf( &temp_string[written_char_count], "%0*u", (int)(maximum_resulting_digit_count), (int)input_value );
				} else {
					sprintf( &temp_string[written_char_count], "%u", (int)input_value );
				}
				break;
			case ValueRadix::Hexadecimal:
				if ( inflate_with_zeros ) {
					sprintf( &temp_string[written_char_count], "%0*X", (int)(maximum_resulting_digit_count), (int)input_value );
				} else {
					sprintf( &temp_string[written_char_count], "%X", (int)input_value );
				}
				break;
			case ValueRadix::Binary:
				if ( inflate_with_zeros ) {
					// TODO: No yet implemented!
					while(1);
				} else {
					// TODO: No yet implemented!
					while(1);
				}
				break;
			default:
				break;
		}

		// Now, apply the generated string to destination array
		maximum_resulting_digit_count += written_char_count;
		written_char_count = 0;
		char *temp_string_ptr = temp_string;
		while ( *temp_string_ptr  &&  maximum_resulting_digit_count ) {
			*destination_array = *temp_string_ptr;
			destination_array++;
			temp_string_ptr++;
			maximum_resulting_digit_count--;
			written_char_count++;
		}

		// Now, if desired, insert null-terminator
		if ( add_trailing_null_terminator )  *destination_array = (const char)'\0';

		return written_char_count;
	}



	uint32_t float32_to_string( float input_value, char *destination_array, uint32_t maximum_precomma_digit_count, uint32_t maximum_postcomma_digit_count, bool reserve_preceding_space_for_sign, bool inflate_precomma_places_with_zeros, bool inflate_postcomma_places_with_zeros, bool add_trailing_null_terminator ) {
		if ( !maximum_precomma_digit_count && !maximum_postcomma_digit_count ) {
			if ( add_trailing_null_terminator )  *destination_array=(char)'\0';
			return 0;
		}

		// Let's have a look at some special cases
		if ( __isnanf(input_value) ) {
			// Not a number
			strcpy( destination_array, "NaN" );
			return 3;
		}
		if ( __isinff(input_value) ) {
			// Infinity
			strcpy( destination_array, "Inf" );
			return 3;
		}


		// Sign?
		uint32_t written_char_count=0;
		if ( input_value < 0 ) {
			input_value = -input_value;
			*destination_array++ = (char)'-';
			written_char_count = 1;
		} else if ( input_value>=0 && reserve_preceding_space_for_sign ) {
			*destination_array++ = (char)' ';
			written_char_count = 1;
		}


		// Generate pre-comma and post-comma places
		uint32_t     precomma_uint32 = (uint32_t)input_value;
		float        postcomma_float = input_value - precomma_uint32;

		static uint32_t multiplicator; 	multiplicator = 10;
		static uint32_t counter;
		for (counter = 1; counter<maximum_postcomma_digit_count; counter++)  multiplicator*=10;

		postcomma_float *= multiplicator;
		static uint32_t postcomma_uint32;     	postcomma_uint32 = (uint32_t)postcomma_float;

		// Round the last post-comma place. Eventually, there might be a carry to pre-comma place
		postcomma_float -= postcomma_uint32;
		postcomma_float *= 100;
		if ( (uint32_t)postcomma_float >= 45U ) {
			postcomma_uint32++;
			if ( postcomma_uint32 == multiplicator ) {  // Carry to pre-comma place?
				postcomma_uint32=0U;
				precomma_uint32++;
			}
		}


		// Generate pre-comma string
		counter = int32_to_string( precomma_uint32, destination_array, maximum_precomma_digit_count, false, ValueRadix::Decimal, inflate_precomma_places_with_zeros, false );
		written_char_count += counter;
		destination_array += counter;

		// If necessary: generate post-comma string
		if ( (postcomma_uint32||inflate_postcomma_places_with_zeros)  &&  maximum_postcomma_digit_count ) {
			*destination_array++ = '.';
			written_char_count++;

			// Apply post-comma places. The resulting string might contain trailing zeros. We'll remove those later.
			counter = int32_to_string( postcomma_uint32, destination_array, maximum_postcomma_digit_count, false, ValueRadix::Decimal, true, false );
			written_char_count += counter;
			destination_array += counter;

			// Now, if necessary, remove the trailing zeros
			if ( !inflate_postcomma_places_with_zeros ) {
				while ( *--destination_array == '0' )  written_char_count--;
				destination_array++;
			}
		}


		// If desired, now insert null-terminator.
		if ( add_trailing_null_terminator )  *destination_array = (const char)'\0';

		return written_char_count;
	}

} /* namespace StringUtil */


