/* 
 * File:   Renderer.hpp
 * Author: tku
 *
 * Created on 30 maj 2012, 00:04
 */

#ifndef RENDERER_HPP
#define	RENDERER_HPP

//#include "Core/Core.hpp"
#include "NGE/Rendering/MatrixStack.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Geometry/Ray.hpp"

#define matrixStack Rendering::Renderer::GetInstance().GetMatrixStack()

namespace NGE {
	namespace Rendering {

		class Renderer {
		  private:
			Renderer();
			Renderer(const Renderer&);
			Renderer& operator=(const Renderer&);
			~Renderer();

			MatrixStack<float> stack;
			Math::vec2i dimensions;

		  public:
			
			// TODO: Change it to use Singleton class.
			static Renderer& GetInstance() {
				static Renderer instance;
				return instance;
			}

			void GetRendererInformation();

			MatrixStack<float>& GetMatrixStack();

			Math::Vector3<float> GetScreenCoords(const Math::Vector3<float>& worldPosition);
			Math::Vector3<float> GetWorldCoords(const Math::Vector3<float>& windowPosition);

			Geometry::Ray<float> GetCursorRay(const Math::vec2i& mousePosition);

			void Enter2DMode();
			void Exit2DMode();

			/**
			 * Clear buffers - color, depth or stencil. If the method is invoked
			 * without any parameter, by default it clears all buffers.
			 * TODO: This method should be rewritten to support both OpenGL and
			 * DirectX.
			 * @param buffers This parameter is a bitfield.
			 * The values can be GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT,
			 * GL_STENCIL_BUFFER_BIT.
			 */
			void ClearBuffers(unsigned int buffers = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			/**
			 * Set background color. When rendering API is OpenGL this method
			 * invokes glClearColor with provided parameters.
			 * TODO: Add support to DirectX.
			 * @param red Red color component from 0.0f to 1.0f.
			 * @param green Green color component from 0.0f to 1.0f.
			 * @param blue Blue color component from 0.0f to 1.0f.
			 * @param alpha Alpha component from 0.0f to 1.0f.
			 */
			void ClearColor(float red, float green, float blue, float alpha);

			/**
			 * Set background color. When rendering API is OpenGL this method
			 * invokes glClearColor with provided parameters.
			 * @param color Color vector with componenets between 0.0f to 1.0f,
			 * where x is red, y is green, z is blue and w is alpha.
			 */
			void ClearColor(const Math::Vector4<float>& color);

			void SetDimensions(const Math::vec2i& dimensions);
			Math::vec2i& GetDimensions();
		};
	}
}

#endif	/* RENDERER_HPP */

