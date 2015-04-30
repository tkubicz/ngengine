/* 
 * File:   WaterTexture.hpp
 * Author: tku
 *
 * Created on 22 październik 2013, 15:52
 */

#ifndef WATERTEXTURE_HPP
#define	WATERTEXTURE_HPP

#include "NGE/Geometry/Mesh.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Nature
        {

            class WaterTexture : public Mesh
            {
              public:

                struct WaveParameters
                {
                    float speed;
                    float amplitude;
                    float wavelength;
                    float steepness;
                };

                struct WaveDirections
                {
                    float x;
                    float z;
                };

                WaterTexture();

                unsigned int Initialize(float waterPlaneLength, Media::Shaders::GLSLProgram* shader);
                void Reshape(int width, int height);
                void Update(float dt);
                void Render();
                void Terminate();

              protected:
                virtual void DeleteBuffers();
                virtual void DeleteArrays();

              private:

                static const int TEXTURE_SIZE = 1024;
                static const int NUMBER_WAVES = 4;

                float passedTime;
                int parentWidth, parentHeight;

                Media::Shaders::GLSLProgram* shader;
                GLuint mirrorTexture; // Tekstura
                GLuint depthMirrorTexture; // Renderbuffer Object
                GLuint waterTexture; // Framebuffer Object
            };
        }
    }
}

#endif	/* WATERTEXTURE_HPP */

