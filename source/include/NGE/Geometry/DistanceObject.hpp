/* 
 * File:   DistanceObject.hpp
 * Author: tku
 *
 * Created on 6 styczeń 2013, 00:02
 */

#ifndef DISTANCEOBJECT_HPP
#define	DISTANCEOBJECT_HPP

namespace NGE {
	namespace Geometry {

		template <class T> class DistanceObject {
		  private:
			float distance;
			T object;

		  public:

			DistanceObject() : distance(0.0f) { }

			DistanceObject(T object) {
				distance = 0.0f;
				this->object = object;
			}

			DistanceObject& operator=(const DistanceObject& copy) {
				if (this != &copy) {
					distance = copy.distance;
					object = copy.object;
				}
				return* this;
			}

			bool operator==(const DistanceObject& copy) {
				return (distance == copy.distance);
			}

			bool operator>=(const DistanceObject& copy) {
				return (distance >= copy.distance);
			}

			bool operator<=(const DistanceObject& copy) {
				return (distance <= copy.distance);
			}

			bool operator>(const DistanceObject& copy) {
				return (distance > copy.distance);
			}

			bool operator<(const DistanceObject& copy) {
				return (distance < copy.distance);
			}

			void Set(float distance, T object) {
				this->distance = distance;
				this->object = object;
			}

			void SetDistance(float distance) {
				this->distance = distance;
			}

			float GetDistance() {
				return distance;
			}

			void SetObject(T object) {
				this->object = object;
			}

			T& GetObject() {
				return object;
			}
		};
	}
}

#endif	/* DISTANCEOBJECT_HPP */

