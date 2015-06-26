/* 
 * File:   Sphere.h
 * Author: tku
 *
 * Created on 28 styczeń 2014, 15:01
 */

#ifndef SPHERE_HPP
#define	SPHERE_HPP

namespace NGE {
	namespace Math {
		namespace Objects3D {

			/*
			 * Szablon reprezentujący kulę w przestrzeni trójwymiarowej.
			 * Kula matematycznie to |X - C| = R, gdzie C jest środkiem,
			 * a R promieniem.
			 */
			template <typename T> class Sphere {
			  public:
				/* Konstruktory i destruktor */

				/**
				 * Domyślny konstruktor tworzy kulę o środku w punkcie (0,0,0)
				 * i promieniu 1.
				 */
				Sphere();

				/**
				 * Konstruktor tworzący kulę z podanych parametrów.
				 * @param center Wektor będący środkiem kuli.
				 * @param radius Promień kuli.
				 */
				Sphere(const Vector3<T>& center, T radius);

				/**
				 * Domyślny destruktor.
				 */
				virtual ~Sphere();

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
				 * Metoda zwracająca promień kuli.
				 * @return Promień kuli.
				 */
				T GetRadius();

				/**
				 * Ustawienie promienia kuli.
				 * @return Promień kuli.
				 */
				void SetRadius(const T radius);

			  protected:

				Vector3<T> center;
				T radius;
			};

#include "NGE/Math/Objects3D/Sphere.inc"

			typedef Sphere<float> spheref;
			typedef Sphere<double> sphered;
		}
	}
}

#endif	/* SPHERE_HPP */

