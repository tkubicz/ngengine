/* 
 * File:   MinSphere3.hpp
 * Author: tku
 *
 * Created on 28 styczeń 2014, 14:52
 */

#ifndef MINSPHERE3_HPP
#define	MINSPHERE3_HPP

#include "NGE/Math/Objects3D/Sphere.hpp"
#include "NGE/Math/Matrix3.hpp"
#include "NGE/Tools/Random.hpp"
#include "NGE/Tools/Logger.hpp"
#include <float.h>

namespace NGE {
	namespace Math {

		/**
		 * Klasa służąca do obliczania najmniejszej sfery ograniczającej z podanych punktów.
		 * Algorytm losowo permutuje punkty wejściowe, więc złożoność czasowa algorytmu
		 * powinna wynosić O(N).
		 *
		 * Implementacja tej klasy została napisana przez Davida Eberly i użyta w silniku
		 *  Wild Magic.
		 */
		template <typename T> class MinSphere3 {
		  public:
			MinSphere3(int numPoints, const Vector3<T>* points, Objects3D::Sphere<T>& minimal, T epsilon = (T) 1e-03);

		  private:

			/**
			 *  Indeksy punktów które wspierają minimalna sferę ograniczającą.
			 */
			class Support {
			  public:
				bool Contains(int index, Vector3<T>** points, T epsilon);

				int quantity;
				int index[4];
			};

			/**
			 * Sprawdzenie czy punkt P znajduje się w kuli S.
			 * @param point
			 * @param sphere
			 * @param distDiff
			 * @return 
			 */
			bool Contains(const Vector3<T>& point, Objects3D::Sphere<T>& sphere, T& distDiff);

			Objects3D::Sphere<T> ExactSphere1(const Vector3<T>& p);
			Objects3D::Sphere<T> ExactSphere2(const Vector3<T>& p0, const Vector3<T>& p1);

			/**
			 * 
			 * @note Sposób działania metody: obliczenie okręgu (w 3D) zawierającego
			 * punkty p0, p1 i p2. Środek okręgu we współrzędnych barycentrycznych
			 * to K = u0 * p0 + u1 * p1 + u2 * p2, gdzie u0 + u1 + u2 = 1. Środek
			 * jest równoodległy od trzech punktów, więc |K - p0| = |K - p1| = |K - p2| = R,
			 * gdzie R jest promieniem kuli.
			 * 
			 * Z tych współrzędnych,
			 *  K - p0 = u0 * A + u1 * B - A
			 *  K - p1 = u0 * A + u1 * B - B
			 *  K - p2 = u0 * A + u1 * B
			 * gdzie A = p0 - p2 i B = p1 - p2, co prowadzi do
			 *  r^2 = |u0 * A + u1 * B|^2 - 2 * Dot(A, u0 * A + u1 * B) + |A|^2
			 *  r^2 = |u0 * A + u1 * B|^2 - 2 * Dot(B, u0 * A + u1 * B) + |B|^2
			 *  r^2 = |u0 * A + u1 * B|^2
			 * Odejmując ostatnie równanie od dwóch pierwszych i zapisując równania
			 * jako liniowe
			 * 
			 * +-                 -++-  -+       +-        -+
			 * | Dot(A,A) Dot(A,B) || u0 | = 0.5 | Dot(A,A) |
			 * | Dot(B,A) Dot(B,B) || u1 |       | Dot(B,B) |
			 * +-                 -++-  -+       +-        -+
			 * 
			 * Poniższy kod rozwiązuje równania dla u0 i u1, następnie wylicza r^2
			 * w trzecim równaniu w celu obliczenia r.
			 * 
			 * @param p0 Pierwszy punkt.
			 * @param p1 Drugi punkt.
			 * @param p2 Trzeci punkt.
			 * @return 
			 */
			Objects3D::Sphere<T> ExactSphere3(const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2);

			/**
			 * 
			 * @note Sposób działania metody: obliczenie okręgu z punktów p0, p1, p2, p3.
			 * Środek okręgu we współrzędnych barycentrycznych to K = u0 * p0 + u1 * p1 + u2 * p2 + u3 * p3
			 * gdzie u0 + u1 + u2 + u3 = 1. Środek jest równoodległy od czterech 
			 * punktów, więc |K - p0| = |K - p1| = |K - p2| = |K - p3| = R,
			 * gdzie R to promień kuli.
			 * 
			 * Z tych współrzędnych
			 *  K - p0 = u0 * A + u1 * B + u2 * C - A
			 *  K - p1 = u0 * A + u1 * B + u2 * C - B
			 *  K - p2 = u0 * A + u1 * B + u2 * C - C
			 *  K - p3 = u0 * A + u1 * B + u2 * C
			 * gdzie A = p0 - p3, B = p1 - p3 i C = p2 - 3, co prowadzi do
			 *  r^2 = 
			 * 
			 * @param p0
			 * @param p1
			 * @param p2
			 * @param p3
			 * @return 
			 */
			Objects3D::Sphere<T> ExactSphere4(const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3);

			Objects3D::Sphere<T> UpdateSupport1(int i, Vector3<T>** permuted, Support& support);
			Objects3D::Sphere<T> UpdateSupport2(int i, Vector3<T>** permuted, Support& support);
			Objects3D::Sphere<T> UpdateSupport3(int i, Vector3<T>** permuted, Support& support);
			Objects3D::Sphere<T> UpdateSupport4(int i, Vector3<T>** permuted, Support& support);

			T epsilon;
		};

		template <typename T> MinSphere3<T>::MinSphere3(int numPoints, const Vector3<T>* points, Objects3D::Sphere<T>& minimal, T epsilon) : epsilon(epsilon) {
			Support support;
			T distDiff;

			if (numPoints >= 1) {
				// Utworzenie permutacji identycznościowych (0,1,...,numPoints - 1).
				Vector3<T>** permuted = new Vector3<T>*[numPoints];

				for (int i = 0; i < numPoints; ++i)
					permuted[i] = (Vector3<T>*) & points[i];

				// Wygenerowanie losowej permutacji
				Tools::Random random;
				for (int i = numPoints - 1; i > 0; --i) {
					int j = random.RandomBits() % (i + 1);
					if (j != i) {
						Vector3<T>* save = permuted[i];
						permuted[i] = permuted[j];
						permuted[j] = save;
					}
				}

				minimal = ExactSphere1(*permuted[0]);
				support.quantity = 1;
				support.index[0] = 0;

				for (int i = 1 % numPoints, n = 0; i != n; i = (i + 1) % numPoints) {
					if (!support.Contains(i, permuted, epsilon)) {
						if (!Contains(*permuted[i], minimal, distDiff)) {
							//UpdateFunction update = update[support.quantity];
							//Objects3D::Sphere<T> sphere = (this->*update)(i, permuted, support);
							Objects3D::Sphere<T> sphere;
							if (support.quantity == 1)
								sphere = UpdateSupport1(i, permuted, support);
							else if (support.quantity == 2)
								sphere = UpdateSupport2(i, permuted, support);
							else if (support.quantity == 3)
								sphere = UpdateSupport3(i, permuted, support);
							else if (support.quantity == 4)
								sphere = UpdateSupport4(i, permuted, support);

							if (sphere.GetRadius() > minimal.GetRadius()) {
								minimal = sphere;
								n = i;
							}
						}
					}
				}

				delete permuted;
			} else {
				Tools::Logger::WriteErrorLog("MinSphere3 --> Input must contain points.");
				return;
			}

			minimal.SetRadius(std::sqrt(minimal.GetRadius()));
		}

		template <typename T> bool MinSphere3<T>::Contains(const Vector3<T>& point, Objects3D::Sphere<T>& sphere, T& distDiff) {
			Vector3<T> diff = point - sphere.GetCenter();
			T test = diff.LengthSquared();

			// W tym algorytmie, klasa Sphere posiada promień podniesiony do kwadratu,
			// dlatego ta linia kodu nie jest błędna.
			distDiff = test - sphere.GetRadius();

			return distDiff <= (T) 0;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::ExactSphere1(const Vector3<T>& p) {
			Objects3D::Sphere<T> minimal(p, (T) 0);
			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::ExactSphere2(const Vector3<T>& p0, const Vector3<T>& p1) {
			Objects3D::Sphere<T> minimal;
			minimal.SetCenter(((T) 0.5) * (p0 + p1));
			Vector3<T> diff = p1 - p0;
			minimal.SetRadius(((T) 0.25) * diff.LengthSquared());
			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::ExactSphere3(const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2) {
			Vector3<T> A = p0 - p2;
			Vector3<T> B = p1 - p2;
			T AdA = A.DotProduct(A);
			T AdB = A.DotProduct(B);
			T BdB = B.DotProduct(B);
			T det = AdA * BdB - AdB * AdB;

			Objects3D::Sphere<T> minimal;
			if (std::fabs(det) > (T) 0) {
				T m00, m01, m10, m11, d0, d1;
				if (AdA >= BdB) {
					m00 = (T) 1;
					m01 = AdB / AdA;
					m10 = m01;
					m11 = BdB / AdA;
					d0 = (T) 0.5;
					d1 = ((T) 0.5) * m11;
				} else {
					m00 = AdA / BdB;
					m01 = AdB / BdB;
					m10 = m01;
					m11 = (T) 1;
					d0 = ((T) 0.5) * m00;
					d1 = (T) 0.5;
				}

				T invDet = ((T) 1) / (m00 * m11 - m01 * m10);
				T u0 = invDet * (m11 * d0 - m01 * d1);
				T u1 = invDet * (m00 * d1 - m10 * d0);
				T u2 = (T) 1 - u0 - u1;
				minimal.SetCenter(u0 * p0 + u1 * p1 + u2 * p2);
				Vector3<T> tmp = u0 * A + u1 * B;
				minimal.SetRadius(tmp.LengthSquared());
			} else {
				minimal.SetCenter(Vector3<T>(0, 0, 0));
				minimal.SetRadius(FLT_MAX);
			}

			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::ExactSphere4(const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3) {
			Vector3<T> A = p0 - p3;
			Vector3<T> B = p1 - p3;
			Vector3<T> C = p2 - p3;
			Matrix3<T> M(A.x, A.y, A.z,
					B.x, B.y, B.z,
					C.x, C.y, C.z);
			Vector3<T> D(A.DotProduct(A), B.DotProduct(B), C.DotProduct(C));
			D *= (T) 0.5;

			Matrix3<T> invM = M.Invserse();

			Objects3D::Sphere<T> minimal;
			if (invM != Matrix3<T>()) {
				Vector3<T> V = D * invM;
				Vector3<T> U = invM * V;
				T U3 = (T) 1 - U[0] - U[1] - U[2];
				minimal.SetCenter(U[0] * p0 + U[1] * p1 + U[2] * p2 + U3 * p3);
				Vector3<T> tmp = U[0] * A + U[1] * B + U[2] * C;
				minimal.SetRadius(tmp.LengthSquared());
			} else {
				minimal.SetCenter(Math::vec3f(0, 0, 0));
				minimal.SetRadius(FLT_MAX);
			}

			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::UpdateSupport1(int i, Vector3<T>** permuted, Support& support) {
			const Vector3<T>& p0 = *permuted[support.index[0]];
			const Vector3<T>& p1 = *permuted[i];

			Objects3D::Sphere<T> minimal = ExactSphere2(p0, p1);
			support.quantity = 2;
			support.index[1] = i;

			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::UpdateSupport2(int i, Vector3<T>** permuted, Support& support) {
			const Vector3<T>* point[2] = {
				permuted[support.index[0]],
				permuted[support.index[1]]
			};
			const Vector3<T>& p2 = *permuted[i];

			const int numType2 = 2;
			const int type2[numType2][2] = {
				{0, 1},
				{1, 0}
			};

			const int numType3 = 1;

			Objects3D::Sphere<T> sphere[numType2 + numType3];
			int indexSphere = 0;
			T minRSqr = FLT_MAX;
			int indexMinRSqr = -1;
			T distDiff, minDistDiff = FLT_MAX;
			int indexMinDistDiff = -1;

			for (int j = 0; j < numType2; ++j, ++indexSphere) {
				sphere[indexSphere] = ExactSphere2(*point[type2[j][0]], p2);
				if (sphere[indexSphere].GetRadius() < minRSqr) {
					if (Contains(*point[type2[j][1]], sphere[indexSphere], distDiff)) {
						minRSqr = sphere[indexSphere].GetRadius();
						indexMinRSqr = indexSphere;
					} else if (distDiff < minDistDiff) {
						minDistDiff = distDiff;
						indexMinDistDiff = indexSphere;
					}
				}
			}

			sphere[indexSphere] = ExactSphere3(*point[0], *point[1], p2);
			if (sphere[indexSphere].GetRadius() < minRSqr) {
				minRSqr = sphere[indexSphere].GetRadius();
				indexMinRSqr = indexSphere;
			}

			if (indexMinRSqr == -1)
				indexMinRSqr = indexMinDistDiff;

			Objects3D::Sphere<T> minimal = sphere[indexMinRSqr];
			switch (indexMinRSqr) {
				case 0:
					support.index[1] = i;
					break;
				case 1:
					support.index[0] = i;
					break;
				case 2:
					support.quantity = 3;
					support.index[2] = i;
					break;
			}

			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::UpdateSupport3(int i, Vector3<T>** permuted, Support& support) {
			const Vector3<T>* point[3] = {
				permuted[support.index[0]],
				permuted[support.index[1]],
				permuted[support.index[2]]
			};

			const Vector3<T>& p3 = *permuted[i];

			const int numType2 = 3;
			const int type2[numType2][3] = {
				{0, 1, 2},
				{1, 0, 2},
				{2, 0, 1}
			};

			const int numType3 = 3;
			const int type3[numType3][3] = {
				{0, 1, 2},
				{0, 2, 1},
				{1, 2, 0}
			};

			const int numType4 = 1;

			Objects3D::Sphere<T> sphere[numType2 + numType3 + numType4];
			int indexSphere = 0;
			T minRSqr = FLT_MAX;
			int indexMinRSqr = -1;
			T distDiff, minDistDiff = FLT_MAX;
			int indexMinDistDiff = -1;

			for (int j = 0; j < numType2; ++j, ++indexSphere) {
				sphere[indexSphere] = ExactSphere2(*point[type2[j][0]], p3);
				if (sphere[indexSphere].GetRadius() < minRSqr) {
					if (Contains(*point[type2[j][1]], sphere[indexSphere], distDiff) &&
							Contains(*point[type2[j][2]], sphere[indexSphere], distDiff)) {
						minRSqr = sphere[indexSphere].GetRadius();
						indexMinRSqr = indexSphere;
					} else if (distDiff < minDistDiff) {
						minDistDiff = distDiff;
						indexMinDistDiff = indexSphere;
					}
				}
			}

			for (int j = 0; j < numType3; ++j, ++indexSphere) {
				sphere[indexSphere] = ExactSphere3(*point[type3[j][0]], *point[type3[j][1]], p3);
				if (sphere[indexSphere].GetRadius() < minRSqr) {
					if (Contains(*point[type3[j][2]], sphere[indexSphere], distDiff)) {
						minRSqr = sphere[indexSphere].GetRadius();
						indexMinRSqr = indexSphere;
					} else if (distDiff < minDistDiff) {
						minDistDiff = distDiff;
						indexMinDistDiff = indexSphere;
					}
				}
			}

			sphere[indexSphere] = ExactSphere4(*point[0], *point[1], *point[2], p3);
			if (sphere[indexSphere].GetRadius() < minRSqr) {
				minRSqr = sphere[indexSphere].GetRadius();
				indexMinRSqr = indexSphere;
			}

			if (indexMinRSqr == -1)
				indexMinRSqr = indexMinDistDiff;

			Objects3D::Sphere<T> minimal = sphere[indexMinRSqr];
			switch (indexMinRSqr) {
				case 0:
					support.quantity = 2;
					support.index[1] = i;
					break;
				case 1:
					support.quantity = 2;
					support.index[0] = i;
					break;
				case 2:
					support.quantity = 2;
					support.index[0] = support.index[2];
					support.index[1] = i;
					break;
				case 3:
					support.index[2] = i;
					break;
				case 4:
					support.index[1] = i;
					break;
				case 5:
					support.index[0] = i;
					break;
				case 6:
					support.quantity = 4;
					support.index[3] = i;
					break;
			}

			return minimal;
		}

		template <typename T> Objects3D::Sphere<T> MinSphere3<T>::UpdateSupport4(int i, Vector3<T>** permuted, Support& support) {
			const Vector3<T>* point[4] = {
				permuted[support.index[0]],
				permuted[support.index[1]],
				permuted[support.index[2]],
				permuted[support.index[3]]
			};

			const Vector3<T>& p4 = *permuted[i];

			const int numType2 = 4;
			const int type2[numType2][4] = {
				{0, 1, 2, 3},
				{1, 0, 2, 3},
				{2, 0, 1, 3},
				{3, 0, 1, 2}
			};

			const int numType3 = 6;
			const int type3[numType3][4] = {
				{0, 1, 2, 3},
				{0, 2, 1, 3},
				{0, 3, 1, 2},
				{1, 2, 0, 3},
				{1, 3, 0, 2},
				{2, 3, 0, 1}
			};

			const int numType4 = 4;
			const int type4[numType4][4] = {
				{0, 1, 2, 3},
				{0, 1, 3, 2},
				{0, 2, 3, 1},
				{1, 2, 3, 0}
			};

			Objects3D::Sphere<T> sphere[numType2 + numType3 + numType4];
			int indexSphere = 0;
			T minRSqr = FLT_MAX;
			int indexMinRSqr = -1;
			T distDiff, minDistDiff = FLT_MAX;
			int indexMinDistDiff = -1;

			for (int j = 0; j < numType2; ++j, ++indexSphere) {
				sphere[indexSphere] = ExactSphere2(*point[type2[j][0]], p4);
				if (sphere[indexSphere].GetRadius() < minRSqr) {
					if (Contains(*point[type2[j][1]], sphere[indexSphere], distDiff) &&
							Contains(*point[type2[j][2]], sphere[indexSphere], distDiff) &&
							Contains(*point[type2[j][3]], sphere[indexSphere], distDiff)) {
						minRSqr = sphere[indexSphere].GetRadius();
						indexMinRSqr = indexSphere;
					} else if (distDiff < minDistDiff) {
						minDistDiff = distDiff;
						indexMinDistDiff = indexSphere;
					}
				}
			}

			for (int j = 0; j < numType3; ++j, ++indexSphere) {
				sphere[indexSphere] = ExactSphere3(*point[type3[j][0]], *point[type3[j][1]], p4);
				if (sphere[indexSphere].GetRadius() < minRSqr) {
					if (Contains(*point[type3[j][2]], sphere[indexSphere], distDiff) &&
							Contains(*point[type3[j][3]], sphere[indexSphere], distDiff)) {
						minRSqr = sphere[indexSphere].GetRadius();
						indexMinRSqr = indexSphere;
					} else if (distDiff < minDistDiff) {
						minDistDiff = distDiff;
						indexMinDistDiff = indexSphere;
					}
				}
			}

			for (int j = 0; j < numType4; ++j, ++indexSphere) {
				sphere[indexSphere] = ExactSphere4(*point[type4[j][0]], *point[type4[j][1]], *point[type4[j][2]], p4);
				if (sphere[indexSphere].GetRadius() < minRSqr) {
					if (Contains(*point[type4[j][3]], sphere[indexSphere], distDiff)) {
						minRSqr = sphere[indexSphere].GetRadius();
						indexMinRSqr = indexSphere;
					} else if (distDiff < minDistDiff) {
						minDistDiff = distDiff;
						indexMinDistDiff = indexSphere;
					}
				}
			}

			if (indexMinRSqr == -1)
				indexMinRSqr = indexMinDistDiff;

			Objects3D::Sphere<T> minimal = sphere[indexMinRSqr];
			switch (indexMinRSqr) {
				case 0:
					support.quantity = 2;
					support.index[1] = i;
					break;
				case 1:
					support.quantity = 2;
					support.index[0] = i;
					break;
				case 2:
					support.quantity = 2;
					support.index[0] = support.index[2];
					support.index[1] = i;
					break;
				case 3:
					support.quantity = 2;
					support.index[0] = support.index[3];
					support.index[1] = i;
					break;
				case 4:
					support.quantity = 3;
					support.index[2] = i;
					break;
				case 5:
					support.quantity = 3;
					support.index[1] = i;
					break;
				case 6:
					support.quantity = 3;
					support.index[1] = support.index[3];
					support.index[2] = i;
					break;
				case 7:
					support.quantity = 3;
					support.index[0] = i;
					break;
				case 8:
					support.quantity = 3;
					support.index[0] = support.index[3];
					support.index[2] = i;
					break;
				case 9:
					support.quantity = 3;
					support.index[0] = support.index[3];
					support.index[1] = i;
					break;
				case 10:
					support.index[3] = i;
					break;
				case 11:
					support.index[2] = i;
					break;
				case 12:
					support.index[1] = i;
					break;
				case 13:
					support.index[0] = i;
					break;
			}

			return minimal;
		}

		template <typename T> bool MinSphere3<T>::Support::Contains(int index, Vector3<T>** points, T epsilon) {
			for (int i = 0; i < quantity; ++i) {
				Vector3<T> diff = *points[index] - *points[this->index[i]];
				if (diff.LengthSquared() < epsilon)
					return true;
			}

			return false;
		}
	}
}

#endif	/* MINSPHERE3_HPP */

