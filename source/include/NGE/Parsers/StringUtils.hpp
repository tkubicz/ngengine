/* 
 * File:   StringUtils.hpp
 * Author: tku
 *
 * Created on 17 March 2016, 16:09
 */

#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <algorithm>
#include <sstream>

namespace NGE {
	namespace Parsers {

		class StringUtils {
		  public:

			inline static void Replace(std::string& str, const std::string& from, const std::string& to) {
				size_t startPos = str.find(from);
				if (startPos == std::string::npos) {
					return;
				}
				str.replace(startPos, from.length(), to);
			}

			inline static void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
				if (from.empty()) {
					return;
				}
				size_t startPos = 0;
				while ((startPos = str.find(from, startPos)) != std::string::npos) {
					str.replace(startPos, from.length(), to);
					startPos += to.length();
				}
			}

			inline static std::string RandomString(size_t length) {
				auto randchar = []() -> char {
					const char charset[] =
							"0123456789"
							"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							"abcdefghijklmnopqrstuvwxyz";
					const size_t maxIndex = (sizeof (charset) - 1);
					return charset[std::rand() % maxIndex];
				};
				std::string str(length, 0);
				std::generate_n(str.begin(), length, randchar);
				return str;
			}

			static void Split(const std::string &s, char delim, std::vector<std::string> &elems) {
				std::stringstream ss;
				ss.str(s);
				std::string item;
				while (std::getline(ss, item, delim)) {
					elems.push_back(item);
				}
			}

			static std::vector<std::string> Split(const std::string &s, char delim) {
				std::vector<std::string> elems;
				Split(s, delim, elems);
				return elems;
			}
		};
	}
}

#endif /* STRINGUTILS_HPP */

