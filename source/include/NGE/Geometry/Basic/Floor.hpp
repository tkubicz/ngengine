/* 
 * File:   Floor.hpp
 * Author: tku
 *
 * Created on 7 maj 2013, 16:19
 */

#ifndef FLOOR_HPP
#define	FLOOR_HPP

#include "NGE/Geometry/Basic/Basic.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"

namespace NGE {
	namespace Geometry {
		namespace Basic {

			class Floor : public Basic {
			  public:
				Floor();
				virtual ~Floor();

				virtual bool Initialize();

				/**
				 * Initialise the floor. This method invokes geometry generation
				 * and initialise VBO and VBA.
				 * @param halfSize Half size of the floor. Default 100.f.
				 * @param step How dense the floor should be. Default 4.f.
				 * @param shader Shader program used to render the floor.
				 * @return true if everything was ok, otherwise false.
				 */
				bool Initialize(float halfSize = 100.f, float step = 4.f, Media::Shaders::GLSLProgram* shader = NULL);

				/**
				 * Update the floor in time.
				 * @param dt Delta time.
				 */
				void Update(float dt);

				/**
				 * Render the floor.
				 */
				void Render();

				/**
				 * Set the floor color.
				 * @param color Color in RGBA.
				 */
				void SetColor(const Math::vec4f& color);

			  protected:
				/**
				 * Delete all previously created vertex buffers.
				 */
				virtual void DeleteBuffers();

				/**
				 * Delete all previously create vertex arrays.
				 */
				virtual void DeleteArrays();

				/**
				 * Initialise geometry and store it in proper fields inherited from
				 * Mesh interface.
				 * @return true if everything was ok, otherwise false.
				 */
				virtual bool InitializeGeometry();

				/**
				 * Initialise Vertex Buffer Objects.
				 * @return true if everything was ok, otherwise false.
				 */
				virtual bool InitializeVBO();

				/**
				 * Initialise Vertex Buffer Arrays.
				 * @return true if everything was ok, otherwise false.
				 */
				virtual bool InitializeVBA();

			  protected:
				/**
				 * Pointer to shader used to render the floor.
				 */
				Media::Shaders::GLSLProgram* shader;

				/**
				 * Color in RGBA. This field is used in default rendering method
				 * to set the floor color. Please remeber, that if you use
				 * your own rendering routin, you have to implement it (or not)
				 * yourself.
				 */
				Math::vec4f color;

				/**
				 * Half size of the floor in float.
				 */
				float halfSize;

				/**
				 * How dense flor tiles should be.
				 */
				float step;
			};
		}
	}
}

#endif	/* FLOOR_HPP */

