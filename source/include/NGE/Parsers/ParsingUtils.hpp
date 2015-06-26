/* 
 * File:   ParsingUtils.hpp
 * Author: tku
 *
 * Created on 22 październik 2014, 15:36
 */

#ifndef PARSINGUTILS_HPP
#define	PARSINGUTILS_HPP

#include <cstring>

namespace NGE {
	namespace Parsers {

		class ParsingUtils {
		  public:

			template <class char_t> inline static bool IsSpace(char_t in) {
				return (in == (char_t) ' ' || in == (char_t) '\t');
			}

			template <class char_t> inline static bool IsLineEnd(char_t in) {
				return (in == (char_t) '\r' || in == (char_t) '\n' || in == (char_t) '\0');
			}

			template <class char_t> inline static bool IsSpaceOrNewLine(char_t in) {
				return IsSpace<char_t>(in) || IsLineEnd<char_t>(in);
			}

			template <class char_t> inline static bool SkipSpaces(const char_t* in, const char_t** out) {
				while (*in == (char_t) ' ' || *in == (char_t) '\t')
					in++;

				*out = in;
				return !IsLineEnd<char_t>(*in);
			}

			template <class char_t> inline static bool SkipSpaces(const char_t** inout) {
				return SkipSpaces<char_t>(*inout, inout);
			}

			template <class char_t> inline static bool SkipLine(const char_t* in, const char_t** out) {
				while (*in != (char_t) '\r' && *in != (char_t) '\n' && *in != (char_t) '\0')
					in++;

				// Files are opened in binary mode. Ergo there are both NL and CR.
				while (*in == (char_t) '\r' || *in == (char_t) '\n')
					in++;

				*out = in;
				return *in != (char_t) '\0';
			}

			template <class char_t> inline static bool SkipLine(const char** inout) {
				return SkipLine<char_t>(*inout, inout);
			}

			template <class char_t> inline static bool SkipSpacesAndLineEnd(const char_t* in, const char_t** out) {
				while (*in == (char_t) ' ' || *in == (char_t) '\t' || *in == (char_t) '\r' || *in == (char_t) '\n')
					in++;
				*out = in;
				return *in != '\0';
			}

			template <class char_t> inline static bool SkipSpacesAndLineEnd(const char_t** inout) {
				return SkipSpacesAndLineEnd<char_t>(*inout, inout);
			}

			template <class char_t> inline static bool TokenMath(char_t* &in, const char* token, unsigned int len) {
				if (!::strncmp(token, in, len) && IsSpaceOrNewLine(in[len])) {
					in += len + 1;
					return true;
				}
				return false;
			}
		};
	}
}

#endif	/* PARSINGUTILS_HPP */

