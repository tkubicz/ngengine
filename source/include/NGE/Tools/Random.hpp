/* 
 * File:   Random.hpp
 * Author: tku
 *
 * Created on 20 kwiecień 2012, 00:49
 */

#ifndef RANDOM_HPP
#define	RANDOM_HPP

#include "NGE/Math/Vector3.hpp"

namespace NGE
{
    namespace Tools
    {

        class Random
        {
        public:

            /**
             * Nowy strumień liczb losowych z ziarnem opartym na czasie.
             */
            Random();

            /**
             * Nowy strumień liczb losowych z podanego ziarna.
             */
            Random(unsigned seed);

            /**
             * Nadanie ziarna dla strumienia.
             */
            void Seed(unsigned seed);

            /**
             * Zwraca następny ciąg losowych bitów ze strumienia. Jest to najszybsza metoda.
             */
            unsigned RandomBits();

            /**
             * Zwraca losową liczbę zmiennoprzecinkową z przedziału 0 i 1.
             */
            float RandomFloat();

            /**
             * Zwraca losową liczbę zmiennoprzecinkową z przedzialu 0 i scale.
             */
            float RandomFloat(float scale);

            /**
             * Zwraca losową liczbę zmiennoprzecinkową z przedziału min i max.
             */
            float RandomFloat(float min, float max);

            unsigned RandomInt(unsigned max);

            float RandomBinomial(float scale);

            Math::vec3f RandomVector(float scale);

            Math::vec3f RandomVector(const Math::vec3f& scale);

            Math::vec3f RandomVector(const Math::vec3f& min, const Math::vec3f& max);

            Math::vec3f RandomXZVector(float scale);

            // Quaterion RandomQuaterion();

        private:

            int p1, p2;
            unsigned buffer[17];

        };
    }
}

#endif	/* RANDOM_HPP */

