/* 
 * File:   Plane.hpp
 * Author: tku
 *
 * Created on 25 listopad 2013, 14:28
 */

#ifndef PLANE_HPP
#define	PLANE_HPP

#include "NGE/Geometry/Basic/Basic.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Basic
        {
            class Plane : public Basic
            {
              public:
                bool Initialize();
                
                virtual void Update(float dt);
                virtual void Render();
                
              protected:
                virtual void DeleteBuffers();
                virtual void DeleteArrays();

                virtual bool InitializeGeometry();
                virtual bool InitializeVBA();
                virtual bool InitializeVBO();
            };
        }
    }
}

#endif	/* PLANE_HPP */

