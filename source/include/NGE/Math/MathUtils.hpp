/* 
 * File:   MathUtils.hpp
 * Author: tku
 *
 * Created on 9 styczeń 2012, 00:59
 */

#ifndef MATHUTILS_HPP
#define	MATHUTILS_HPP

#include <math.h>
#include <algorithm>

namespace NGE {
	namespace Math {

		class MathUtils {
		  public:
			static const double PI;

			template <typename T> static T DegToRad(T deg) {
				return deg * (T) PI / (T) 180.0;
			}

			template <typename T> static T RadToDeg(T rad) {
				return rad * (T) 180 / (T) PI;
			}

			template <typename T> inline static T Clamp(T x, T min, T max) {
				return (x < min) ? min : (x > max) ? max : x;
			}

			template <typename T, typename U> inline static T Rotl(T x, U y) {
				return (x << y) | (x >> ((sizeof (T) * 4) - y));
			}

			template <typename T, typename U> inline static T Rotr(T x, U y) {
				return (x >> y) | (x << ((sizeof (T) * 4) - y));
			}

			template <typename T> inline static bool SolveQuadratic(const T& a, const T& b, const T& c, T& x0, T& x1) {
				T discr = b * b - 4 * a * c;

				if (discr < 0)
					return false;
				else if (discr == 0)
					x0 = x1 = -0.5 * b / a;
				else {
					T q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
					x0 = q / a;
					x1 = c / q;
				}

				if (x0 > x1)
					std::swap(x0, x1);

				return true;
			}
		};
	}
}

#endif	/* MATHUTILS_HPP */
