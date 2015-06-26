/* 
 * File:   SkeletonNode.hpp
 * Author: tku
 *
 * Created on 27 wrzesień 2013, 20:00
 */

#ifndef SKELETONNODE_HPP
#define	SKELETONNODE_HPP

#include <vector>
#include <list>

#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Matrix4.hpp"

namespace NGE {
	namespace Geometry {
		namespace Skeleton {

			class SkeletonNode {
			  public:
				SkeletonNode(int parentIndex, float tx, float ty, float tz, int firstIndex, int lastIndex);
				SkeletonNode();
				~SkeletonNode();

				void AddChild(SkeletonNode* node);
				void RemoveChild(SkeletonNode* node);

				void RotateX(float angleX);
				void RotateY(float angleY);
				void RotateZ(float angleZ);
				void Translate(float tx, float ty, float tz);

				void AttachVertices(int firstIndex, int lastIndex);
				void SetParentIndex(int parentIndex);
				int GetParentIndex() const;
				int GetFirstIndex() const;
				int GetLastIndex() const;

				Math::mat4f* GetMatrix() const;
				Math::mat4f* GetInverseMatrix() const;

				SkeletonNode* GetParent() const;
				int GetChildCount() const;

				void UpdateMatrices();

				std::vector<Math::vec3f> PreprocessPhase(std::vector<Math::vec3f>& vertices);
				std::vector<Math::vec3f> AnimationPhase(std::vector<Math::vec3f>& vertices);

				void Transform1(std::vector<Math::vec3f>& vertices, float tx, float ty, float tz);
				void Transform2(std::vector<Math::vec3f>& vertices, Math::mat4f matrix);

			  private:
				std::list<SkeletonNode*> children;
				int firstIndex, lastIndex, parentIndex;
				SkeletonNode* parent;

				Math::vec3f t, angle;
				Math::mat4f *matrix, *invMatrix;
			};
		}
	}
}

#endif	/* SKELETONNODE_HPP */

