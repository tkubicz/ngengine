/* 
 * File:   Octree.hpp
 * Author: tku
 *
 * Created on 18 October 2016, 12:33
 */

#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <vector>
#include "NGE/Math/Vector3.hpp"
// TOOD: Remove after testing
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Rendering/Camera/Frustum.hpp"

namespace NGE {
	namespace Rendering {
		namespace Scene {

			template <typename T> class Octree {
			  private:
				// Centre of the node of the tree
				Math::vec3f origin;
				// Half size of height, width and depth of the node
				Math::vec3f halfSize;

				Math::vec3f position;

				T* data;

			  public:

				Octree<T>* children[8];

				Octree(const Math::vec3f& origin, const Math::vec3f& halfSize) : origin(origin), halfSize(halfSize) {
					InitialiseBuffers();

					// At the beginning there are no nodes
					for (int i = 0; i < 8; ++i)
						children[i] = nullptr;
					data = nullptr;
				}

				Octree(const Octree<T>& copy) : origin(copy.origin), halfSize(copy.halfSize), data(copy.data) { }

				~Octree() {
					for (unsigned i = 0; i < 8; ++i) {
						if (children[i] != nullptr)
							delete children[i];
					}
				}

				void Clean() {
					for (unsigned i = 0; i < 8; ++i) {
						if (children[i] != nullptr)
							delete children[i];
					}
				}

				int GetOctantContainingPoint(const Math::vec3f& point) {
					int oct = 0;
					if (point.x > origin.x) oct |= 4;
					if (point.y > origin.y) oct |= 2;
					if (point.z > origin.z) oct |= 1;
					return oct;
				}

				bool IsLeafNode() const {
					return children[0] == nullptr;
				}

				void Insert(T* point, const Math::vec3f& position) {
					if (IsLeafNode()) {
						if (data == nullptr) {
							data = point;
							this->position = position;
							return;
						} else {
							T* oldPoint = data;
							Math::vec3f oldPosition = this->position;

							data = nullptr;
							this->position.Set(0, 0, 0);

							for (int i = 0; i < 8; ++i) {
								Math::vec3f newOrigin = origin;
								newOrigin.x += halfSize.x * (i & 4 ? .5f : -.5f);
								newOrigin.y += halfSize.y * (i & 2 ? .5f : -.5f);
								newOrigin.z += halfSize.z * (i & 1 ? .5f : -.5f);
								children[i] = new Octree(newOrigin, halfSize * .5f);
							}

							children[GetOctantContainingPoint(oldPosition)]->Insert(oldPoint, oldPosition);
							children[GetOctantContainingPoint(position)]->Insert(point, position);
						}
					} else {
						int octant = GetOctantContainingPoint(position);
						children[octant]->Insert(point, position);
					}
				}

				void GetPointsInsideBox(const Math::vec3f& bmin, const Math::vec3f& bmax, std::vector<T*>& results) {
					if (IsLeafNode()) {
						if (data != nullptr) {
							if (position.x > bmax.x || position.y > bmax.y || position.z > bmax.z)
								return;
							if (position.x < bmin.x || position.y < bmin.y || position.z < bmin.z)
								return;
							results.push_back(data);
						}
					} else {
						for (int i = 0; i < 8; ++i) {
							Math::vec3f cmax = children[i]->origin + children[i]->halfSize;
							Math::vec3f cmin = children[i]->origin - children[i]->halfSize;

							if (cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z)
								continue;
							if (cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z)
								continue;

							children[i]->GetPointsInsideBox(bmin, bmax, results);
						}
					}
				}

				Math::vec3f GetOrigin() const {
					return origin;
				}

				Math::vec3f GetHalfSize() const {
					return halfSize;
				}

				// TODO: Remove this fields after testing.
				GLuint vertexBuffer, indexBuffer, vertexArray;
				Media::Shaders::GLSLProgram* shader;

				// TODO: Remove this method after testing.

				void InitialiseBuffers() {
					// Create points - we need 8 vertices (upper and lower) wall, and
					// 4 faces. We are going to render boxes using GL_QUADS.
					Math::vec3f vertices[8];
					unsigned indices[24];

					// Lower wall
					vertices[0] = Math::vec3f(origin.x - halfSize.x, origin.y - halfSize.y, origin.z - halfSize.z);
					vertices[1] = Math::vec3f(origin.x + halfSize.x, origin.y - halfSize.y, origin.z - halfSize.z);
					vertices[2] = Math::vec3f(origin.x + halfSize.x, origin.y - halfSize.y, origin.z + halfSize.z);
					vertices[3] = Math::vec3f(origin.x - halfSize.x, origin.y - halfSize.y, origin.z + halfSize.z);
					// Upper wall
					vertices[4] = Math::vec3f(origin.x - halfSize.x, origin.y + halfSize.y, origin.z - halfSize.z);
					vertices[5] = Math::vec3f(origin.x + halfSize.x, origin.y + halfSize.y, origin.z - halfSize.z);
					vertices[6] = Math::vec3f(origin.x + halfSize.x, origin.y + halfSize.y, origin.z + halfSize.z);
					vertices[7] = Math::vec3f(origin.x - halfSize.x, origin.y + halfSize.y, origin.z + halfSize.z);

					// Indices
					// Lower wall
					/*indices[0] = 0;
					indices[1] = 1;
					indices[2] = 2;
					indices[3] = 3;

					// Upper wall
					indices[4] = 4;
					indices[5] = 5;
					indices[6] = 6;
					indices[7] = 7;
                    
					// Right wall
					indices[8] = 1;
					indices[9] = 2;
					indices[10] = 6;
					indices[11] = 5;
                    
					// Left wall
					indices[12] = 0;
					indices[13] = 3;
					indices[14] = 7;
					indices[15] = 4;
                    
					// Back wall
					indices[16] = 0;
					indices[17] = 1;
					indices[18] = 4;
					indices[19 ] = 5;*/

					indices[0] = 0;
					indices[1] = 1;
					indices[2] = 0;
					indices[3] = 3;
					indices[4] = 0;
					indices[5] = 4;

					indices[6] = 2;
					indices[7] = 1;
					indices[8] = 2;
					indices[9] = 3;
					indices[10] = 2;
					indices[11] = 6;

					indices[12] = 7;
					indices[13] = 3;
					indices[14] = 7;
					indices[15] = 6;
					indices[16] = 4;
					indices[17] = 7;

					indices[18] = 4;
					indices[19] = 5;
					indices[20] = 5;
					indices[21] = 6;
					indices[22] = 5;
					indices[23] = 1;

					/*vertices[0] = Math::vec3f(-10, 0, -10);
					vertices[1] = Math::vec3f(10, 0, -10);
					vertices[2] = Math::vec3f(10, 0, 10);
					vertices[3] = Math::vec3f(-10, 0, 10);
                    
					indices[0] = 0;
					indices[1] = 1;
					indices[2] = 2;
					indices[3] = 3;*/

					glGenBuffers(1, &vertexBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
					glBufferData(GL_ARRAY_BUFFER, 8 * sizeof (GLfloat) * 3, vertices, GL_STATIC_DRAW);

					glGenBuffers(1, &indexBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
					glBufferData(GL_ARRAY_BUFFER, 24 * sizeof (GLuint), indices, GL_STATIC_DRAW);

					glGenVertexArrays(1, &vertexArray);
					glBindVertexArray(vertexArray);

					glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

					glBindVertexArray(0);

					shader = Media::MediaManager::GetInstance().GetShaderManager().GetProgram("solidColorShader");
				}

				void Render(const Camera::Frustum<float>& frustum) {
					Math::Objects3D::AABox<float> box(origin, halfSize);
					if (frustum.AABoxInFrustum(box) == 2) {
						Render(Math::vec4f(0.0f, 1.f, 0.0f));
						for (unsigned i = 0; i < 8; ++i)
							if (children[i] != nullptr)
								children[i]->Render(frustum);
					} else if (frustum.AABoxInFrustum(box) == 1) {
						Render(Math::vec4f(0.0f, 0.0f, 1.f));
						for (unsigned i = 0; i < 8; ++i)
							if (children[i] != nullptr)
								children[i]->Render(frustum);
					} else {
						Render(Math::vec4f(1.0f, 0.0f, 0.f));
						for (unsigned i = 0; i < 8; ++i)
							if (children[i] != nullptr)
								children[i]->Render(frustum);
					}
				}

				void Render(const Math::vec4f& color) {
					shader->BindShader();
					shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
					shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
					shader->sendUniform("color", color);

					glBindVertexArray(vertexArray);
					glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);

					shader->UnbindShader();
				}
			};
		}
	}
}

#endif /* OCTREE_HPP */

