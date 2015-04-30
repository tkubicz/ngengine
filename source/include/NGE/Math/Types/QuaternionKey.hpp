/* 
 * File:   QuaternionKey.hpp
 * Author: tku
 *
 * Created on 28 luty 2014, 16:09
 */

#ifndef QUATERNIONKEY_HPP
#define	QUATERNIONKEY_HPP

#include "NGE/Math/Quaternion.hpp"

namespace NGE
{
    namespace Math
    {
        namespace Types
        {

            /**
             * A time value pair specifying a rotation for the given time.
             */
            template <typename T> class QuaternionKey
            {
              public:

                /**
                 * Default constructor.
                 */
                QuaternionKey() { }

                /**
                 * Construction from a given time and key value.
                 * @param time Time assigned to time field.
                 * @param value Value assigned to value field.
                 */
                QuaternionKey(T time, const Quaternion<T>& value) : time(time), value(value) { }

                /**
                 * Comparsion operator. For use with std::find().
                 * @param other QuaternionKey to compare.
                 * @return True if value of the current vector equals other,
                 * otherwise false.
                 */
                bool operator ==(const QuaternionKey& other) const;

                /**
                 * Not equal operator. For use with std::find().
                 * @param other QuaternionKey to compare.
                 * @return True if value of the current vector not equals other,
                 * otherwise false.
                 */
                bool operator !=(const QuaternionKey& other) const;

                /**
                 * Relational operators. For use with std::sort().
                 * @param other QuaternionKey to compare.
                 * @return True if the right time value is bigger,
                 * otherwise false.
                 */
                bool operator<(const QuaternionKey& other) const;

                /**
                 * Relational operators. For use with std::sort().
                 * @param other QuaternionKey to compare.
                 * @return True if the left time value is bigger,
                 * otherwise false.
                 */
                bool operator>(const QuaternionKey& other) const;

                /**
                 * Get the current time.
                 * @return Current time.
                 */
                T GetTime() const;

                /**
                 * Get the current value.
                 * @return Current value.
                 */
                Quaternion<T> GetValue() const;

                /**
                 * Set the time.
                 * @param time Time to be assigned.
                 */
                void SetTime(T time);

                /**
                 * Set the value.
                 * @param value Value to be assigned.
                 */
                void SetValue(const Quaternion<T>& value);

              protected:
                /**
                 * The time of this key.
                 */
                T time;

                /**
                 * The value of this key.
                 */
                Quaternion<T> value;
            };

#include "NGE/Math/Types/QuaternionKey.inc"
        }
    }
}

#endif	/* QUATERNIONKEY_HPP */

