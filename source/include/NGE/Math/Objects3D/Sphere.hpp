/* 
 * File:   Sphere.h
 * Author: tku
 *
 * Created on 28 January 2014, 15:01
 */

#ifndef SPHERE_HPP
#define SPHERE_HPP

namespace NGE {
	namespace Math {
		namespace Objects3D {

			/*
			 * Class that represents sphere in three dimensional space. The sphere is defined
			 * as |X - C| = R, where C is the centre, and R is a radius.
			 */
			template <typename T> class Sphere {
			  protected:

				Vector3<T> center;
				T radius;

			  public:

				/**
				 * Default empty constructor creates with radius = 1 positioned at origin(0, 0, 0)
				 */
				Sphere() : radius(1) { }

				/**
				 * Create sphere from provided parameters.
				 * @param center Origin point of the sphere.
				 * @param radius Radius of the sphere.
				 */
				Sphere(const Vector3<T>& center, T radius) : center(center), radius(radius) { }

				/**
				 * Default empthy descructor.
				 */
				virtual ~Sphere() { }

				/**
				 * Operator kopiujący przypisujący aktualnej instancji
				 * wartości z kopii.
				 * @param copy Kula której środek i promień mają zostać przypisane.
				 */
				Sphere& operator=(const Sphere<T>& copy);

				/* Metody dostępu do pól */

				/**
				 * Metoda zwracająca środek kuli.
				 * @return Środek kuli.
				 */
				Math::Vector3<T> GetCenter();

				/**
				 * Ustawienie środka kuli.
				 * @param Wektor który ma być nowym środkiem kuli.
				 */
				void SetCenter(const Vector3<T>& center);

				/**
				 * Get radius of the sphere.
				 * @return Sphere radius.
				 */
				T GetRadius();

				/**
				 * Set sphere radius.
				 * @return New sphere radius.
				 */
				void SetRadius(const T radius);
			};

			template <typename T> Math::Vector3<T> Sphere<T>::GetCenter() {
				return center;
			}

			template <typename T> Sphere<T>& Sphere<T>::operator=(const Sphere<T>& copy) {
				this->center = copy.center;
				this->radius = copy.radius;

				return *this;
			}

			template <typename T> void Sphere<T>::SetCenter(const Vector3<T>& center) {
				this->center = center;
			}

			template <typename T> T Sphere<T>::GetRadius() {
				return radius;
			}

			template <typename T> void Sphere<T>::SetRadius(const T radius) {
				this->radius = radius;
			}

			typedef Sphere<float> spheref;
			typedef Sphere<double> sphered;
		}
	}
}

#endif /* SPHERE_HPP */

