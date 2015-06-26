/* 
 * File:   Vector4.hpp
 * Author: tku
 *
 * Created on 29 styczeń 2012, 04:19
 */

#ifndef VECTOR4_HPP
#define	VECTOR4_HPP

#include "NGE/Math/Vector3.hpp"

namespace NGE {
	namespace Math {

		template<typename T> class Vector4 {
		  public:
			T x, y, z, w;

			Vector4(T x = 0, T y = 0, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) { }

			Vector4(const Vector4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }

			Vector4(const Vector3<T>& vec, const T w = 1) : x(vec.x), y(vec.y), z(vec.z), w(w) { }

			inline operator const T*() const {
				return &x;
			}

			inline operator const T*() {
				return &x;
			}

			inline const T operator[](unsigned i) const;
			inline T &operator[](unsigned i);

			inline Vector4 &operator=(const Vector4& vec);
			inline Vector4 &operator=(const Vector3<T>& vec);

			inline Vector4 &operator/=(const T scale);

			inline void Set(T x, T y, T z, T w);
			inline void Set(const Vector4<T> &vec);
			inline void Set(const Vector3<T> &vec, T w);
		};

		template <typename T> inline Vector4<T> &Vector4<T>::operator=(const Vector4<T>& vec) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
			return *this;
		}

		template <typename T> inline Vector4<T> &Vector4<T>::operator=(const Vector3<T>& vec) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = 1;
			return *this;
		}

		template <typename T> inline const T Vector4<T>::operator[](unsigned i) const {
			return ((T*) & x)[i];
		}

		template <typename T> inline T &Vector4<T>::operator[](unsigned i) {
			return ((T*) & x)[i];
		}

		template <typename T> inline Vector4<T> &Vector4<T>::operator/=(const T scale) {
			if (scale) {
				x /= scale;
				y /= scale;
				z /= scale;
				w /= scale;
			}
			return *this;
		}

		template <typename T> void Vector4<T>::Set(T x, T y, T z, T w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		template <typename T> void Vector4<T>::Set(const Vector4<T>& vec) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
		}

		template <typename T> void Vector4<T>::Set(const Vector3<T>& vec, T w) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
			this->w = w;
		}

		template <typename T> std::ostream &operator<<(std::ostream &out, const Vector4<T> &v) {
			return out << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
		}

		typedef Vector4<int> vec4i;
		typedef Vector4<float> vec4f;
		typedef Vector4<double> vec4d;
	}
}

#endif	/* VECTOR4_HPP */

