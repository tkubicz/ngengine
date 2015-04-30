/* 
 * File:   SkyColor.hpp
 * Author: tku
 *
 * Created on 23 wrzesień 2013, 04:51
 */

#ifndef SKYCOLOR_HPP
#define	SKYCOLOR_HPP

#include "NGE/Math/Vector4.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Nature
        {

            class SkyColor
            {
              public:
                SkyColor();

                void SetInfo(float sunTheta, float turbidity);
                void GetVertexColor(float theta, float phi, float sunPhi, Math::vec4f* color);

              private:
                void ToRGBA(float x, float y, float Y, Math::vec4f* rgb);
                
                /**
                 * Wszystkie kąty w radianach, kąty theta wyliczone z normalnej (?)
                 * @param theta1
                 * @param phi1
                 * @param theta2
                 * @param phi2
                 * @return 
                 */
                float Angle(float theta1, float phi1, float theta2, float phi2);

                inline float PerezFunction(const float* perez, const float& cosTheta, const float& cosGamma, const float& gamma);

                float perez_x[5];
                float perez_y[5];
                float perez_Y[5];

                float T, T2, suntheta, suntheta2, suntheta3;
                float cosSTheta;
                float zenith_x, zenith_y, zenith_Y;
            };
        }
    }
}

#endif	/* SKYCOLOR_HPP */

