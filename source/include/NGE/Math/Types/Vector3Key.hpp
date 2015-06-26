/* 
 * File:   Vector3Key.h
 * Author: tku
 *
 * Created on 28 luty 2014, 15:45
 */

#ifndef VECTOR3KEY_HPP
#define	VECTOR3KEY_HPP

namespace NGE {
	namespace Math {
		namespace Types {

			/**
			 * A time value pair specifying a certain 3D vector for the given time.
			 */
			template <typename T> class Vector3Key {
			  public:

				/**
				 * Default constructor.
				 */
				Vector3Key() { }

				/**
				 * Construction from a given time and key value.
				 * @param time Time assigned to time field.
				 * @param value Value assigned to value field.
				 */
				Vector3Key(T time, const Vector3<T>& value) : time(time), value(value) { }

				/**
				 * Comparsion operator. For use with std::find().
				 * @param other Vector3Key to compare.
				 * @return True if value of the current vector equals other,
				 * otherwise false.
				 */
				bool operator==(const Vector3Key& other) const;

				/**
				 * Not equal operator. For use with std::find().
				 * @param other Vector3Key to compare.
				 * @return True if value of the current vector not equals other,
				 * otherwise false.
				 */
				bool operator!=(const Vector3Key& other) const;

				/**
				 * Relational operators. For use with std::sort().
				 * @param other Vector3Key to compare.
				 * @return True if the right time value is bigger,
				 * otherwise false.
				 */
				bool operator<(const Vector3Key& other) const;

				/**
				 * Relational operators. For use with std::sort().
				 * @param other Vector3Key to compare.
				 * @return True if the left time value is bigger,
				 * otherwise false.
				 */
				bool operator>(const Vector3Key& other) const;

				/**
				 * Get the current time.
				 * @return Current time.
				 */
				T GetTime() const;

				/**
				 * Get the current value.
				 * @return Current value.
				 */
				Vector3<T> GetValue() const;

				/**
				 * Set the time.
				 * @param time Time to be assigned.
				 */
				void SetTime(T time);

				/**
				 * Set the value.
				 * @param value Value to be assigned.
				 */
				void SetValue(const Vector3<T>& value);

			  protected:

				/**
				 * The time of this key.
				 */
				T time;

				/**
				 * The value of this key.
				 */
				Vector3<T> value;
			};

#include "NGE/Math/Types/Vector3Key.inc"
		}
	}
}

#endif	/* VECTOR3KEY_HPP */

