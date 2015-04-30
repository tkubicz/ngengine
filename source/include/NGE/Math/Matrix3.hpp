/* 
 * File:   Matrix3.hpp
 * Author: tku
 *
 * Created on 16 kwiecień 2013, 00:43
 */

#ifndef MATRIX3_HPP
#define	MATRIX3_HPP

#include <iostream>
#include <cstring>

#include "NGE/Math/Vector3.hpp"

namespace NGE
{
    namespace Math
    {

        template <typename T> class Matrix3
        {
          private:
            T m[9];

          public:
            Matrix3();
            Matrix3(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8);
            Matrix3(const T* elements);
            Matrix3(const Matrix3<T> &mat);

            inline Matrix3 operator +(const Matrix3 &mat) const;
            inline Matrix3 operator -(const Matrix3 &mat) const;
            inline Matrix3 operator *(const Matrix3 &mat) const;

            inline Matrix3 &operator +=(const Matrix3 &mat);
            inline Matrix3 &operator -=(const Matrix3 &mat);
            inline Matrix3 &operator *=(const T val);
            inline Matrix3 &operator *=(const Matrix3 &mat);

            inline bool operator ==(const Matrix3 &mat);
            inline bool operator !=(const Matrix3 &mat);
            inline Matrix3 &operator =(const Matrix3 &mat);

            inline operator const T*() const;
            inline operator T*();

            bool Equals(const Matrix3& mat, const T delta = 0.000001f);

            void Set(const Matrix3<T> &mat);
            void Set(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8);
            void Set(const T* elem);
            void SetValueAt(int index, T value);

            inline void SetIdentity();
            inline bool SetInverse(T epsilon = 0.0001);
            inline void SetTranspose();

            inline Matrix3<T> Transpose();
            inline Matrix3<T> Invserse();

            inline void SetSkewSymmetric(const Vector3<T>& vec);
            inline Vector3<T> TransformTranspose(const Vector3<T>& vec);
            inline void SetInertiaTensorCoeffs(T ix, T iy, T iz, T ixy = 0, T ixz = 0, T iyz = 0);
            inline void SetBlockInertiaTensor(const Vector3<T>& halfSize, T mass);

            inline Matrix3<T> LinearInterpolate(const Matrix3<T>& matrix, T prop);
        };

        template <typename T> Matrix3<T>::Matrix3()
        {
            SetIdentity();
        }

        template <typename T> Matrix3<T>::Matrix3(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8)
        {
            Set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
        }

        template <typename T> Matrix3<T>::Matrix3(const T* elements)
        {
            Set(elements);
        }

        template <typename T> Matrix3<T>::Matrix3(const Matrix3<T> &mat)
        {
            Set(mat);
        }

        template <typename T> inline Matrix3<T> Matrix3<T>::operator +(const Matrix3<T> &mat) const
        {
            return Matrix3<T > (m[0] + mat[0], m[1] + mat[1], m[2] + mat[2], m[3] + mat[3],
                                m[4] + mat[4], m[5] + mat[5], m[6] + mat[6], m[7] + mat[7],
                                m[8] + mat[8]);
        }

        template <typename T> inline Matrix3<T> Matrix3<T>::operator -(const Matrix3<T> &mat) const
        {
            return Matrix3<T > (m[0] - mat[0], m[1] - mat[1], m[2] - mat[2], m[3] - mat[3],
                                m[4] - mat[4], m[5] - mat[5], m[6] - mat[6], m[7] - mat[7],
                                m[8] - mat[8]);
        }

        // TODO: Mnożenie macierzy 3x3

        template <typename T> inline Matrix3<T> Matrix3<T>::operator *(const Matrix3<T> &mat) const
        {
            return Matrix3(m[0] * mat[0] + m[3] * mat[1] + m[6] * mat[2],
                           m[1] * mat[0] + m[4] * mat[1] + m[7] * mat[2],
                           m[2] * mat[0] + m[5] * mat[1] + m[8] * mat[2],
                           m[0] * mat[3] + m[3] * mat[4] + m[6] * mat[5],
                           m[1] * mat[3] + m[4] * mat[4] + m[7] * mat[5],
                           m[2] * mat[3] + m[5] * mat[4] + m[8] * mat[5],
                           m[0] * mat[6] + m[3] * mat[7] + m[6] * mat[8],
                           m[1] * mat[6] + m[4] * mat[7] + m[7] * mat[8],
                           m[2] * mat[6] + m[5] * mat[7] + m[8] * mat[8]);
        }

        template <typename T> inline Matrix3<T> &Matrix3<T>::operator +=(const Matrix3<T> &mat)
        {
            for (int i = 0; i < 9; ++i)
                m[i] += mat[i];
            return *this;
        }

        template <typename T> inline Matrix3<T> &Matrix3<T>::operator -=(const Matrix3<T> &mat)
        {
            for (int i = 0; i < 9; ++i)
                m[i] -= mat[i];
            return *this;
        }

        template <typename T> inline Matrix3<T> &Matrix3<T>::operator *=(const T val)
        {
            for (int i = 0; i < 9; ++i)
                m[i] *= val;
            return *this;
        }

        // TODO: Mnożenie macierzy 3x3

        template <typename T> inline Matrix3<T> &Matrix3<T>::operator *=(const Matrix3<T> &mat)
        {
            Set(m[0] * mat[0] + m[3] * mat[1] + m[6] * mat[2],
                m[1] * mat[0] + m[4] * mat[1] + m[7] * mat[2],
                m[2] * mat[0] + m[5] * mat[1] + m[8] * mat[2],
                m[0] * mat[3] + m[3] * mat[4] + m[6] * mat[5],
                m[1] * mat[3] + m[4] * mat[4] + m[7] * mat[5],
                m[2] * mat[3] + m[5] * mat[4] + m[8] * mat[5],
                m[0] * mat[6] + m[3] * mat[7] + m[6] * mat[8],
                m[1] * mat[6] + m[4] * mat[7] + m[7] * mat[8],
                m[2] * mat[6] + m[5] * mat[7] + m[8] * mat[8]);
            return *this;
        }

        template <typename T> inline bool Matrix3<T>::operator ==(const Matrix3 &mat)
        {
            return memcmp(m, mat, sizeof (T) * 9) == 0;
        }

        template <typename T> inline bool Matrix3<T>::operator !=(const Matrix3 &mat)
        {
            return memcmp(m, mat, sizeof (T) * 9) != 0;
        }

        template <typename T> inline Matrix3<T> &Matrix3<T>::operator =(const Matrix3 &mat)
        {
            memcpy(m, mat, sizeof (T) * 9);
            return *this;
        }

        template <typename T> inline Matrix3<T>::operator const T*() const
        {
            return m;
        }

        template <typename T> inline Matrix3<T>::operator T*()
        {
            return m;
        }

        template <typename T> inline bool Matrix3<T>::Equals(const Matrix3& mat, const T delta)
        {
            if ((m[0] <= mat[0] + delta && m[0] >= mat[0] - delta) &&
                (m[1] <= mat[1] + delta && m[1] >= mat[1] - delta) &&
                (m[2] <= mat[2] + delta && m[2] >= mat[2] - delta) &&
                (m[3] <= mat[3] + delta && m[3] >= mat[3] - delta) &&
                (m[4] <= mat[4] + delta && m[4] >= mat[4] - delta) &&
                (m[5] <= mat[5] + delta && m[5] >= mat[5] - delta) &&
                (m[6] <= mat[6] + delta && m[6] >= mat[6] - delta) &&
                (m[7] <= mat[7] + delta && m[7] >= mat[7] - delta) &&
                (m[8] <= mat[8] + delta && m[8] >= mat[8] - delta))
                return true;

            return false;
        }

        template <typename T> inline void Matrix3<T>::Set(const Matrix3<T> &mat)
        {
            memcpy(m, mat.m, sizeof (T) * 9);
        }

        template <typename T> inline void Matrix3<T>::Set(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8)
        {
            m[0] = m0;
            m[1] = m1;
            m[2] = m2;
            m[3] = m3;
            m[4] = m4;
            m[5] = m5;
            m[6] = m6;
            m[7] = m7;
            m[8] = m8;
        }

        template <typename T> inline void Matrix3<T>::Set(const T* elem)
        {
            memcpy(m, elem, sizeof (T) * 9);
        }

        template <typename T> inline void Matrix3<T>::SetValueAt(int index, T value)
        {
            if (index < 0 || index > 8)
                return;
            m[index] = value;
        }

        template <typename T> inline void Matrix3<T>::SetIdentity()
        {
            Set(1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0);
        }

        template <typename T> inline bool Matrix3<T>::SetInverse(T epsilon)
        {
            Matrix3 inverse;
            
            inverse[0] = m[4] * m[8] - m[5] * m[7];
            inverse[1] = m[2] * m[7] - m[1] * m[8];
            inverse[2] = m[1] * m[5] - m[2] * m[4];
            inverse[3] = m[5] * m[6] - m[3] * m[8];
            inverse[4] = m[0] * m[8] - m[2] * m[6];
            inverse[5] = m[2] * m[3] - m[0] * m[5];
            inverse[6] = m[3] * m[7] - m[4] * m[6];
            inverse[7] = m[1] * m[6] - m[0] * m[7];
            inverse[8] = m[0] * m[4] - m[1] * m[3];
            
            T det = m[0] * inverse[0] + m[1] * inverse[3] + m[2] * inverse[6];
            
            if (std::fabs(det) > epsilon)
            {
                T invDet = ((T)1) / det;
                inverse *= invDet;
                
                *this = inverse;
                
                return true;
            }
            
            return false;
        }

        template <typename T> inline void Matrix3<T>::SetTranspose()
        {
            Matrix3<T> copy = *this;

            m[1] = copy[3];
            m[2] = copy[6];
            m[3] = copy[1];
            m[5] = copy[7];
            m[6] = copy[2];
            m[7] = copy[5];
        }

        template <typename T> inline Matrix3<T> Matrix3<T>::Transpose()
        {
            Matrix3<T> result;

            result[0] = m[0];
            result[1] = m[3];
            result[2] = m[6];
            result[3] = m[1];
            result[4] = m[4];
            result[5] = m[7];
            result[6] = m[2];
            result[7] = m[5];
            result[8] = m[8];

            return result;
        }

        template <typename T> inline Matrix3<T> Matrix3<T>::Invserse()
        {
            Matrix3<T> result;
            //result = *this;
            result.Set(m);
            if (result.SetInverse())
                return result;
            else
                return Matrix3<T>();
        }

        template <typename T> inline void Matrix3<T>::SetSkewSymmetric(const Vector3<T>& vec)
        {
            m[0] = m[4] = m[8] = 0;
            m[3] = -vec.z;
            m[6] = vec.y;
            m[1] = vec.z;
            m[7] = -vec.x;
            m[2] = -vec.y;
            m[5] = vec.x;
        }

        template <typename T> inline Vector3<T> Matrix3<T>::TransformTranspose(const Vector3<T>& vec)
        {
            return Vector3<T> (vec.x * m[0] + vec.y * m[1] + vec.z * m[2],
                               vec.x * m[3] + vec.y * m[4] + vec.z * m[5],
                               vec.x * m[6] + vec.y * m[7] + vec.z * m[8]);
        }

        template <typename T> inline void Matrix3<T>::SetInertiaTensorCoeffs(T ix, T iy, T iz, T ixy, T ixz, T iyz)
        {
            m[0] = ix;
            m[3] = m[1] = -ixy;
            m[6] = m[2] = -ixz;
            m[4] = iy;
            m[7] = m[5] = -iyz;
            m[8] = iz;
        }

        template <typename T> inline void Matrix3<T>::SetBlockInertiaTensor(const Vector3<T>& halfSize, T mass)
        {
            Math::Vector3<T> squares = halfSize.ComponentProduct(halfSize);
            SetInertiaTensorCoeffs((T)0.3 * mass * (squares.y + squares.z),
                                   (T)0.3 * mass * (squares.x + squares.z),
                                   (T)0.3 * mass * (squares.x + squares.y));
        }

        template <typename T> inline Matrix3<T> Matrix3<T>::LinearInterpolate(const Matrix3<T>& matrix, T prop)
        {
            Matrix3<T> result;
            for (unsigned i = 0; i < 9; ++i)
                result[i] = m[i] * (1.0f - prop) + matrix[i] * prop;
            return result;
        }

        template <typename T> Vector3<T> operator *(const Vector3<T> &vec, const Matrix3<T> &mat)
        {
            return Vector3<T > (vec.x * mat[0] + vec.y * mat[1] + vec.z * mat[2],
                                vec.x * mat[3] + vec.y * mat[4] + vec.z * mat[5],
                                vec.x * mat[6] + vec.y * mat[7] + vec.z * mat[8]);
        }

        template <typename T> Vector3<T> operator *(const Matrix3<T> &mat, const Vector3<T> &vec)
        {
            return Vector3<T > (mat[0] * vec.x + mat[3] * vec.y + mat[6] * vec.z,
                                mat[1] * vec.x + mat[4] * vec.y + mat[7] * vec.z,
                                mat[2] * vec.x + mat[5] * vec.y + mat[8] * vec.z);
        }

        template <typename T> void operator *=(Vector3<T>& vec, const Matrix3<T>& mat)
        {
            vec.Set(mat[0] * vec.x + mat[3] * vec.y + mat[6] * vec.z,
                    mat[1] * vec.x + mat[4] * vec.y + mat[7] * vec.z,
                    mat[2] * vec.x + mat[5] * vec.y + mat[8] * vec.z);
        }

        template <typename T> std::ostream & operator <<(std::ostream &out, const Matrix3<T> &right)
        {
            return out << "[[" << right[0] << ", " << right[3] << ", " << right[6] << "],\n"
                    " [" << right[1] << ", " << right[4] << ", " << right[7] << "],\n"
                    " [" << right[2] << ", " << right[5] << ", " << right[8] << "]]";
        }

        typedef Matrix3<int> mat3i;
        typedef Matrix3<float> mat3f;
        typedef Matrix3<double> mat3d;
    }
}

#endif	/* MATRIX3_HPP */

