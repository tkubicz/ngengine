/* 
 * File:   SkinnedMesh.hpp
 * Author: tku
 *
 * Created on 7 październik 2013, 16:46
 */

#ifndef SKINNEDMESH_HPP
#define	SKINNEDMESH_HPP

#include <vector>
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Geometry/Mesh.hpp"
#include "NGE/Geometry/Skeleton/Skeleton.hpp"

namespace NGE {
	namespace Geometry {
		namespace Skeleton {

			class SkinnedMesh : public Mesh {
			  public:

				enum PolyType {
					TRIANGLE, QUAD
				};

				SkinnedMesh(PolyType type);
				virtual ~SkinnedMesh();

				void LoadMesh(const std::string& filename);
				void Preprocess();
				void Render();

				void SetColor(float r, float g, float b);
				void SetColor(const Math::vec3f& color);

				void AttachSkeleton(Skeleton* skeleton);
				Skeleton* GetSkeleton() const;

				void SetShader(Media::Shaders::GLSLProgram* shader);

			  protected:
				virtual void DeleteBuffers();
				virtual void DeleteArrays();

			  private:
				std::vector<Math::vec3f*> verticesV;
				std::vector<Math::vec4i*> polygons;

				std::vector<Math::vec3f> verticesW;
				std::vector<Math::vec3f> verticesVT;
				std::vector<Math::vec3f> vertices2;

				PolyType polytype;
				Math::vec3f color;

				Media::Shaders::GLSLProgram* shader;

				Skeleton* skeleton;

				void Normal(const Math::vec3f* p1, const Math::vec3f* p2, const Math::vec3f* p3) const;
				void Transform();
			};
		}
	}
}

#endif	/* SKINNEDMESH_H */

