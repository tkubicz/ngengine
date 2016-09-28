#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include <iostream>
#include <cstring>
#include <cmath>

#include "NGE/Math/Matrix3.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Math/MathUtils.hpp"

namespace NGE {
	namespace Math {

		template <typename T> class Matrix4 {
		  private:
			T m[16];

		  public:
			Matrix4();
			Matrix4(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15);
			Matrix4(const T* elements);
			Matrix4(const Matrix4<T> &mat);

			inline Matrix4 operator+(const Matrix4 &mat) const;
			inline Matrix4 operator-(const Matrix4 &mat) const;
			inline Matrix4 operator*(const Matrix4 &mat) const;

			inline Matrix4 &operator+=(const Matrix4 &mat);
			inline Matrix4 &operator-=(const Matrix4 &mat);
			inline Matrix4 &operator*=(const T val);
			inline Matrix4 &operator*=(const Matrix4 &mat);

			inline bool operator==(const Matrix4 &mat);
			inline bool operator!=(const Matrix4 &mat);
			inline Matrix4 &operator=(const Matrix4 &mat);

			inline operator const T*() const;
			inline operator T*();

			inline Matrix4 &LookAt(T eyeX, T eyeY, T eyeZ, T centerX, T centerY, T cebterZ, T upX, T upY, T upZ);
			inline Matrix4 &LookAt(Vector3<T> &eyePosition, Vector3<T> &center, Vector3<T> &upVector);

			void Set(const Matrix4<T> &mat);
			void Set(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15);
			void Set(const T* elem);
			void SetValueAt(int index, T value);

			inline void SetPerspectiveProjection(T fov, T aspect, T Near, T Far);
			inline void SetOrthographicProjection(T left, T right, T bottom, T top, T Near, T Far);

			//inline T GetDeterminant();
			inline bool SetInverse();
			inline void SetTranspose();
			inline void SetIdentity();
			//inline void SetInverseTranspose();

			inline void SetInverseRigidBody();

			inline Matrix3<T> GetNormalMatrix();

			inline Matrix4 Transpose();

			inline void Scale(const Vector3<T> &axis);
			inline void Scale(T x, T y, T z);

			inline Vector3<T> GetScale();
			inline void GetScale(Vector3<T>& axis);
			inline void GetScale(T scales[3]);

			/**
			 * Create rotation matrix from provided angle and axis.
			 * @param angle Angle in radians.
			 * @param axis Vector3 axis.
			 */
			inline void RotateAxis(T angle, const Vector3<T> &axis);

			/**
			 * Create an X-axis rotation matrix.
			 * Camera roll.
			 * @param angle Angle in radians.
			 */
			inline void RotateX(const T angle);

			/**
			 * Create an Y-axis rotation matrix.
			 * Camera pitch.
			 * @param angle Angle in radians.
			 */
			inline void RotateY(const T angle);

			/**
			 * Create an Z-axis rotation matrix.
			 * Camera yaw.
			 * @param angle Angle in radians.
			 */
			inline void RotateZ(const T angle);

			// inline void RotateXYZ(const Vector3<T> &vec);
			inline void RotateXYZ(float x, float y, float z);

			inline void Translate(const T x, const T y, const T z);
			inline void Translate(const Vector3<T> &v);

			inline Vector3<T> TransformInverse(const Vector3<T>& vec) const;

			inline Vector3<T> TransformDirection(const Vector3<T>& vec) const;
			inline Vector3<T> TransformInverseDirection(const Vector3<T>& vec) const;

			inline Vector3<T> GetAxisVector(unsigned index) const;

			inline Matrix3<T> ExtractMatrix3() const;
			inline void ExtractMatrix3(const Matrix3<T> &mat);
		};

		template <typename T> Matrix4<T>::Matrix4() {
			SetIdentity();
		}

		template <typename T> Matrix4<T>::Matrix4(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15) {
			Set(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15);
		}

		template <typename T> Matrix4<T>::Matrix4(const T* elements) {
			Set(elements);
		}

		template <typename T> Matrix4<T>::Matrix4(const Matrix4<T> &mat) {
			Set(mat);
		}

		template <typename T> inline Matrix4<T> Matrix4<T>::operator+(const Matrix4<T> &mat) const {
			return Matrix4<T > (m[0] + mat[0], m[1] + mat[1], m[2] + mat[2], m[3] + mat[3],
					m[4] + mat[4], m[5] + mat[5], m[6] + mat[6], m[7] + mat[7],
					m[8] + mat[8], m[9] + mat[9], m[10] + mat[10], m[11] + mat[11],
					m[12] + mat[12], m[13] + mat[13], m[14] + mat[14], m[15] + mat[15]);
		}

		template <typename T> inline Matrix4<T> Matrix4<T>::operator-(const Matrix4<T> &mat) const {
			return Matrix4<T > (m[0] - mat[0], m[1] - mat[1], m[2] - mat[2], m[3] - mat[3],
					m[4] - mat[4], m[5] - mat[5], m[6] - mat[6], m[7] - mat[7],
					m[8] - mat[8], m[9] - mat[9], m[10] - mat[10], m[11] - mat[11],
					m[12] - mat[12], m[13] - mat[13], m[14] - mat[14], m[15] - mat[15]);
		}

		template <typename T> inline Matrix4<T> Matrix4<T>::operator*(const Matrix4<T> &mat) const {
			return Matrix4(m[0] * mat[0] + m[4] * mat[1] + m[8] * mat[2] + m[12] * mat[3],
					m[1] * mat[0] + m[5] * mat[1] + m[9] * mat[2] + m[13] * mat[3],
					m[2] * mat[0] + m[6] * mat[1] + m[10] * mat[2] + m[14] * mat[3],
					m[3] * mat[0] + m[7] * mat[1] + m[11] * mat[2] + m[15] * mat[3],
					m[0] * mat[4] + m[4] * mat[5] + m[8] * mat[6] + m[12] * mat[7],
					m[1] * mat[4] + m[5] * mat[5] + m[ 9] * mat[6] + m[13] * mat[7],
					m[2] * mat[4] + m[6] * mat[5] + m[10] * mat[6] + m[14] * mat[7],
					m[3] * mat[4] + m[7] * mat[5] + m[11] * mat[6] + m[15] * mat[7],
					m[0] * mat[8] + m[4] * mat[9] + m[8] * mat[10] + m[12] * mat[11],
					m[1] * mat[8] + m[5] * mat[9] + m[9] * mat[10] + m[13] * mat[11],
					m[2] * mat[8] + m[6] * mat[9] + m[10] * mat[10] + m[14] * mat[11],
					m[3] * mat[8] + m[7] * mat[9] + m[11] * mat[10] + m[15] * mat[11],
					m[0] * mat[12] + m[4] * mat[13] + m[ 8] * mat[14] + m[12] * mat[15],
					m[1] * mat[12] + m[5] * mat[13] + m[ 9] * mat[14] + m[13] * mat[15],
					m[2] * mat[12] + m[6] * mat[13] + m[10] * mat[14] + m[14] * mat[15],
					m[3] * mat[12] + m[7] * mat[13] + m[11] * mat[14] + m[15] * mat[15]);
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::operator+=(const Matrix4<T> &mat) {
			for (int i = 0; i < 16; ++i)
				m[i] += mat[i];
			return *this;
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::operator-=(const Matrix4<T> &mat) {
			for (int i = 0; i < 16; ++i)
				m[i] -= mat[i];
			return *this;
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::operator*=(const T val) {
			for (int i = 0; i < 16; ++i)
				m[i] *= val;
			return *this;
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::operator*=(const Matrix4<T> &mat) {
			Set(m[0] * mat[ 0] + m[4] * mat[ 1] + m[ 8] * mat[ 2] + m[12] * mat[ 3],
					m[1] * mat[ 0] + m[5] * mat[ 1] + m[ 9] * mat[ 2] + m[13] * mat[ 3],
					m[2] * mat[ 0] + m[6] * mat[ 1] + m[10] * mat[ 2] + m[14] * mat[ 3],
					m[3] * mat[ 0] + m[7] * mat[ 1] + m[11] * mat[ 2] + m[15] * mat[ 3],
					m[0] * mat[ 4] + m[4] * mat[ 5] + m[ 8] * mat[ 6] + m[12] * mat[ 7],
					m[1] * mat[ 4] + m[5] * mat[ 5] + m[ 9] * mat[ 6] + m[13] * mat[ 7],
					m[2] * mat[ 4] + m[6] * mat[ 5] + m[10] * mat[ 6] + m[14] * mat[ 7],
					m[3] * mat[ 4] + m[7] * mat[ 5] + m[11] * mat[ 6] + m[15] * mat[ 7],
					m[0] * mat[ 8] + m[4] * mat[ 9] + m[ 8] * mat[10] + m[12] * mat[11],
					m[1] * mat[ 8] + m[5] * mat[ 9] + m[ 9] * mat[10] + m[13] * mat[11],
					m[2] * mat[ 8] + m[6] * mat[ 9] + m[10] * mat[10] + m[14] * mat[11],
					m[3] * mat[ 8] + m[7] * mat[ 9] + m[11] * mat[10] + m[15] * mat[11],
					m[0] * mat[12] + m[4] * mat[13] + m[ 8] * mat[14] + m[12] * mat[15],
					m[1] * mat[12] + m[5] * mat[13] + m[ 9] * mat[14] + m[13] * mat[15],
					m[2] * mat[12] + m[6] * mat[13] + m[10] * mat[14] + m[14] * mat[15],
					m[3] * mat[12] + m[7] * mat[13] + m[11] * mat[14] + m[15] * mat[15]);
			return *this;
		}

		template <typename T> inline bool Matrix4<T>::operator==(const Matrix4 &mat) {
			return memcmp(m, mat, sizeof (T)*16) == 0;
		}

		template <typename T> inline bool Matrix4<T>::operator!=(const Matrix4 &mat) {
			return memcmp(m, mat, sizeof (T)*16) != 0;
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::operator=(const Matrix4 &mat) {
			memcpy(m, mat, sizeof (T) * 16);
			return *this;
		}

		template <typename T> inline Matrix4<T>::operator const T*() const {
			return m;
		}

		template <typename T> inline Matrix4<T>::operator T*() {
			return m;
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::LookAt(T eyeX, T eyeY, T eyeZ, T centerX, T centerY, T centerZ, T upX, T upY, T upZ) {
			Vector3<T> eye(eyeX, eyeY, eyeZ),
					center(centerX, centerY, centerZ),
					up(upX, upY, upZ);

			return LookAt(eye, center, up);
		}

		template <typename T> inline Matrix4<T> &Matrix4<T>::LookAt(Vector3<T> &eyePosition, Vector3<T> &center, Vector3<T> &upVector) {
			SetIdentity();

			Vector3<T> f = center - eyePosition;
			f.Normalize();

			upVector.Normalize();

			Vector3<T> s = f.CrossProduct(upVector);
			s.Normalize();

			Vector3<T> u = s.CrossProduct(f);
			s.Normalize();

			m[0] = s[0];
			m[4] = s[1];
			m[8] = s[2];

			m[1] = u[0];
			m[5] = u[1];
			m[9] = u[2];

			m[2] = -f[0];
			m[6] = -f[1];
			m[10] = -f[2];

			Matrix4<T> mat;
			mat.Translate(-eyePosition.x, -eyePosition.y, -eyePosition.z);
			*this *= mat;

			return *this;
		}

		template <typename T> inline void Matrix4<T>::Set(const Matrix4<T> &mat) {
			memcpy(m, mat.m, sizeof (T) * 16);
		}

		template <typename T> inline void Matrix4<T>::Set(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15) {
			m[0] = m0;
			m[1] = m1;
			m[2] = m2;
			m[3] = m3;
			m[4] = m4;
			m[5] = m5;
			m[6] = m6;
			m[7] = m7;
			m[8] = m8;
			m[9] = m9;
			m[10] = m10;
			m[11] = m11;
			m[12] = m12;
			m[13] = m13;
			m[14] = m14;
			m[15] = m15;
		}

		template <typename T> inline void Matrix4<T>::Set(const T* elem) {
			memcpy(m, elem, sizeof (T) * 16);
		}

		template <typename T> inline void Matrix4<T>::SetValueAt(int index, T value) {
			if (index < 0 || index > 15)
				return;
			m[index] = value;
		}

		template <typename T> void Matrix4<T>::SetPerspectiveProjection(T fov, T aspect, T Near, T Far) {
			T r = MathUtils::DegToRad(fov / 2);
			T delta = Far - Near;
			T s = sin(r);
			T cotangent = 0;

			if (delta == 0 || s == 0 || aspect == 0)
				return;

			cotangent = cos(r) / s;
			SetIdentity();

			m[0] = cotangent / aspect;
			m[5] = cotangent;
			m[10] = -(Far + Near) / delta;
			m[11] = -1;
			m[14] = -2 * Near * Far / delta;
			m[15] = 0;
		}

		template <typename T> void Matrix4<T>::SetOrthographicProjection(T left, T right, T bottom, T top, T Near, T Far) {
			T x = (right + left) / (right - left);
			T y = (top + bottom) / (top - bottom);
			T z = (Far + Near) / (Far - Near);
			SetIdentity();

			m[0] = 2 / (right - left);
			m[5] = 2 / (top - bottom);
			m[10] = -2 / (Far - Near);
			m[12] = -x;
			m[13] = -y;
			m[14] = -z;
		}

		template <typename T> inline bool Matrix4<T>::SetInverse() {
			int i, j, k, swap;
			Matrix4<T> temp(m);
			T t;

			SetIdentity();

			for (i = 0; i < 4; i++) {
				swap = i;
				for (j = i + 1; j < 4; j++) {
					if (fabs(temp[j * 4 + i]) > fabs(temp[i * 4 + i])) {
						swap = j;
					}
				}

				if (swap != i) {
					for (k = 0; k < 4; k++) {
						t = temp[i * 4 + k];
						temp[i * 4 + k] = temp[swap * 4 + k];
						temp[swap * 4 + k] = t;

						t = m[i * 4 + k];
						m[i * 4 + k] = m[swap * 4 + k];
						m[swap * 4 + k] = t;
					}
				}

				if (!temp[i * 4 + i])
					return false;

				t = temp[i * 4 + i];

				for (k = 0; k < 4; k++) {
					temp[i * 4 + k] /= t;
					m[i * 4 + k] = m[i * 4 + k] / t;
				}

				for (j = 0; j < 4; j++) {
					if (j != i) {
						t = temp[j * 4 + i];
						for (k = 0; k < 4; k++) {
							temp[j * 4 + k] -= temp[i * 4 + k] * t;
							m[j * 4 + k] = m[j * 4 + k] - m[i * 4 + k] * t;
						}
					}
				}
			}
			return true;
		}

		template <typename T> inline void Matrix4<T>::SetTranspose() {
			Matrix4<T> temp;

			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					temp[(i * 4) + j] = m[(j * 4) + i];

			memcpy(m, temp.m, sizeof (T) * 16);
		}

		template <typename T> inline void Matrix4<T>::SetIdentity() {
			Set(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
		}

		template <typename T> inline void Matrix4<T>::SetInverseRigidBody() {
			NGE::Math::Vector3<T> scales;
			NGE::Math::Matrix4<T> inverseScale;
			NGE::Math::Matrix4<T> inverseRotation;
			NGE::Math::Matrix4<T> inverseTranslation;

			scales = this->GetScale();

			inverseScale[0] = 1.0f / (scales[0] * scales[0]);
			inverseScale[5] = 1.0f / (scales[1] * scales[1]);
			inverseScale[10] = 1.0f / (scales[2] * scales[2]);

			// Copy only rotation
			inverseRotation.Set(*this);
			inverseRotation[3] = 0.0f;
			inverseRotation[7] = 0.0f;
			inverseRotation[11] = 0.0f;
			inverseRotation[12] = 0.0f;
			inverseRotation[13] = 0.0f;
			inverseRotation[14] = 0.0f;
			inverseRotation[15] = 1.0f;
			inverseRotation.SetTranspose();

			inverseTranslation[12] = -m[12];
			inverseTranslation[13] = -m[13];
			inverseTranslation[14] = -m[14];

			NGE::Math::Matrix4<T> result = inverseRotation * inverseTranslation;
			result = inverseScale * result;

			Set(result);
		}

		template <typename T> inline Matrix3<T> Matrix4<T>::GetNormalMatrix() {
			Matrix3<T> M;
			M[0] = m[0];
			M[1] = m[1];
			M[2] = m[2];
			M[3] = m[4];
			M[4] = m[5];
			M[5] = m[6];
			M[6] = m[8];
			M[7] = m[9];
			M[8] = m[10];

			//Work out the determinate
			float determinate = M[0] * M[4] * M[8] + M[1] * M[5] * M[6] + M[2] * M[3] * M[7];
			determinate -= M[2] * M[4] * M[6] + M[0] * M[5] * M[7] + M[1] * M[3] * M[8];

			//One division is faster than several
			float oneOverDet = 1.0f / determinate;

			Matrix3<T> N;

			//Calculate the inverse and assign it to the transpose matrix positions
			N[0] = (M[4] * M[8] - M[5] * M[7]) * oneOverDet;
			N[3] = (M[2] * M[7] - M[1] * M[8]) * oneOverDet;
			N[6] = (M[1] * M[5] - M[2] * M[4]) * oneOverDet;

			N[1] = (M[5] * M[6] - M[3] * M[8]) * oneOverDet;
			N[4] = (M[0] * M[8] - M[2] * M[6]) * oneOverDet;
			N[7] = (M[2] * M[3] - M[0] * M[5]) * oneOverDet;

			N[2] = (M[3] * M[7] - M[4] * M[6]) * oneOverDet;
			N[5] = (M[6] * M[1] - M[0] * M[7]) * oneOverDet;
			N[8] = (M[0] * M[4] - M[1] * M[3]) * oneOverDet;

			return N;
		}

		template <typename T> inline Matrix4<T> Matrix4<T>::Transpose() {
			Matrix4<T> temp;

			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					temp[(i * 4) + j] = m[(j * 4) + i];

			return temp;
		}

		template <typename T> inline void Matrix4<T>::Scale(const Vector3<T>& axis) {
			SetIdentity();
			m[0] = axis.x;
			m[5] = axis.y;
			m[10] = axis.z;
		}

		template <typename T> inline void Matrix4<T>::Scale(T x, T y, T z) {
			SetIdentity();
			m[0] = x;
			m[5] = y;
			m[10] = z;
		}

		template <typename T> inline Math::Vector3<T> Matrix4<T>::GetScale() {
			T scales[3];
			for (int i = 0; i < 3; ++i)
				scales[i] = std::sqrt(m[4 * i] * m[4 * i] + m[4 * i + 1] * m[4 * i + 1] + m[4 * i + 2] * m[4 * i + 2]);

			return Math::Vector3<T>(scales[0], scales[1], scales[2]);
		}

		template <typename T> inline void Matrix4<T>::GetScale(Vector3<T>& axis) {
			axis = GetScale();
		}

		template <typename T> inline void Matrix4<T>::GetScale(T scales[3]) {
			Math::Vector3<T> axis = GetScale();
			scales[0] = axis.x;
			scales[1] = axis.y;
			scales[2] = axis.z;
		}

		template <typename T> inline void Matrix4<T>::RotateAxis(T angle, const Vector3<T>& axis) {
			T sinAngle = std::sin(angle);
			T cosAngle = std::cos(angle);
			T oneMinusCosAngle = (T) 1.0 - cosAngle;

			SetIdentity();
			//axis.Normalize();

			m[0] = cosAngle + axis.x * axis.x * oneMinusCosAngle;
			m[4] = -axis.z * sinAngle + axis.x * axis.y * oneMinusCosAngle;
			m[8] = axis.y * sinAngle + axis.x * axis.z * oneMinusCosAngle;

			m[1] = axis.z * sinAngle + axis.y * axis.x * oneMinusCosAngle;
			m[5] = cosAngle + axis.y * axis.y * oneMinusCosAngle;
			m[9] = -axis.x * sinAngle + axis.y * axis.z * oneMinusCosAngle;

			m[2] = -axis.y * sinAngle + axis.z * axis.z * oneMinusCosAngle;
			m[6] = axis.x * sinAngle + axis.z * axis.y * oneMinusCosAngle;
			m[10] = cosAngle + axis.z * axis.z * oneMinusCosAngle;
		}

		template <typename T> inline void Matrix4<T>::RotateX(const T angle) {
			SetIdentity();

			T sinAngle = std::sin(angle);
			T cosAngle = std::cos(angle);

			m[5] = cosAngle;
			m[6] = sinAngle;
			m[9] = -sinAngle;
			m[10] = cosAngle;
		}

		template <typename T> inline void Matrix4<T>::RotateY(const T angle) {
			SetIdentity();

			T sinAngle = std::sin(angle);
			T cosAngle = std::cos(angle);

			m[0] = cosAngle;
			m[2] = -sinAngle;
			m[8] = sinAngle;
			m[10] = cosAngle;
		}

		template <typename T> inline void Matrix4<T>::RotateZ(const T angle) {
			SetIdentity();

			T sinAngle = std::sin(angle);
			T cosAngle = std::cos(angle);

			m[0] = cosAngle;
			m[1] = sinAngle;
			m[4] = -sinAngle;
			m[5] = cosAngle;
		}

		template <typename T> inline void Matrix4<T>::RotateXYZ(float x, float y, float z) {
			SetIdentity();

			T sinX = std::sin(x);
			T cosX = std::cos(x);

			T sinY = std::sin(y);
			T cosY = std::cos(y);

			T sinZ = std::sin(z);
			T cosZ = std::cos(z);

			m[0] = cosZ * cosY;
			m[1] = sinZ * cosY;
			m[2] = -sinY;

			m[4] = cosZ * sinY * sinX - sinZ * cosX;
			m[5] = sinZ * sinY * sinX + cosZ * cosX;
			m[6] = cosY * sinX;

			m[8] = cosZ * sinY * cosX + sinZ * sinX;
			m[9] = sinZ * sinY * cosX - cosZ * sinX;
			m[10] = cosY * cosX;
		}

		template <typename T> inline void Matrix4<T>::Translate(const T x, const T y, const T z) {
			SetIdentity();
			m[12] = x;
			m[13] = y;
			m[14] = z;
		}

		template <typename T> inline void Matrix4<T>::Translate(const Vector3<T>& v) {
			SetIdentity();
			m[12] = v.x;
			m[13] = v.y;
			m[14] = v.z;
		}

		template <typename T> inline Vector3<T> Matrix4<T>::TransformInverse(const Vector3<T>& vec) const {
			Vector3<T> temp = vec;
			temp.x -= m[12];
			temp.y -= m[13];
			temp.z -= m[14];

			return Vector3<T>(temp.x * m[0] + temp.y * m[1] + temp.z * m[2],
					temp.x * m[4] + temp.y * m[5] + temp.z * m[6],
					temp.x * m[8] + temp.y * m[9] + temp.z * m[10]);
		}

		template <typename T> inline Vector3<T> Matrix4<T>::TransformDirection(const Vector3<T>& vec) const {
			return Vector3<T>(vec.x * m[0] + vec.y * m[4] + vec.z * m[8],
					vec.x * m[1] + vec.y * m[5] + vec.z * m[9],
					vec.x * m[2] + vec.y * m[6] + vec.z * m[10]);
		}

		template <typename T> inline Vector3<T> Matrix4<T>::TransformInverseDirection(const Vector3<T>& vec) const {
			return Vector3<T>(vec.x * m[0] + vec.y * m[1] + vec.z * m[2],
					vec.x * m[4] + vec.y * m[5] + vec.z * m[6],
					vec.x * m[8] + vec.y * m[9] + vec.z * m[10]);
		}

		template <typename T> inline Vector3<T> Matrix4<T>::GetAxisVector(unsigned index) const {
			unsigned i = index * 4;
			return Vector3<T>(m[i], m[i + 1], m[i + 2]);
		}

		template <typename T> inline Matrix3<T> Matrix4<T>::ExtractMatrix3() const {
			Matrix3<T> val;

			val[0] = m[0];
			val[1] = m[1];
			val[2] = m[2];

			val[3] = m[4];
			val[4] = m[5];
			val[5] = m[6];

			val[6] = m[8];
			val[7] = m[9];
			val[8] = m[10];

			return val;
		}

		template <typename T> inline void Matrix4<T>::ExtractMatrix3(const Matrix3<T>& mat) {
			mat[0] = m[0];
			mat[1] = m[1];
			mat[2] = m[2];

			mat[3] = m[4];
			mat[4] = m[5];
			mat[5] = m[6];

			mat[6] = m[8];
			mat[7] = m[9];
			mat[8] = m[10];
		}

		template <typename T> Vector3<T> operator*(const Vector3<T> &vec, const Matrix4<T> &mat) {
			return Vector3<T > (mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12],
					mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13],
					mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14]);
		}

		template <typename T> Vector3<T> operator*(const Matrix4<T> &mat, const Vector3<T> &vec) {
			return Vector3<T > (mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12],
					mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13],
					mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14]);
		}

		template <typename T> void operator*=(Vector3<T>& vec, const Matrix4<T>& mat) {
			vec.Set(mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12],
					mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13],
					mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14]);
		}

		template <typename T> Vector4<T> operator*(const Vector4<T> &vec, const Matrix4<T> &mat) {
			return Vector4<T > (mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12] * vec.w,
					mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13] * vec.w,
					mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14] * vec.w,
					mat[3] * vec.x + mat[7] * vec.y + mat[11] * vec.z + mat[15] * vec.w);
		}

		template <typename T> Vector4<T> operator*(const Matrix4<T> &mat, const Vector4<T> &vec) {
			return Vector4<T > (mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12] * vec.w,
					mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13] * vec.w,
					mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14] * vec.w,
					mat[3] * vec.x + mat[7] * vec.y + mat[11] * vec.z + mat[15] * vec.w);
		}

		template <typename T> void operator*=(Vector4<T>& vec, const Matrix4<T>& mat) {
			vec.Set(mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12] * vec.w,
					mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13] * vec.w,
					mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14] * vec.w,
					mat[3] * vec.x + mat[7] * vec.y + mat[11] * vec.z + mat[15] * vec.w);
		}

		template <typename T> std::ostream & operator<<(std::ostream &out, const Matrix4<T> &right) {
			return out << "[" << right[0] << ", " << right[4] << ", " << right[8] << ", " << right[12] << "],\n"
					"[" << right[1] << ", " << right[5] << ", " << right[9] << ", " << right[13] << "],\n"
					"[" << right[2] << ", " << right[6] << ", " << right[10] << ", " << right[14] << "],\n"
					"[" << right[3] << ", " << right[7] << ", " << right[11] << ", " << right[15] << "]]";
		}

		typedef Matrix4<int> mat4i;
		typedef Matrix4<float> mat4f;
		typedef Matrix4<double> mat4d;
	}
}

#endif
