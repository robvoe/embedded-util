/*
 * number_to_string_conversions.c
 *
 *  Created on: 27.07.2020
 *      Author: Robert Voelckner
 *
 *  Description:
 *    This module offers auxiliary functions to convert numeric values to strings.
 */
#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_


#include <stdint-gcc.h>


namespace StringUtil {

	/// Defines the value base for outputting integer-based values.
	enum class ValueRadix {
		Binary, Decimal, Hexadecimal
	};



	/**
	 * Converts a signed 32-bit integer value to string.
	 *
	 * @param input_value                    	..	The signed 32-bit value we want to output.
	 * @param destination_array              	..	Destination array. Should be at least of length "maximum_resulting_digit_count", PLUS sign, PLUS null-terminator.
	 * @param maximum_resulting_digit_count  	..	Maximum amount of resulting integer places. Does NOT include the sign nor null-terminator!
	 * @param reserve_preceding_space_for_sign 	..	Determines if we exclusively want to reserve a preceding space for the eventual sign.
	 * @param radix                         	..	Radix
	 * @param inflate_with_zeros             	..	Determines if unused places shall be inflated with preceding zeros.
	 * @param add_trailing_null_terminator     	..	Determines if, in the end, a null-terminator shall be inserted.
	 *
	 * @return                                	..	Amount of printable characters that were output (incl. sign, excl. null-terminator).
	 */
	extern uint32_t int32_to_string( int32_t input_value, char *destination_array, uint32_t maximum_resulting_digit_count = 10, bool reserve_preceding_space_for_sign = false, ValueRadix radix = ValueRadix::Decimal, bool inflate_with_zeros = false, bool add_trailing_null_terminator = true );


	/**
	 * Converts a signed floating-point value of single precision
	 * to a (decimal based) string.
	 *
	 * @param input_value                        	..	The floating point number we want to output.
	 * @param destination_array                  	..	Destination array. Should be at least of length "maximum_precomma_digit_count" PLUS
	 *                                                  "maximum_postcomma_digit_count", PLUS sign, PLUS null-terminator.
	 * @param maximum_precomma_digit_count        	..	Maximum amount of resulting pre-comma places. The rest will be cut from left.
	 * @param maximum_postcomma_digit_count        	..	Maximum amount of resulting post-comma places. The rest will be cut from right.
	 * @param reserve_preceding_space_for_sign     	..	Determines if we exclusively want to reserve a preceding space for the eventual sign.
	 * @param inflate_precomma_places_with_zeros  	..	Determines if unused pre-comma places shall be filled-up with zeros.
	 * @param inflate_postcomma_places_with_zeros 	..	Determines if unused post-comma places shall be filled-up with zeros.
	 * @param add_trailing_null_terminator         	..	Determines if, in the end, a null-terminator shall be inserted.
	 *
	 * @return                                    	..	Amount of printable characters that were output (incl. sign, excl. null-terminator).
	 */
	extern uint32_t float32_to_string( float input_value, char *destination_array, uint32_t maximum_precomma_digit_count = 10, uint32_t maximum_postcomma_digit_count = 3, bool reserve_preceding_space_for_sign = false, bool inflate_precomma_places_with_zeros = false, bool inflate_postcomma_places_with_zeros = false, bool add_trailing_null_terminator = true );


} /* namespace StringUtil */




#endif /* HELPER_FUNCTIONS_H_ */
