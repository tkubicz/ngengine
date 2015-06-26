/* 
 * File:   Sphere.hpp
 * Author: tku
 *
 * Created on 17 marzec 2012, 16:37
 */

#ifndef SPHERE_HPP
#define	SPHERE_HPP

#include "NGE/Rendering/MatrixStack.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Geometry/Mesh.hpp"
#include "NGE/Geometry/Basic/Basic.hpp"
#include <vector>

namespace NGE {
	namespace Geometry {
		namespace Basic {

			class Sphere : public Basic {
			  public:

				bool Initialize();
				bool Initialize(unsigned int slices, float radius, Media::Shaders::GLSLProgram* program = NULL);

				virtual void Update(float deltaTime);
				virtual void Render();

				void SetShader(Media::Shaders::GLSLProgram* shader);
				void SetTexture(Media::Images::Texture* texture);

			  protected:

				virtual void DeleteBuffers();
				virtual void DeleteArrays();

			  protected:

				virtual bool InitializeGeometry();
				virtual bool InitializeVBO();
				virtual bool InitializeVBA();

				unsigned int numberSlices;
				float sphereRadius;

				Media::Shaders::GLSLProgram* shader;
				Media::Images::Texture* texture;
			};
		}
	}
}

#endif	/* SPHERE_HPP */

