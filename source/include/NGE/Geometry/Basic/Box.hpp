/* 
 * File:   Box.hpp
 * Author: tku
 *
 * Created on 18 maj 2013, 15:57
 */

#ifndef BOX_HPP
#define	BOX_HPP

#include "NGE/Geometry/Mesh.hpp"
#include "NGE/Geometry/Basic/Basic.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Media/Images/Texture.hpp"

namespace NGE {
	namespace Geometry {
		namespace Basic {

			class Box : public Basic {
			  public:
				bool Initialize();

				/**
				 * Zainicjowanie kostki
				 * @param size Rozmiar boku kostki w jednostkach OpenGL. Domyślnie 1.0.
				 * @return true - jeżeli wszystko się powiodło.
				 */
				bool Initialize(float size);

				virtual void Update(float deltaTime);
				virtual void Render();

				void SetShader(Media::Shaders::GLSLProgram* shader);
				void SetTexture(Media::Images::Texture* texture);

			  protected:
				virtual void DeleteBuffers();
				virtual void DeleteArrays();

				virtual bool InitializeGeometry();
				virtual bool InitializeVBO();
				virtual bool InitializeVBA();

				Media::Shaders::GLSLProgram* shader;
				Media::Images::Texture* texture;
				float boxSize;
			};
		}
	}
}

#endif	/* BOX_HPP */

