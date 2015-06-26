/* 
 * File:   Skeleton.hpp
 * Author: tku
 *
 * Created on 7 październik 2013, 16:33
 */

#ifndef SKELETON_HPP
#define	SKELETON_HPP

#include "NGE/Geometry/Skeleton/SkeletonNode.hpp"

namespace NGE {
	namespace Geometry {
		namespace Skeleton {

			class Skeleton {
			  public:
				Skeleton();
				virtual ~Skeleton();

				SkeletonNode* GetRoot() const;
				void LoadSkeleton(const std::string& filename);
				void Rotate(int i, float angleX, float angleY, float angleZ);
				void TranslateBase(float tx, float ty, float tz);

			  private:
				SkeletonNode* root;
				SkeletonNode* base;
				std::vector<SkeletonNode*> bones;

				void AttachBones();
			};
		}
	}
}

#endif	/* SKELETON_HPP */

