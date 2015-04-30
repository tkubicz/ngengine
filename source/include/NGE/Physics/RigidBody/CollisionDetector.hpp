/* 
 * File:   CollisionDetector.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:47
 */

#ifndef COLLISIONDETECTOR_HPP
#define	COLLISIONDETECTOR_HPP

#include "NGE/Physics/RigidBody/CollisionSphere.hpp"
#include "NGE/Physics/RigidBody/CollisionPlane.hpp"
#include "NGE/Physics/RigidBody/CollisionData.hpp"
#include "NGE/Physics/RigidBody/CollisionBox.hpp"


namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {

            class CollisionDetector
            {
              public:
                static unsigned SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);
                static unsigned SphereAndTruePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);
                static unsigned SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data);

                static unsigned BoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data);
                static unsigned BoxAndBox(const CollisionBox& one, const CollisionBox& two, CollisionData* data);
                static unsigned BoxAndPoint(const CollisionBox& box, const Math::vec3f& point, CollisionData* data);
                static unsigned BoxAndSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data);

              protected:
                /**
                 * Metoda sprawdzająca czy dwa pudełka nakładają się na siebie wzdłuż podanej osi, zwracając wielkość nałożenia się.
                 */
                static inline float PenetrationOnAxis(const CollisionBox& one, const CollisionBox& two, const Math::vec3f& axis, const Math::vec3f& toCenter);
                static inline bool TryAxis(const CollisionBox& one, const CollisionBox& two, const Math::vec3f& axis2, const Math::vec3f& toCenter,
                                           unsigned index, float& smallestPentration, unsigned &smallestCase);
                static inline void FillPointFaceBoxBox(const CollisionBox& one, const CollisionBox& two, const Math::vec3f& toCenter, CollisionData* data, unsigned best, float pen);
                static inline Math::vec3f ContactPoint(const Math::vec3f& pOne, const Math::vec3f& dOne, float oneSize, const Math::vec3f& pTwo, const Math::vec3f& dTwo,
                                                       float twoSize, bool useOne);

            };
        }
    }
}

#endif	/* COLLISIONDETECTOR_HPP */

