/* 
 * File:   IntersectionTests.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:34
 */

#ifndef INTERSECTIONTESTS_HPP
#define	INTERSECTIONTESTS_HPP

#include "NGE/Physics/RigidBody/CollisionSphere.hpp"
#include "NGE/Physics/RigidBody/CollisionPlane.hpp"
#include "NGE/Physics/RigidBody/CollisionBox.hpp"


namespace NGE {
	namespace Physics {
		namespace RigidBody {

			/**
			 * Klasa zawierająca testy przecinania się obiektów. Ta klasa może
			 * być użyta do prostego systemu detekcji kolizjii.
			 */
			class IntersectionTests {
				friend class CollisionDetector;

			  public:
				static bool SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane);
				static bool SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two);
				static bool BoxAndBox(const CollisionBox& one, const CollisionBox& two);
				static bool BoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane);

			  protected:
				static float TransformToAxis(const CollisionBox& box, const Math::vec3f& axis);
				static float OverlapOnAxis(const CollisionBox& one, const CollisionBox& two, const Math::vec3f& axis, const Math::vec3f& toCenter);
			};
		}
	}
}

#endif	/* INTERSECTIONTESTS_HPP */

