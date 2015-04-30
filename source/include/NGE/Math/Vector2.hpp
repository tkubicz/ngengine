/* 
 * File:   Vector2.hpp
 * Author: tku
 *
 * Created on 29 styczeń 2012, 04:23
 */

#ifndef VECTOR2_HPP
#define	VECTOR2_HPP

#include <iostream>

namespace NGE
{
    namespace Math
    {

        template <typename T> class Vector2
        {
        public:
            T x, y;

            Vector2(const T x = 0, const T y = 0) : x(x), y(y) { }

            Vector2(const Vector2<T>&v) : x(v.x), y(v.y) { }

            inline const T operator [] (unsigned i) const;
            inline T &operator [] (unsigned i);

            inline Vector2 &operator /=(const T scale);

            bool operator ==(const Vector2 &v);
            bool operator !=(const Vector2 &v);

            inline void Set(T x, T y);
            inline void Set(const Vector2<T> &vec);
        };

        template <typename T> inline const T Vector2<T>::operator[](unsigned i) const
        {
            return ((T*) & x)[i];
        }

        template <typename T> inline T &Vector2<T>::operator[](unsigned i)
        {
            return ((T*) & x)[i];
        }

        template <typename T> Vector2<T>& Vector2<T>::operator /=(const T scale)
        {
            if (scale)
            {
                x /= scale;
                y /= scale;
            }
            return *this;
        }

        template <typename T> bool Vector2<T>::operator ==(const Vector2<T> &v)
        {
            return (x == v.x && y == v.y);
        }

        template <typename T> bool Vector2<T>::operator !=(const Vector2<T> &v)
        {
            return !(x == v.x && y == v.y);
        }

        template <typename T> void Vector2<T>::Set(T x, T y)
        {
            this->x = x;
            this->y = y;
        }

        template <typename T> void Vector2<T>::Set(const Vector2<T>& vec)
        {
            Set(vec.x, vec.y);
        }
        
        template <typename T> std::ostream &operator<<(std::ostream &out, const Vector2<T> &v)
        {
            return out << "(" << v.x << "," << v.y << ")";
        }

        typedef Vector2<int> vec2i;
        typedef Vector2<float> vec2f;
        typedef Vector2<double> vec2d;
    }
}

#endif	/* VECTOR2_HPP */

