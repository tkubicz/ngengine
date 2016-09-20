/* 
 * File:   Plane.hpp
 * Author: tku
 *
 * Created on 11 February 2014, 13:43
 */

#ifndef PLANE_HPP
#define PLANE_HPP

#include "NGE/Math/Vector3.hpp"

namespace NGE {
	namespace Math {
		namespace Objects3D {

			/**
			 * Class that represents plane in three dimensional space. The plane
			 * is a set of points that meets equations Ax + By + Cz + D = 0, where
			 * A, B and C cannot be 0 at the same time. Vector [A,B,C] is then
			 * a normal for the plane.
			 */
			template <typename T> class Plane {
			  protected:
				/**
				 * Coefficients of the plane.
				 */
				T a, b, c, d;

			  public:

				/**
				 * Default, empty constructor. It creates plane with normal vector
				 * (0,1,0) positioned at origin(0,0,0).
				 */
				Plane() : a(0), b(1), c(0), d(0) { }

				/**
				 * Create plane from provided coefficients. It implicitly executes
				 * SetCoefficients(a, b, c, d) method.
				 * @param a Coefficient A.
				 * @param b Coefficient B.
				 * @param c Coefficient C.
				 * @param d Coefficient D.
				 */
				Plane(T a, T b, T c, T d);

				/**
				 * Create plane from three provided points. It implicitly
				 * executs SetPoints(p1, p2, p3) method.
				 * @param p1 First point.
				 * @param p2 Second point.
				 * @param p3 Third point.
				 */
				Plane(const Math::Vector3<T>& p1, const Math::Vector3<T>& p2, const Math::Vector3<T>& p3);

				/**
				 * Create plane from normal vector and point. It implicitly
				 * executes SetNormalAndPoint(normal, point) method.
				 * @param normal Normal vector of the plane.
				 * @param point Point in space.
				 */
				Plane(const Math::Vector3<T>& normal, const Math::Vector3<T>& point);

				/**
				 * Copy operator.
				 * @param p Plane that should be copied.
				 * @return Copied plane.
				 */
				inline Plane& operator=(const Plane& p);

				/**
				 * Array access operator.
				 * @param i Index - it should be between [0-3].
				 * @return Coefficient at specified index.
				 */
				inline const T operator[](unsigned i) const;

				/**
				 * Array assigment operator.
				 * @param i Index - it should be between [0-3].
				 * @return New coefficient.
				 */
				inline T &operator[](unsigned i);

				/**
				 * Get distance from point to plane.
				 * @param p Point we test against.
				 * @return Distance between point and plane.
				 */
				T GetDistance(const Math::Vector3<T>& p) const;

				/**
				 * Get plane normal vector. It is constructed from A, B and C
				 * coefficients.
				 * @return New Vector3 [A,B,C].
				 */
				NGE::Math::Vector3<T> GetNormal() const;

				/**
				 * Create plane equation from points provided.
				 * @param p1 First point.
				 * @param p2 Second point.
				 * @param p3 Third point.
				 */
				void SetPoints(const Math::Vector3<T>& p1, const Math::Vector3<T>& p2, const Math::Vector3<T>& p3);

				/**
				 * Create plane from normal vector and point.
				 * @param normal Normal vector of the plane.
				 * @param point Point in space.
				 */
				void SetNormalAndPoint(const Math::Vector3<T>& normal, const Math::Vector3<T>& point);

				/**
				 * Create plane from coefficients provided.
				 * @param a Coefficient A.
				 * @param b Coefficient B.
				 * @param c Coefficient C.
				 * @param d Coefficient D.
				 */
				void SetCoefficients(T a, T b, T c, T d);
			};

			template <typename T> Plane<T>::Plane(T a, T b, T c, T d) {
				SetCoefficients(a, b, c, d);
			}

			template <typename T> Plane<T>::Plane(const Math::Vector3<T>& p1, const Math::Vector3<T>& p2, const Math::Vector3<T>& p3) {
				SetPoints(p1, p2, p3);
			}

			template <typename T> Plane<T>::Plane(const Math::Vector3<T>& normal, const Math::Vector3<T>& point) {
				SetNormalAndPoint(normal, point);
			}

			template<typename T> Plane<T>& Plane<T>::operator=(const Plane<T>& p) {
				a = p[0];
				b = p[1];
				c = p[2];
				d = p[3];
				return* this;
			}

			template<typename T> const T Plane<T>::operator[](unsigned i) const {
				return ((T*) & a)[i];
			}

			template<typename T> T& Plane<T>::operator[](unsigned i) {
				return ((T*) & a)[i];
			}

			template <typename T> void Plane<T>::SetPoints(const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3) {
				// Calculate normal vector from three points.
				Math::Vector3<T> aux1, aux2;
				aux1 = p1 - p2;
				aux2 = p3 - p2;

				Math::Vector3<T> normal = aux2.CrossProduct(aux1);
				normal.Normalize();

				a = normal.x;
				b = normal.y;
				c = normal.z;
				d = -(normal.DotProduct(p2));
			}

			template<typename T> void Plane<T>::SetNormalAndPoint(const Math::Vector3<T>& normal, const Math::Vector3<T>& point) {
				Math::Vector3<T> copy = normal;
				copy.Normalize();
				a = copy.x;
				b = copy.y;
				c = copy.z;
				d = -(copy.DotProduct(point));
			}

			template<typename T> void Plane<T>::SetCoefficients(T a, T b, T c, T d) {
				this->a = a;
				this->b = b;
				this->c = c;
				this->d = d;
			}

			template<typename T> T Plane<T>::GetDistance(const Math::Vector3<T>& p) const {
				Math::Vector3<T> normal(a, b, c);
				return (d + normal.DotProduct(p));
			}

			template <typename T> NGE::Math::Vector3<T> Plane<T>::GetNormal() const {
				return Math::Vector3<T>(a, b, c);
			}

			template <typename T> std::ostream &operator<<(std::ostream &out, const Plane<T> &p) {
				return out << "[" << p[0] << "," << p[1] << "," << p[2] << "," << p[3] << "]";
			}

			typedef Plane<float> planef;
			typedef Plane<double> planed;
		}
	}
}

#endif /* PLANE_HPP */

