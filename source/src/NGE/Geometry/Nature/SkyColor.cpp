#include "NGE/Geometry/Nature/SkyColor.hpp"
#include "NGE/Math/MathUtils.hpp"
using namespace NGE::Geometry::Nature;

SkyColor::SkyColor() {
	suntheta = 0.0f;
	T = 0.0f;
}

void SkyColor::SetInfo(float sunTheta, float turbidity) {
	this->suntheta = sunTheta;
	suntheta2 = suntheta * suntheta;
	suntheta3 = suntheta * suntheta2;
	T = turbidity;
	T2 = T * T;

	perez_x[0] = -0.01925f * T - 0.25922f;
	perez_x[1] = -0.06651f * T + 0.00081f;
	perez_x[2] = -0.00041f * T + 0.21247f;
	perez_x[3] = -0.06409f * T - 0.89887f;
	perez_x[4] = -0.00325f * T + 0.04517f;

	perez_y[0] = -0.01669f * T - 0.26078f;
	perez_y[1] = -0.09495f * T + 0.00921f;
	perez_y[2] = -0.00792f * T + 0.21023f;
	perez_y[3] = -0.04405f * T - 1.65369f;
	perez_y[4] = -0.01092f * T + 0.05291f;

	perez_Y[0] = 0.17872f * T - 1.46303f;
	perez_Y[1] = -0.35540f * T + 0.42749f;
	perez_Y[2] = -0.02266f * T + 5.32505f;
	perez_Y[3] = 0.12064f * T - 2.57705f;
	perez_Y[4] = -0.06696f * T + 0.37027f;

	zenith_x = (0.00165f * suntheta3 - 0.00375f * suntheta2 + 0.00209f * suntheta + 0.00000f) * T2 +
			(-0.02903f * suntheta3 + 0.06377f * suntheta2 - 0.03202f * suntheta + 0.00394f) * T +
			(0.11693f * suntheta3 - 0.21196f * suntheta2 + 0.06052f * suntheta + 0.25886f);

	zenith_y = (0.00275f * suntheta3 - 0.00610f * suntheta2 + 0.00317f * suntheta + 0.00000f) * T2 +
			(-0.04214f * suntheta3 + 0.08970f * suntheta2 - 0.04153f * suntheta + 0.00516f) * T +
			(0.15346f * suntheta3 - 0.26756f * suntheta2 + 0.06670f * suntheta + 0.26688f);

	float X = (4.0f / 9.0f - T / 120.0f) * ((float) Math::MathUtils::PI - 2.0f * suntheta);
	zenith_Y = ((4.0453f * T - 4.9710f) * std::tan(X) - 0.2155f * T + 2.4192f);
	// Konwersja jasności z kcd/m^2 na cd/m^2
	zenith_Y *= 1000.0;
}

float SkyColor::PerezFunction(const float* perez, const float& cosTheta, const float& cosGamma, const float& gamma) {
	float val1 = (1 + perez[0] * exp(perez[1] / cosTheta)) * (1 + perez[2] * exp(perez[3] * gamma) + perez[4] * (cosGamma * cosGamma));
	float val2 = (1 + perez[0] * exp(perez[1])) * (1 + perez[2] * exp(perez[3] * suntheta) + perez[4] * (cosSTheta * cosSTheta));

	return val1 / val2;
}

void SkyColor::ToRGBA(float x, float y, float Y, NGE::Math::vec4f* rgb) {
	float fX, fY, fZ;

	fY = Y;
	fX = x / y * Y;
	fZ = ((1.0f - x - y) / y) * Y;

	float r, g, b;

	r = 3.240479f * fX - 1.537150f * fY - 0.498535f * fZ;
	g = -0.969256f * fX + 1.875991f * fY + 0.041556f * fZ;
	b = 0.055648f * fX - 0.204043f * fY + 1.057311f * fZ;

	float expo = -(1.0f / 15000.0f);
	r = 1.0f - exp(expo * r);
	g = 1.0f - exp(expo * g);
	b = 1.0f - exp(expo * b);

	r = Math::MathUtils::Clamp(r, 0.0f, 1.0f);
	g = Math::MathUtils::Clamp(g, 0.0f, 1.0f);
	b = Math::MathUtils::Clamp(b, 0.0f, 1.0f);

	rgb->x = r;
	rgb->y = g;
	rgb->z = b;
	rgb->w = 1.0f;
}

float SkyColor::Angle(float theta1, float phi1, float theta2, float phi2) {
	float cospsi = std::sin(theta1) * sin(theta2) * cos(phi2 - phi1) + cos(theta1) * cos(theta2);
	if (cospsi > 1.f)
		return 0.f;
	if (cospsi < -1.f)
		return (float) Math::MathUtils::PI;

	return acos(cospsi);
}

void SkyColor::GetVertexColor(float theta, float phi, float sunPhi, NGE::Math::vec4f* color) {
	float x, y, Y;
	float cosTheta, cosGamma;

	float gm = Angle(theta, phi, suntheta, sunPhi);
	float das = std::fabs(theta - ((float) Math::MathUtils::PI / 2.0f));
	if (das < 0.001f)
		cosTheta = 0.00001f;
	else
		cosTheta = cos(theta);

	cosGamma = cos(gm);
	cosSTheta = cos(suntheta);

	x = zenith_x * PerezFunction(perez_x, cosTheta, cosGamma, gm);
	y = zenith_y * PerezFunction(perez_y, cosTheta, cosGamma, gm);
	Y = zenith_Y * PerezFunction(perez_Y, cosTheta, cosGamma, gm);

	ToRGBA(x, y, Y, color);

	if (cosSTheta <= 0.1)
		color->w = cosSTheta / 0.1f;
	if (cosSTheta < 0.04)
		color->w = 0.4f;
}