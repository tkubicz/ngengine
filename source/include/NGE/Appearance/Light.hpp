/* 
 * File:   Light.hpp
 * Author: tku
 *
 * Created on 14 lipiec 2013, 18:50
 */

#ifndef LIGHT_HPP
#define	LIGHT_HPP

#include "NGE/Math/Vector4.hpp"

namespace NGE {
	namespace Appearance {

		class Light {
		  public:

			Light() { }

			void SetPosition(const Math::vec4f& position);
			void SetPosition(float x, float y, float z, float w);
			Math::vec4f GetPosition();

			void SetAmbient(const Math::vec4f& ambient);
			void SetAmbient(float r, float g, float b, float a);
			Math::vec4f GetAmbient();

			void SetDiffuse(const Math::vec4f& diffuse);
			void SetDiffuse(float r, float g, float b, float a);
			Math::vec4f GetDiffuse();

			void SetSpecular(const Math::vec4f& specular);
			void SetSpecular(float r, float g, float b, float a);
			Math::vec4f GetSpecular();

			void SetConstantAttenuation(float attenuation);
			float GetConstantAttenuation();

			void SetLinearAttenuation(float attenuation);
			float GetLinearAttenuation();

			void SetQuadraticAttenuation(float attenuation);
			float GetQuadraticAttenuation();

			void SetSpotCutOff(float value);
			float GetSpotCutOff();

			void SetSpotExponenet(float value);
			float GetSpotExponenet();

			void SetSpotDirection(const Math::vec3f& direction);
			void SetSpotDirection(float x, float y, float z);
			Math::vec3f GetSpotDirection();

		  protected:
			Math::vec4f position,
			ambient,
			diffuse,
			specular;

			float constantAttenuation,
			linearAttenuation,
			quadraticAttenuation;

			// Spot
			float spotCutOff, spotExponenet;
			Math::vec3f spotDirection;
		};
	}
}

#endif	/* LIGHT_HPP */

