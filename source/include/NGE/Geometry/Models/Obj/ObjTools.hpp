/* 
 * File:   ObjTools.hpp
 * Author: tku
 *
 * Created on 8 październik 2014, 16:34
 */

#ifndef OBJTOOLS_HPP
#define	OBJTOOLS_HPP

#include "NGE/Math/NumberConverter.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Models
        {
            namespace Obj
            {

                /**
                 * Returns true, if the last entry of the buffer is reached.
                 * @param it Iterator of current position.
                 * @param end Iterator with end of buffer.
                 * @return True, if the end of buffer is reached.
                 */
                template<class char_t> inline bool IsEndOfBuffer(char_t it, char_t end)
                {
                    if (it == end)
                    {
                        return true;
                    }
                    else
                    {
                        end--;
                    }
                    return (it == end);
                }

                /**
                 * Returns true, if token is a space of any supported platform.
                 * @param token Token to search in.
                 * @return True, if token is a space.
                 */
                inline bool IsSeparator(char token)
                {
                    return (token == ' ' ||
                            token == '\n' ||
                            token == '\f' ||
                            token == '\r' ||
                            token == '\t');
                }

                /**
                 * Returns true, if token id a new line marking token.
                 * @param token Token to search in.
                 * @return True, if token is a newline token.
                 */
                inline bool IsNewLine(char token)
                {
                    return (token == '\n' || token == '\f' || token == '\r');
                }

                /**
                 * Returns next word separated by a space.
                 * @param buffer Pointer to the data buffer.
                 * @param end Pointer to end of buffer.
                 * @return Pointer to next space.
                 */
                template<class char_t> inline char_t GetNextWord(char_t buffer, char_t end)
                {
                    while (!IsEndOfBuffer(buffer, end))
                    {
                        if (!IsSeparator(*buffer) || IsNewLine(*buffer))
                            break;
                        buffer++;
                    }
                    return buffer;
                }

                /**
                 * Returns pointer to next token.
                 * @param buffer Pointer to data buffer.
                 * @param end Pointer to end of buffer.
                 * @return Pointer to next token.
                 */
                template<class char_t> inline char_t GetNextToken(char_t buffer, char_t end)
                {
                    while (!IsEndOfBuffer(buffer, end))
                    {
                        if (IsSeparator(*buffer))
                            break;
                        buffer++;
                    }
                    return GetNextWord(buffer, end);
                }

                /**
                 * Skips a line.
                 * @param it Iterator set to current position.
                 * @param end Iterator set to end of scratch buffer for readout.
                 * @param line Current linenumber in format.
                 * @return Current-iterator with a new position.
                 */
                template<class char_t> inline char_t SkipLine(char_t it, char_t end, unsigned int& line)
                {
                    while (!IsEndOfBuffer(it, end) && !IsNewLine(*it))
                        ++it;
                    if (it != end)
                    {
                        ++it;
                        ++line;
                    }
                    // TODO: Fix.. from time to time here are spaces at the beginning of material line
                    while (it != end && (*it == '\t' || *it == ' '))
                        ++it;
                    return it;
                }

                /**
                 * Get a name from the current line. Preserve space in the middle,
                 * but trim it at the end.
                 * @param it Set to current position.
                 * @param end Set to end of scratch buffer for readout.
                 * @param name Separated name.
                 * @return Current-iterator with a new position.
                 */
                template<class char_t> inline char_t GetName(char_t it, char_t end, std::string& name)
                {
                    name = "";
                    it = GetNextToken<char_t>(it, end);
                    if (IsEndOfBuffer(it, end))
                        return end;

                    char* start = &(*it);
                    while (!IsEndOfBuffer(it, end) && !IsNewLine(*it))
                        ++it;

                    while (IsEndOfBuffer(it, end) || IsNewLine(*it) || IsSeparator(*it))
                        --it;

                    ++it;

                    // Get a name.
                    std::string strname(start, &(*it));
                    if (strname.empty())
                        return it;
                    else
                        name = strname;

                    return it;
                }

                /**
                 * Get next word from the given line.
                 * @param it Set to current position.
                 * @param end Set to end of scratch buffer for readout.
                 * @param buffer Buffer for next word.
                 * @param length Buffer length.
                 * @return Current-iterator with new position.
                 */
                template<class char_t> inline char_t CopyNextWord(char_t it, char_t end, char* buffer, size_t length)
                {
                    size_t index = 0;
                    it = GetNextWord<char_t>(it, end);
                    while (!IsSeparator(*it) && !IsEndOfBuffer(it, end))
                    {
                        buffer[index] = *it;
                        index++;
                        if (index == length - 1)
                            break;
                        ++it;
                    }
                    buffer[index] = '\0';
                    return it;
                }

                /**
                 * Get next float from the given line.
                 * @param it Set to current position.
                 * @param end Set to end of scratch buffer for readout.
                 * @param value Separated float value.
                 * @return Current-iterator with new position.
                 */
                template<class char_t> inline char_t GetFloat(char_t it, char_t end, float& value)
                {
                    static const size_t BUFFERSIZE = 1024;
                    char buffer[BUFFERSIZE];
                    it = CopyNextWord<char_t>(it, end, buffer, BUFFERSIZE);
                    value = (float) Math::NumberConverter::FastAToF(buffer);

                    return it;
                }

                /**
                 * Will perform a simple tokenize.
                 * @param str String to teknize.
                 * @param tokens Array with tokens, will be empty if no token was found.
                 * @param delimiters Delimiter for tokenize.
                 * @return Number of found token.
                 */
                template <class string_type> unsigned int Tokenize(const string_type& str, std::vector<string_type>& tokens,
                                                                   const string_type& delimiters)
                {
                    // Skip delimiters at beginning.
                    typename string_type::size_type lastPos = str.find_first_not_of(delimiters, 0);

                    // Find first "non-delimiter".
                    typename string_type::size_type pos = str.find_first_of(delimiters, lastPos);
                    while (string_type::npos != pos || string_type::npos != lastPos)
                    {
                        // Found a token, add it to the vector.
                        string_type tmp = str.substr(lastPos, pos - lastPos);
                        if (!tmp.empty() && ' ' != tmp[0])
                            tokens.push_back(tmp);

                        // Skip delimiters. Note the "not_of".
                        lastPos = str.find_first_not_of(delimiters, pos);

                        // Find next "non-delimiter".
                        pos = str.find_first_of(delimiters, lastPos);
                    }

                    return static_cast<unsigned int> (tokens.size());
                }

                template<class char_t> bool IsLineEnd(char_t in)
                {
                    return (in == (char_t) '\r' || in == (char_t) '\n' || in == (char_t) '\0');
                }
            }
        }
    }
}

#endif	/* OBJTOOLS_HPP */

