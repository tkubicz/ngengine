/* 
 * File:   SkyPlane.hpp
 * Author: tku
 *
 * Created on 16 wrzesień 2013, 14:11
 */

#ifndef SKYPLANE_HPP
#define	SKYPLANE_HPP

#include "NGE/Geometry/Mesh.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Media/Images/Texture.hpp"

namespace NGE {
	namespace Geometry {
		namespace Nature {

			class SkyPlane : public Geometry::Mesh {
			  public:
				void GenerateSkyPlane(int divisions, float planetRadius, float athmosphereRadius, float hTile, float vTile, float alphaPercent, bool exponentialFadeout = false);
				void Render(int alpha);

				void SetShader(Media::Shaders::GLSLProgram* shader);
				void SetTexture(Media::Images::Texture* texture);

			  protected:
				virtual void DeleteBuffers();
				virtual void DeleteArrays();

			  private:
				std::vector<float> alphas;

				Media::Shaders::GLSLProgram* shader;
				Media::Images::Texture* texture;
			};
		}
	}
}

#endif	/* SKYPLANE_HPP */

