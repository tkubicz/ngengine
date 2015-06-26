/* 
 * File:   Frustum.hpp
 * Author: tku
 *
 * Created on 26 grudzień 2013, 20:08
 */

#ifndef FRUSTUM_HPP
#define	FRUSTUM_HPP

#include "NGE/Core/Core.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Objects3D/Plane.hpp"
#include "NGE/Math/Objects3D/AABox.hpp"

// TODO: Remove this after testing
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
//#include "NGE/Media/MediaManager.hpp"

namespace NGE {
	namespace Rendering {
		namespace Camera {

			template <typename T> class Frustum {
			  protected:

				enum Plane {
					TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR
				};

				enum State {
					OUTSIDE = 0, INTERSECT, INSIDE
				};

				/**
				 * Hold the planes of the frustum.
				 */
				Math::Objects3D::Plane<T> plane[6];

				/**
				 * Points that represents the view frustum - near[] is the near plane
				 * and respectivly far is the far plane.
				 * Indices correspond to points:
				 * 0 - top left vertex, 
				 * 1 - top right vertex, 
				 * 2 - bottom left vertex,
				 * 3 - bottom right vertex.
				 */
				Math::Vector3<T> near[4], far[4];

				/**
				 * Dimensions of near and far plane.
				 * The x and y coefficients represents near plane width and height.
				 * The z and w coefficients represents far plane width and height.
				 */
				Math::Vector4<T> dimensions;

				/**
				 * The distance from the near plane.
				 */
				T nearDistance;

				/**
				 * The distance from the far plane.
				 */
				T farDistance;

				/**
				 * The ratio of width / height.
				 */
				T ratio;

				/**
				 * Field of view.
				 */
				T fieldOfView;

				T tang;


				Media::Shaders::GLSLProgram* shader;
				GLuint vertexBuffer, vertexArray;
				void InitialiseVBO();
				void InitialiseVBA();

			  public:

				/**
				 * Default empty constructor.
				 */
				Frustum() { }

				/**
				 * Set up camera frustum.
				 * @param angle Angle of the camera.
				 * @param ratio Ratio between width and height.
				 * @param nearDistance The distance from the near plane.
				 * @param farDistance The distance from the far plane.
				 */
				void SetCameraInternals(T fieldOfView, T ratio, T nearDistance, T farDistance);

				/**
				 * Set up camera definition. This method is used to update frustum.
				 * When camera changes this should be updated.
				 * @param position Position of the camera.
				 * @param focus Point where camera focuses.
				 * @param upVector Normalise up vector - usualy (0,1,0) when Y axis is UP.
				 */
				void SetCameraDefinition(const Math::Vector3<T>& position, const Math::Vector3<T>& focus, const Math::Vector3<T>& upVector);

				/**
				 * Test if point is in frustum.
				 * @param Point Position of the point.
				 * @return State in which point is.
				 * @see State enum.
				 */
				int PointInFrustum(const Math::Vector3<T>& point);

				/**
				 * Test if sphere is in furstum.
				 * @param position Position of the sphere.
				 * @param radius Radius of the sphere.
				 * @return State in which sphere is.
				 * @see State enum.
				 */
				int SphereInFrustum(const Math::Vector3<T>& position, T radius);

				/**
				 * Test if axis aligned box is in frustum.
				 * TODO: Finish this comment and method.
				 * @param position 
				 * @return 
				 */
				int AABoxInFrustum(const Math::Objects3D::AABox<T>& box) const;

				/**
				 * Render visual representation of frustum.
				 * Before calling this method you have to initialise VBO and VBA
				 * using InitialiseVBO() and InitialiseVBA().
				 */
				void Render();
			};

#include "NGE/Rendering/Camera/Frustum.inc"
		}
	}
}

#endif	/* FRUSTUM_HPP */

