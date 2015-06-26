/* 
 * File:   NumberConverter.hpp
 * Author: tku
 *
 * Created on 23 luty 2014, 21:00
 */

#ifndef NUMBERCONVERTER_HPP
#define NUMBERCONVERTER_HPP

#include <math.h>
#include <stdint.h>

namespace NGE {
	namespace Math {

		class NumberConverter {
		  public:

			/**
			 * Convert a string in decimal format to a number.
			 * @param in
			 * @param out
			 * @return 
			 */
			inline static unsigned int StrToUl10(const char* in, const char** out = 0);

			/**
			 * Convert a string in decimal format to a signed number.
			 * Signed variant of StrToUl10.
			 * @param in 
			 * @param out
			 * @return 
			 */
			inline static int StrToL10(const char* in, const char** out = 0);

			/**
			 * Convert a string in octal format to a number.
			 * @param in
			 * @param out
			 * @return 
			 */
			inline static unsigned int StrToUl8(const char* in, const char** out = 0);

			/**
			 * Conver a string in hex format to a number.
			 * @param in
			 * @param out
			 * @return 
			 */
			inline static unsigned int StrToUl16(const char* in, const char** out = 0);

			/**
			 * Convert just one hex digit.
			 * @param in Character of hex digit.
			 * @return Convertex number or UINT_MAX if the input character
			 * is not a hex digit.
			 */
			inline static unsigned int HexDigitToDecimal(char in);

			/**
			 * Convert a hex-encoded octet to a number.
			 * @param in Octet - 2 characters e.g. df or 1a.
			 * @return Convertex number as uint8_t.
			 */
			inline static uint8_t HexOctetToDecimal(const char* in);

			/**
			 * Parse a C++ like integer literal - hex and oct prefixes.
			 * 0xNNNN   - hex
			 * 0NNN     - oct
			 * @NNN     - dec
			 * @param in
			 * @param out
			 * @return 
			 */
			inline static unsigned int StrToUl_CppStyle(const char* in, const char** out = 0);

			/**
			 * Special version of the function, providing higher accuracy and safety.
			 * It is mainly used by FastAtoF to prevent ugly and unwanted integer overflows.
			 * @param in
			 * @param out
			 * @param maxiInout
			 * @return 
			 */
			inline static uint64_t StrToUl10_64(const char* in, const char** out = 0, unsigned int* maxInout = 0);

			/**
			 * Provides a fast functiom for converting a string into a float, about
			 * 6 times faster than atof in win32.
			 * @param c
			 * @param out
			 * @return 
			 */
			template <typename T> inline static const char* FastAToFMove(const char* c, T& out);

			/**
			 * Fast function for converting a string into a float.
			 * @param c
			 * @return 
			 */
			inline static float FastAToF(const char* c);

		  protected:

			static short fastAtoFRelevantDecimals;

			static const float fastAToFTable[16];
		};

#include "NGE/Math/NumberConverter.inc"
	}
}

#endif /* NUMBERCONVERTER_HPP */

