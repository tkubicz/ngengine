/* 
 * File:   Ray.hpp
 * Author: tku
 *
 * Created on 30 maj 2012, 18:11
 */

#ifndef RAY_HPP
#define	RAY_HPP

#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/MathUtils.hpp"

namespace NGE {
	namespace Geometry {

		template <typename T> class Ray {
		  public:
			Math::Vector3<T> direction;
			Math::Vector3<T> position;

			Ray() { }

			Ray(Math::Vector3<T> direction, Math::Vector3<T> position) : direction(direction), position(position) { }

			bool IntersectSphere(const T& radius, const Math::Vector3<T>& center) {
				float t0, t1;

				Math::vec3f L = position - center;
				float a = direction.DotProduct(direction);
				float b = 2 * direction.DotProduct(L);
				float c = L.DotProduct(L) - (radius * radius);
				if (!Math::MathUtils::SolveQuadratic(a, b, c, t0, t1))
					return false;

				// TODO: Do poprawki, zrobić tak, by możliwe było przekazanie obu punktów przecięcia, a nie tylko jednego.
				//pointOfIntersection = t0;
				return true;
			}
		};
	}
}

#endif	/* RAY_HPP */

