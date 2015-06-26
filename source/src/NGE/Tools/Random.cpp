#include <cstdlib>
#include <ctime>

#include "NGE/Tools/Random.hpp"
#include "NGE/Math/MathUtils.hpp"

using namespace NGE::Tools;

Random::Random() {
	Seed(0);
}

Random::Random(unsigned seed) {
	Seed(seed);
}

void Random::Seed(unsigned seed) {
	if (seed == 0)
		seed = (unsigned) clock();

	for (unsigned i = 0; i < 17; ++i) {
		seed = seed * 2891336453U + 1;
		buffer[i] = seed;
	}

	p1 = 0;
	p2 = 10;
}

unsigned Random::RandomBits() {
	unsigned result;

	result = buffer[p1] = Math::MathUtils::Rotl(buffer[p2], 13) + Math::MathUtils::Rotl(buffer[p1], 9);

	if (--p1 < 0) p1 = 16;
	if (--p2 < 0) p2 = 16;

	return result;
}

float Random::RandomFloat() {
	unsigned bits = RandomBits();

	union {
		float value;
		unsigned word;
	} convert;

	convert.word = (bits >> 9) | 0x3f800000;

	return convert.value - 1.0f;
}

/*double Random::RandomDouble()
{
		unsigned bits = RandomBits();
	
		union
		{
				double value;
				unsigned word[2];
		} convert;
	
		convert.word[0] = bits << 20;
		convert.word[1] = (bits >> 12) | 0x3FF00000;
	
		return convert.value - 1.0;
}*/

float Random::RandomFloat(float min, float max) {
	return RandomFloat() * (max - min) + min;
}

float Random::RandomFloat(float scale) {
	return RandomFloat() * scale;
}

unsigned Random::RandomInt(unsigned max) {
	return RandomBits() % max;
}

float Random::RandomBinomial(float scale) {
	return (RandomFloat() - RandomFloat()) * scale;
}

// Quaterion Random

NGE::Math::vec3f Random::RandomVector(float scale) {
	return Math::vec3f(RandomBinomial(scale), RandomBinomial(scale), RandomBinomial(scale));
}

NGE::Math::vec3f Random::RandomXZVector(float scale) {
	return Math::vec3f(RandomBinomial(scale), 0, RandomBinomial(scale));
}

NGE::Math::vec3f Random::RandomVector(const NGE::Math::vec3f& scale) {
	return Math::vec3f(RandomBinomial(scale.x), RandomBinomial(scale.y), RandomBinomial(scale.z));
}

NGE::Math::vec3f Random::RandomVector(const NGE::Math::vec3f& min, const NGE::Math::vec3f& max) {
	return Math::vec3f(RandomFloat(min.x, max.x), RandomFloat(min.y, max.y), RandomFloat(min.z, max.z));
}

