#include "NGE/Geometry/Models/MD5/MD5Parser.hpp"
#include "NGE/Math/NumberConverter.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Geometry::Models::MD5;

MD5Parser::MD5Parser(char* buffer, unsigned int fileSize) {
	if (buffer == nullptr || fileSize == 0)
		return;

	this->buffer = buffer;
	this->fileSize = fileSize;
	lineNumber = 0;

	log_info("MD5Parser --> Parsing started.");

	// Parse the file header.
	ParseHeader();

	// Read all sections until we're finished.
	bool running = true;
	while (running) {
		sections.push_back(Section());
		Section& sec = sections.back();
		if (!ParseSection(sec))
			break;
	}

	log_info("MD5Parser --> Parsing ended. Parsed " + to_string((int) sections.size()) + " sections.");
}

void MD5Parser::ReportError(const char* error, unsigned int line) {
	log_error("MD5Parser --> Error, Line " + to_string(line) + ": " + to_string(error) + ".");
}

void MD5Parser::ReportWarning(const char* warn, unsigned int line) {
	log_info("MD5Parser --> Warning, Line " + to_string(line) + ": " + to_string(warn) + ".");
}

void MD5Parser::ParseHeader() {
	// Parse and validate the file version.
	SkipSpaces();
	if (!Parsers::ParsingUtils::TokenMath(buffer, "MD5Version", 10)) {
		ReportError("Invalid MD5 file: MD5Version tag has not been found");
	}
	SkipSpaces();
	unsigned int iVer = Math::NumberConverter::StrToUl10(buffer, (const char**) &buffer);
	if (iVer != 10) {
		ReportError("MD5 version tag is unknown (10 is expected)");
	}
	SkipLine();

	while (!Parsers::ParsingUtils::IsLineEnd(*buffer++));
	SkipSpacesAndLineEnd();
}

bool MD5Parser::ParseSection(Section& out) {
	// Store the current line number for use in error messages.
	out.SetLineNumber(lineNumber);

	// First parse the name of the section.
	char* sz = buffer;
	while (!Parsers::ParsingUtils::IsSpaceOrNewLine(*buffer))
		buffer++;
	out.SetName(std::string(sz, (uintptr_t) (buffer - sz)));
	SkipSpaces();

	bool running = true;
	while (running) {
		if (*buffer == '{') {
			// It is a normal section so read all lines.
			buffer++;
			bool run = true;
			while (run) {
				if (!SkipSpacesAndLineEnd()) {
					// It seems this was the last section.
					return false;
				}
				if (*buffer == '}') {
					buffer++;
					break;
				}

				out.GetElements().push_back(Element());
				Element& elem = out.GetElements().back();

				elem.SetLineNumber(lineNumber);
				elem.SetStart(buffer);

				// Terminate the line with zero.
				while (!Parsers::ParsingUtils::IsLineEnd(*buffer))
					buffer++;
				if (*buffer) {
					++lineNumber;
					*buffer++ = '\0';
				}
			}
			break;
		} else if (!Parsers::ParsingUtils::IsSpaceOrNewLine(*buffer)) {
			// It is an element at global scope. Parse its value and go on.
			sz = buffer;
			while (!Parsers::ParsingUtils::IsSpaceOrNewLine(*buffer++));
			out.SetGlobalValue(std::string(sz, (uintptr_t) (buffer - sz)));
			continue;
		}
		break;
	}
	return SkipSpacesAndLineEnd();
}

#define MD5_SKIP_SPACES() if (!SkipSpaces(&sz)) \
    MD5Parser::ReportWarning("Unexpected end of line", (*eit).GetLineNumber());

#define MD5_READ_TRIPLE(vec) \
    MD5_SKIP_SPACES(); \
    if ('(' != *sz++) \
        MD5Parser::ReportWarning("Unexpected token: ( was expected",(*eit).GetLineNumber()); \
    MD5_SKIP_SPACES(); \
    sz = Math::NumberConverter::FastAToFMove<float>(sz, (float&)vec.x); \
    MD5_SKIP_SPACES(); \
    sz = Math::NumberConverter::FastAToFMove<float>(sz, (float&)vec.y); \
    MD5_SKIP_SPACES(); \
    sz = Math::NumberConverter::FastAToFMove<float>(sz, (float&)vec.z); \
    MD5_SKIP_SPACES(); \
    if (')' != *sz++) \
        MD5Parser::ReportWarning("Unexpected token: ) was expected", (*eit).GetLineNumber());

#define MD5_PARSE_STRING(out) \
    bool quota = (*sz == '\"'); \
    const char* start = sz; \
    while (!Parsers::ParsingUtils::IsSpaceOrNewLine(*sz)) ++sz; \
    const char* end = sz; \
    if (quota) { \
        start++; \
        if ('\"' != *(end-=1)) { \
            MD5Parser::ReportWarning("Expected closing quotation marks in string", \
                (*eit).GetLineNumber()); \
            continue; \
        } \
    } \
    out.SetLength((size_t)(end - start)); \
    ::memcpy(out.data(), start, out.length); \
    out.data()[out.length] = '\0';

MD5MeshParser::MD5MeshParser(SectionList& sections) {
	log_info("MD5MeshParser --> parsing started.");

	// Parse all sections.
	for (SectionList::const_iterator iter = sections.begin(), iterEnd = sections.end(); iter != iterEnd; ++iter) {
		if ((*iter).GetName() == "numMeshes") {
			meshses.reserve(Math::NumberConverter::StrToUl10((*iter).GetGlobalValue().c_str()));
		} else if ((*iter).GetName() == "numJoints") {
			joints.reserve(Math::NumberConverter::StrToUl10((*iter).GetGlobalValue().c_str()));
		} else if ((*iter).GetName() == "joints") {
			for (ElementList::const_iterator eit = (*iter).GetElements().begin(), eitEnd = (*iter).GetElements().end(); eit != eitEnd; ++eit) {
				joints.push_back(BoneDesc());
				BoneDesc& desc = joints.back();

				const char* sz = (*eit).GetStart();
				MD5_PARSE_STRING(desc.GetName());
				MD5_SKIP_SPACES();

				// Negative values, at least -1, is allowed here.
				desc.SetParentIndex((int) Math::NumberConverter::StrToL10(sz, &sz));

				MD5_READ_TRIPLE(desc.GetPositionXYZ());
				// Normalize quaternion.
				MD5_READ_TRIPLE(desc.GetRotationQuat());
			}
		} else if ((*iter).GetName() == "mesh") {
			meshses.push_back(MeshDesc());
			MeshDesc& desc = meshses.back();


		}
	}
}



