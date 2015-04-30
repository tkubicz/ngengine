/* 
 * File:   Quaternion.hpp
 * Author: tku
 *
 * Created on 15 kwiecień 2013, 21:50
 */

#ifndef QUATERNION_HPP
#define	QUATERNION_HPP

#include "NGE/Math/MathUtils.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Matrix3.hpp"
#include "NGE/Math/Matrix4.hpp"

namespace NGE
{
    namespace Math
    {

        /**
         * Kwaterniony posiadają kilka matematycznych własności które sprawiają,
         * że są użyteczne przy reprezentacji orientacji obiektu w przestrzeni
         * trójwymiarowej, jednak potrzebują czterech wartości do przechowywania
         * informacji o trzech wymiarach. Te cztery współczynniki mogą być
         * reprezentowane jako liczba zespolona z trzema jednostkami urojonymi.
         * Kwaterniony nadają się do rotacji 3D. Rotacja jest właściwa wtedy i
         * tylko wtedy, gdy kwaternion jest znormalizowany, tzn. jego długość
         * wynosi 1.
         * 
         * @note Prędkość kątowa i przyśpieszenie mogą być prawidłowo reprezentowane
         * przez wektory. Kwaterniony są potrzebne w zasadzie tylko do rotacji.
         */
        template <typename T> class Quaternion
        {
          public:
            T r, i, j, k;

            /**
             * Domyślny konstruktor tworzy kwaternion reprezentujący rotację o
             * 0 stopni na każdej z osi.
             */
            Quaternion() : r(1), i(0), j(0), k(0) { }

            /**
             * Konstruktor tworzący kwaternion z podanych wartości.
             * 
             * @param r Cześć rzeczywista kwaterniona.
             * @param i Pierwsza jednostka urojona kwaterniona.
             * @param j Druga jednostka urojona kwaterniona.
             * @param k Trzecia jednostka urojona kwaterniona.
             */
            Quaternion(const T r, const T i, const T j, const T k) : r(r), i(i), j(j), k(k) { }

            Quaternion(const Math::Matrix3<T>& rotMatrix)
            {
                T t = 1 + rotMatrix[0] + rotMatrix[4] + rotMatrix[8];

                if (t > 0.001)
                {
                    T s = std::sqrt(t) * 2.0;
                    i = (rotMatrix[5] - rotMatrix[7]) / s;
                    j = (rotMatrix[6] - rotMatrix[2]) / s;
                    k = (rotMatrix[1] - rotMatrix[3]) / s;
                    r = 0.25 * s;
                }
                else if (rotMatrix[0] > rotMatrix[4] && rotMatrix[0] > rotMatrix[8])
                {
                    // Column 0.
                    T s = std::sqrt(1.0 + rotMatrix[0] - rotMatrix[4] - rotMatrix[8]) * 2.0;
                    i = 0.25 * s;
                    j = (rotMatrix[1] + rotMatrix[3]) / s;
                    k = (rotMatrix[6] + rotMatrix[2]) / s;
                    r = (rotMatrix[5] - rotMatrix[7]) / s;
                }
                else if (rotMatrix[4] > rotMatrix[8])
                {
                    // Column 1.
                    T s = std::sqrt(1.0 + rotMatrix[4] - rotMatrix[0] - rotMatrix[8]) * 2.0;
                    i = (rotMatrix[1] + rotMatrix[3]) / s;
                    j = 0.25 * s;
                    k = (rotMatrix[5] + rotMatrix[7]) / s;
                    r = (rotMatrix[6] - rotMatrix[2]) / s;
                }
                else
                {
                    T s = std::sqrt(1.0 + rotMatrix[8] - rotMatrix[0] - rotMatrix[4]) * 2.0;
                    i = (rotMatrix[6] + rotMatrix[2]) / s;
                    j = (rotMatrix[5] + rotMatrix[7]) / s;
                    k = 0.25 * s;
                    r = (rotMatrix[1] - rotMatrix[3]) / s;
                }
            }

            /**
             * Mnożenie kwaterniona przez kwaternion.
             * 
             * @param multi Kwaternion przez który będzie wykonane mnożenie.
             */
            void operator*=(const Quaternion& multi)
            {
                Quaternion<T> q = *this;
                r = q.r * multi.r - q.i * multi.i - q.j * multi.j - q.k * multi.k;
                i = q.r * multi.i + q.i * multi.r + q.j * multi.k - q.k * multi.j;
                j = q.r * multi.j + q.j * multi.r + q.k * multi.i - q.i * multi.k;
                k = q.r * multi.k + q.k * multi.r + q.i * multi.j - q.j * multi.i;
            }

            /**
             * Normalizacja kwaterniona. Kwaternion musi być znormalizowany by
             * rotacja była prawidłowa.
             */
            void Normalize()
            {
                T d = r * r + i * i + j * j + k * k;

                // Sprawdzenie, czy kwaternion nie jest zerowej długości.
                // Jeżeli tak, to użycie kwaterniona będącego rotacją o 0 stopni.
                if (d == 0)
                {
                    r = 1;
                    return;
                }

                d = ((T) 1.0) / (T) sqrt(d);
                r *= d;
                i *= d;
                j *= d;
                k *= d;
            }

            /**
             * Dodanie podanego wektora przeskalowanego przez podaną wartość.
             * Używane do aktualizacji kwaterniona orientacji przez rotację i
             * czas.
             * 
             * @param vec Wektor do dodania.
             * @param scale Wartość przez którą wektor zostanie przeskalowany.
             */
            void AddScaledVector(const Vector3<T>& vec, T scale)
            {
                Quaternion<T> q(0, vec.x * scale, vec.y * scale, vec.z * scale);
                q *= *this;
                r += q.r * ((T) 0.5);
                i += q.i * ((T) 0.5);
                j += q.j * ((T) 0.5);
                k += q.k * ((T) 0.5);
            }

            /**
             * Rotacja kwaterniona preze podany wektor.
             * 
             * @param vec Wektor prez który kwaternion zostanie obrócony.
             */
            void RotateByVector(const Vector3<T>& vec)
            {
                Quaternion q(0, vec.x, vec.y, vec.z);
                (*this) *= q;
            }

            /**
             * TODO: This method should be renamed to GetMatrix3().
             * @return 
             */
            NGE::Math::Matrix3<T> GetMatrix() const
            {
                Math::Matrix3<T> result;
                result[0] = (T) 1.0 - (T) 2.0 * (j * j + k * k);
                result[1] = (T) 2.0 * (i * j + k * r);
                result[2] = (T) 2.0 * (i * k - j * r);
                result[3] = (T) 2.0 * (i * j - k * r);
                result[4] = (T) 1.0 - (T) 2.0 * (i * i + k * k);
                result[5] = (T) 2.0 * (j * k + i * r);
                result[6] = (T) 2.0 * (i * k + j * r);
                result[7] = (T) 2.0 * (j * k - i * r);
                result[8] = (T) 1.0 - (T) 2.0 * (i * i + j * j);
                return result;
            }

            /**
             * Create Matrix 4x4 from quaternion and return it.
             * @return Matrix4<T> created from quaternion.
             */
            NGE::Math::Matrix4<T> GetMatrix4() const
            {
                Math::Matrix4<T> result;

                result[0] = 1.0f - 2.0f * j * j - 2.0f * k * k;
                result[1] = 2.0f * i * j + 2.0f * r * k;
                result[2] = 2.0f * i * k - 2.0f * r * j;
                result[3] = 0.0f;

                result[4] = 2.0f * i * j - 2.0f * r * k;
                result[5] = 1.0f - 2.0f * i * i - 2.0f * k * k;
                result[6] = 2.0f * j * k + 2.0f * r * i;
                result[7] = 0.0f;

                result[8] = 2.0f * i * k + 2.0f * r * j;
                result[9] = 2.0f * j * k - 2.0f * r * i;
                result[10] = 1.0f - 2.0f * i * i - 2.0f * j * j;
                result[11] = 0.0f;

                result[12] = 0.0f;
                result[13] = 0.0f;
                result[14] = 0.0f;
                result[15] = 1.0f;

                return result;
            }

            /**
             * Rotate quaternion around specyfic axes by the given angle.
             * @param angle Angle in degrees.
             * @param x X axis.
             * @param y Y axis.
             * @param z Z axis.
             */
            void Rotate(const T angle, const T x, const T y, const T z)
            {
                T halfAngleRadian = Math::MathUtils::DegToRad(angle) * (T) 0.5;
                T s = sinf(halfAngleRadian);
                Math::Vector3<T> normalized(x, y, z);
                normalized.Normalize();

                i = normalized.x * s;
                j = normalized.y * s;
                k = normalized.z * s;
                r = cosf(halfAngleRadian);
            }

            /**
             * Rotate quaternion around X axis by the given angle.
             * @param angle Angle in degrees.
             */
            void RotateX(const T angle)
            {
                T halfAngleRadian = Math::MathUtils::DegToRad(angle) * (T) 0.5;

                i = sinf(halfAngleRadian);
                j = 0.0f;
                k = 0.0f;
                r = cosf(halfAngleRadian);
            }

            /**
             * Rotate quaternion around Y axis by the given angle.
             * @param angle Angle in degrees.
             */
            void RotateY(const T angle)
            {
                T halfAngleRadian = Math::MathUtils::DegToRad(angle) * (T) 0.5;

                i = 0.0f;
                j = sinf(halfAngleRadian);
                k = 0.0f;
                r = cosf(halfAngleRadian);
            }

            /**
             * Rotate quaternion around Z axis by the given angle.
             * @param angle Angle in degrees.
             */
            void RotateZ(const T angle)
            {
                T halfAngleRadian = Math::MathUtils::DegToRad(angle) * (T) 0.5;

                i = 0.0f;
                j = 0.0f;
                k = sinf(halfAngleRadian);
                r = cosf(halfAngleRadian);
            }
        };

        template <typename T> std::ostream & operator<<(std::ostream &out, const Quaternion<T> &right)
        {
            return out << "r=" << right.r << ", i=" << right.i << ", j=" << right.j << ", k=" << right.k;
        }

        typedef Quaternion<int> quaternioni;
        typedef Quaternion<float> quaternionf;
        typedef Quaternion<double> quaterniond;
    }
}

#endif	/* QUATERNION_HPP */

