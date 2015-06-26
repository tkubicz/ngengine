/* 
 * File:   Material.hpp
 * Author: tku
 *
 * Created on 31 lipiec 2013, 13:54
 */

#ifndef MATERIAL_HPP
#define	MATERIAL_HPP

namespace NGE {
	namespace Appearance {

		class Material {
		  public:

			Material() { }

			void SetAmbientColor(const Math::vec4f& color) {
				ambientColor = color;
			}

			void SetAmbientColor(float r, float g, float b, float a) {
				ambientColor = Math::vec4f(r, g, b, a);
			}

			Math::vec4f GetAmbientColor() {
				return ambientColor;
			}

			void SetDiffuseColor(const Math::vec4f& color) {
				diffuseColor = color;
			}

			void SetDiffuseColor(float r, float g, float b, float a) {
				diffuseColor = Math::vec4f(r, g, b, a);
			}

			Math::vec4f GetDiffuseColor() {
				return diffuseColor;
			}

			void SetSpecularColor(const Math::vec4f& color) {
				specularColor = color;
			}

			void SetSpecularColor(float r, float g, float b, float a) {
				specularColor = Math::vec4f(r, g, b, a);
			}

			Math::vec4f GetSpecularColor() {
				return specularColor;
			}

			void SetEmissiveColor(const Math::vec4f& color) {
				emissiveColor = color;
			}

			void SetEmissiveColor(float r, float g, float b, float a) {
				emissiveColor = Math::vec4f(r, g, b, a);
			}

			Math::vec4f GetEmissiveColor() {
				return emissiveColor;
			}

			void SetShininess(float exponent) {
				shininess = exponent;
			}

			float GetShininess() {
				return shininess;
			}

			std::string GetName() {
				return name;
			}

			void SetName(const std::string& name) {
				this->name = name;
			}

		  protected:
			Math::vec4f ambientColor;
			Math::vec4f diffuseColor;
			Math::vec4f specularColor;
			Math::vec4f emissiveColor;
			float shininess;

			std::string name;
		};
	}
}

#endif	/* MATERIAL_HPP */

