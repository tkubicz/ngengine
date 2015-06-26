#include <limits.h>

unsigned int NumberConverter::StrToUl10(const char* in, const char** out) {
	unsigned int value = 0;

	bool running = true;
	while (running) {
		if (*in < '0' || *in > '9')
			break;

		value = (value * 10) + (*in - '0');
		++in;
	}
	if (out)
		*out = in;
	return value;
}

int NumberConverter::StrToL10(const char* in, const char** out) {
	bool inv = (*in == '-');
	if (inv || *in == '+')
		++in;

	int value = StrToUl10(in, out);
	if (inv)
		value = -value;
	return value;
}

unsigned int NumberConverter::StrToUl8(const char* in, const char** out) {
	unsigned int value = 0;

	bool running = true;
	while (running) {
		if (*in < '0' || *in > '7')
			break;

		value = (value << 3) + (*in - '0');
		++in;
	}

	if (out)
		*out = in;
	return value;
}

unsigned int NumberConverter::StrToUl16(const char* in, const char** out) {
	unsigned int value = 0;
	bool running = true;
	while (running) {
		if (*in >= '0' && *in <= '9')
			value = (value << 4u) + (*in - '0');
		else if (*in >= 'A' && *in <= 'F')
			value = (value << 4u) + (*in - 'A') + 10;
		else if (*in >= 'a' && *in <= 'f')
			value = (value << 4u) + (*in - 'a') + 10;
		else
			break;
		++in;
	}

	if (out)
		*out = in;
	return value;
}

unsigned int NumberConverter::HexDigitToDecimal(char in) {
	unsigned int out = UINT_MAX;
	if (in >= '0' && in <= '9')
		out = in - '0';
	else if (in >= 'a' && in <= 'f')
		out = 10u + in - 'a';
	else if (in >= 'A' && in <= 'F')
		out = 10u + in - 'A';

	return out;
}

uint8_t NumberConverter::HexOctetToDecimal(const char* in) {
	return ((uint8_t) HexDigitToDecimal(in[0]) << 4) + (uint8_t) HexDigitToDecimal(in[1]);
}

unsigned int NumberConverter::StrToUl_CppStyle(const char* in, const char** out) {
	if ('0' == in[0])
		return 'x' == in[1] ? StrToUl16(in + 2, out) : StrToUl8(in + 1, out);
	return StrToUl10(in, out);
}

uint64_t NumberConverter::StrToUl10_64(const char* in, const char** out, unsigned int* maxInout) {
	unsigned int cur = 0;
	uint64_t value = 0;

	bool running = true;
	while (running) {
		if (*in < '0' || *in > '9')
			break;

		const uint64_t newValue = (value * 10) + (*in - '0');

		// We rely here on numeric overflow.
		if (newValue < value)
			return value;

		value = newValue;

		++in;
		++cur;

		if (maxInout && *maxInout == cur) {
			// Skip to the end.
			if (out) {
				while (*in >= '0' && *in <= '9')
					++in;
				*out = in;
			}
		}
	}

	if (out)
		*out = in;

	if (maxInout)
		*maxInout = cur;

	return value;
}

template <typename T> const char* NumberConverter::FastAToFMove(const char* c, T& out) {
	T f;

	bool inv = (*c == '-');
	if (inv || *c == '+')
		c++;

	f = static_cast<T> (StrToUl10_64(c, &c));
	// Allow using commas.
	if (*c == '.' || (c[0] == ',' && c[1] >= '0' && c[1] <= '9')) {
		++c;

		// NOTE: The original implementation is highly inaccurate here. Thre precision
		// of a single IEEE 754 float is not high enough, everything behind the 6th
		// digit tends to be more inaccurate than it would need to be. Casting to
		// double seems to solve the problem.
		// StrToUl10_64 is used to prevent integer overflow.

		// Another fix: this tends to become 0 for long numbers if we don't limit
		// the maximum number of digits to be read. 
		unsigned int diff = fastAtoFRelevantDecimals;
		double pl = static_cast<double> (StrToUl10_64(c, &c, &diff));

		pl *= fastAToFTable[diff];
		f += static_cast<T> (pl);
	}

	// A major 'E' must be allowed. Necessary for proper reading of some DXF files.
	if (*c == 'e' || *c == 'E') {
		++c;
		const bool einv = (*c == '-');
		if (einv || *c == '+')
			++c;

		// The reason float constant are used here is that we've seen cases where
		// compilers would perform such cast on compile-time constants at runtime.
		T exp = static_cast<T> (StrToUl10_64(c, &c));
		if (einv)
			exp = -exp;
		f *= pow(static_cast<T> (10.0f), exp);
	}

	if (inv)
		f = -f;

	out = f;
	return c;
}

float NumberConverter::FastAToF(const char* c) {
	float ret;
	FastAToFMove<float>(c, ret);
	return ret;
}