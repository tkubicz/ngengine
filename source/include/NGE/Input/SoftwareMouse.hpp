/* 
 * File:   SoftwareMouse.hpp
 * Author: tku
 *
 * Created on 2 październik 2014, 15:02
 */

#ifndef SOFTWAREMOUSE_HPP
#define	SOFTWAREMOUSE_HPP

#include <vector>
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector4.hpp"

namespace NGE {
	namespace Input {

		/**
		 * Class to represent mouse that is rendered using graphics engine.
		 */
		class SoftwareMouse {
		  protected:
			/**
			 * Initializes the software mouse geometry. This method initializes
			 * vertices, indices and texture coordinates and fills the respected
			 * variables.
			 * @return True if initialization was ok, otherwise false.
			 */
			bool InitializeGeometry();

			/**
			 * Initializes the vertex buffer objects and vertex array of software
			 * mouse.
			 * @return True if everything was ok, otherwise false.
			 */
			bool InitializeObjects();

		  protected:
			/**
			 * Keeps the buffer object of vertices.
			 */
			unsigned int vertexBufferObject;

			/**
			 * Keeps the buffer object of indices.
			 */
			unsigned int indexBufferObject;

			/**
			 * Keeps the buffer object of texture coordinates.
			 */
			unsigned int textureBufferObject;

			/**
			 * Vertex array to render mouse in single draw call.
			 */
			unsigned int vertexArray;

			/**
			 * Array of vertices. Size 4, because we need four vertices to 
			 * render a square.
			 */
			std::vector<NGE::Math::vec4f> vertices;

			/**
			 * Array of indices. Size 4, because we need four indices to
			 * render a square.
			 */
			std::vector<unsigned int> indices;

			/**
			 * Array of texture coordinates.
			 */
			std::vector<NGE::Math::vec2i> texCoords;
		};
	}
}

#endif	/* SOFTWAREMOUSE_HPP */

