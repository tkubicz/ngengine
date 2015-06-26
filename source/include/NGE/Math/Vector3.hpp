#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <cmath>

namespace NGE {
	namespace Math {

		template <typename T> class Vector3 {
		  public:
			T x, y, z;

			Vector3(const T x = 0, const T y = 0, const T z = 0) : x(x), y(y), z(z) { }

			Vector3(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z) { }

			inline Vector3 &operator=(const Vector3& vec);

			inline const T operator[](size_t i) const;
			inline T &operator[](size_t i);

			inline Vector3 operator+(const Vector3 &v) const;
			inline Vector3 operator-(const Vector3 &v) const;
			inline Vector3 operator*(const T scale) const;
			inline Vector3 operator/(const T scale) const;

			inline Vector3 &operator+=(const Vector3 &v);
			inline Vector3 &operator-=(const Vector3 &v);
			inline Vector3 &operator*=(const T scale);
			inline Vector3 &operator/=(const T scale);

			bool operator==(const Vector3 &v);
			bool operator!=(const Vector3 &v);

			inline const T Length() const;
			inline const T LengthSquared() const;
			inline void Normalize();

			inline Vector3 ComponentProduct(const Vector3 &v) const;
			inline void ComponentProductUpdate(const Vector3 &v);

			// Vector product
			inline Vector3 CrossProduct(const Vector3 &v) const;

			// Scalar product
			inline T DotProduct(const Vector3 &v) const;

			/**
			 * Metoda zwracająca odległość pomiędzy dwoma wektorami.
			 * @param v Drugi wektor pomiędzy którym liczona będzie odległość.
			 * @return Odległość między wektorami.
			 */
			inline const T GetDistance(const Vector3 &v) const;

			/**
			 * Metoda zwracająca kąt pomiędzy dwoma wektorami.
			 * @param v Drugi wektor pomiędzy którym liczony będzie kąt.
			 * @return Kąt pomiędzy wektorami w radianach.
			 */
			inline const T GetAngle(const Vector3 &v) const;

			/**
			 * Metoda nadająca wybrane wartości poszczególnym elementom wektora.
			 * @param nx Wartość na osi X.
			 * @param ny Wartość na osi Y.
			 * @param nz Wartość na osi Z.
			 */
			inline void Set(const T nx, const T ny, const T nz);

			/**
			 * Metoda odwracająca wektor.
			 */
			inline void Invert();

			/**
			 * Metoda czyszcząca wektor. Nadaje wartość 0 wszystkim parametrom.
			 */
			inline void Clear();
		};

		template <typename T> inline Vector3<T> &Vector3<T>::operator=(const Vector3<T>& vec) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}

		template <typename T> inline const T Vector3<T>::operator[](size_t i) const {
			return ((T*) & x)[i];
		}

		template <typename T> inline T &Vector3<T>::operator[](size_t i) {
			return ((T*) & x)[i];
		}

		template <typename T> inline Vector3<T> Vector3<T>::operator+(const Vector3<T> &v) const {
			return Vector3<T > (x + v.x, y + v.y, z + v.z);
		}

		template <typename T> inline Vector3<T> Vector3<T>::operator-(const Vector3<T> &v) const {
			return Vector3<T > (x - v.x, y - v.y, z - v.z);
		}

		template <typename T> inline Vector3<T> Vector3<T>::operator*(const T scale) const {
			return Vector3<T > (x * scale, y * scale, z * scale);
		}

		template <typename T> inline Vector3<T> Vector3<T>::operator/(const T scale) const {
			return scale ? Vector3<T > (x / scale, y / scale, z / scale) : Vector3<T > ();
		}

		template <typename T> inline Vector3<T> &Vector3<T>::operator+=(const Vector3<T> &v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		template <typename T> inline Vector3<T> &Vector3<T>::operator-=(const Vector3<T> &v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		template <typename T> inline Vector3<T> &Vector3<T>::operator*=(const T scale) {
			x *= scale;
			y *= scale;
			z *= scale;
			return *this;
		}

		template <typename T> inline Vector3<T> &Vector3<T>::operator/=(const T scale) {
			if (scale) {
				x /= scale;
				y /= scale;
				z /= scale;
			}
			return *this;
		}

		template <typename T> bool Vector3<T>::operator==(const Vector3<T> &v) {
			return (x == v.x && y == v.y && z == v.z);
		}

		template <typename T> bool Vector3<T>::operator!=(const Vector3<T> &v) {
			return !(x == v.x && y == v.y && z == v.z);
		}

		template <typename T> inline const T Vector3<T>::Length() const {
			return sqrt(x * x + y * y + z * z);
		}

		template <typename T> inline const T Vector3<T>::LengthSquared() const {
			return (x * x + y * y + z * z);
		}

		template <typename T> inline void Vector3<T>::Normalize() {
			T length = Length();
			if (length > 0)
				(*this) *= ((T) 1) / length;
		}

		template <typename T> inline Vector3<T> Vector3<T>::ComponentProduct(const Vector3<T> &v) const {
			return Vector3<T > (x * v.x, y * v.y, z * v.z);
		}

		template <typename T> inline void Vector3<T>::ComponentProductUpdate(const Vector3<T> &v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}

		template <typename T> inline Vector3<T> Vector3<T>::CrossProduct(const Vector3<T> &v) const {
			return Vector3<T > (y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}

		template <typename T> inline T Vector3<T>::DotProduct(const Vector3<T> &v) const {
			return (x * v.x + y * v.y + z * v.z);
		}

		template <typename T> inline const T Vector3<T>::GetDistance(const Vector3<T> &v) const {
			return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
		}

		template <typename T> inline const T Vector3<T>::GetAngle(const Vector3<T> &v) const {
			T angle = acos(DotProduct(v) / (Length() * v.Length()));
			return (isnan(angle) ? 0 : angle);
		}

		template <typename T> inline void Vector3<T>::Set(const T nx, const T ny, const T nz) {
			x = nx;
			y = ny;
			z = nz;
		}

		template <typename T> inline void Vector3<T>::Clear() {
			x = y = z = 0;
		}

		template <typename T> inline void Vector3<T>::Invert() {
			x = -x;
			y = -y;
			z = -z;
		}

		template <typename T> Vector3<T> operator*(float scalar, const Vector3<T>& v) {
			return v * scalar;
		}

		template <typename T> std::ostream &operator<<(std::ostream &out, const Vector3<T> &v) {
			return out << "(" << v.x << "," << v.y << "," << v.z << ")";
		}

		typedef Vector3<int> vec3i;
		typedef Vector3<float> vec3f;
		typedef Vector3<double> vec3d;
	}
}

#endif
