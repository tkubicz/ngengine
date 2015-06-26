#include "NGE/Appearance/Light.hpp"
using namespace NGE::Appearance;

void Light::SetPosition(const NGE::Math::vec4f& position) {
	this->position = position;
}

void Light::SetPosition(float x, float y, float z, float w) {
	this->position = Math::vec4f(x, y, z, w);
}

NGE::Math::vec4f Light::GetPosition() {
	return position;
}

void Light::SetAmbient(const NGE::Math::vec4f& ambient) {
	this->ambient = ambient;
}

void Light::SetAmbient(float r, float g, float b, float a) {
	this->ambient = Math::vec4f(r, g, b, a);
}

NGE::Math::vec4f Light::GetAmbient() {
	return ambient;
}

void Light::SetDiffuse(const NGE::Math::vec4f& diffuse) {
	this->diffuse = diffuse;
}

void Light::SetDiffuse(float r, float g, float b, float a) {
	this->diffuse = Math::vec4f(r, g, b, a);
}

NGE::Math::vec4f Light::GetDiffuse() {
	return diffuse;
}

void Light::SetSpecular(const NGE::Math::vec4f& specular) {
	this->specular = specular;
}

void Light::SetSpecular(float r, float g, float b, float a) {
	this->specular = Math::vec4f(r, g, b, a);
}

NGE::Math::vec4f Light::GetSpecular() {
	return specular;
}

void Light::SetConstantAttenuation(float attenuation) {
	constantAttenuation = attenuation;
}

float Light::GetConstantAttenuation() {
	return constantAttenuation;
}

void Light::SetLinearAttenuation(float attenuation) {
	linearAttenuation = attenuation;
}

float Light::GetLinearAttenuation() {
	return linearAttenuation;
}

void Light::SetQuadraticAttenuation(float attenuation) {
	quadraticAttenuation = attenuation;
}

float Light::GetQuadraticAttenuation() {
	return quadraticAttenuation;
}

void Light::SetSpotCutOff(float value) {
	spotCutOff = value;
}

float Light::GetSpotCutOff() {
	return spotCutOff;
}

void Light::SetSpotExponenet(float value) {
	spotExponenet = value;
}

float Light::GetSpotExponenet() {
	return spotExponenet;
}

void Light::SetSpotDirection(const NGE::Math::vec3f& direction) {
	spotDirection = direction;
}

void Light::SetSpotDirection(float x, float y, float z) {
	spotDirection.Set(x, y, z);
}

NGE::Math::vec3f Light::GetSpotDirection() {
	return spotDirection;
}