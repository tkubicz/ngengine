/* 
 * File:   Animation.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 14:58
 */

#ifndef ANIMATION_HPP
#define	ANIMATION_HPP

#include "NGE/Appearance/Scene/NodeAnim.hpp"
#include "NGE/Appearance/Scene/MeshAnim.hpp"


namespace NGE
{
    namespace Appearance
    {
        namespace Scene
        {

            /**
             * An animation consists of keyframe data for a number of nodes. For
             * each node affected by the animation a separate series of data is given.
             */
            class Animation
            {
              public:

                Animation() : duration(-1.0) { }

                virtual ~Animation()
                {
                    if (numChannels && channels)
                    {
                        for (unsigned int i = 0; i < numChannels; ++i)
                            delete channels[i];

                        delete[] channels;
                    }

                    if (numMeshChannels && meshChannels)
                    {
                        for (unsigned int i = 0; i < numMeshChannels; ++i)
                            delete meshChannels[i];

                        delete[] meshChannels;
                    }
                }

                std::string GetName() const
                {
                    return name;
                }

                void SetName(const std::string& name)
                {
                    this->name = name;
                }

                double GetDuration() const
                {
                    return duration;
                }

                void SetDuration(double duration)
                {
                    this->duration = duration;
                }

                double GetTicksPerSecond() const
                {
                    return ticksPerSecond;
                }

                void SetTicksPerSecond(double ticksPerSecond)
                {
                    this->ticksPerSecond = ticksPerSecond;
                }

                unsigned int GetNumChannels() const
                {
                    return numChannels;
                }

                void SetNumChannels(unsigned int numChannels)
                {
                    this->numChannels = numChannels;
                }
                
                NodeAnim** GetChannels()
                {
                    return channels;
                }
                
                void SetChannels(NodeAnim** channels)
                {
                    this->channels = channels;
                }

              protected:
                /**
                 * The name of the animation. If the modeling package this data was
                 * exported from does support only a single animation channel, this
                 * name is usually empty.
                 */
                std::string name;

                /**
                 * Duration of the animation in ticks.
                 */
                double duration;

                /**
                 * Ticks per second. 0 if not specified in the imported file.
                 */
                double ticksPerSecond;

                /**
                 * The number of bone animation channels. Each channel affects a single node.
                 */
                unsigned int numChannels;

                /**
                 * The node animation channels. Each channel affects a single node.
                 * The array is numChannels in size.
                 */
                NodeAnim** channels;

                /**
                 * The number of mesh animation channels. Each channel affects a single
                 * mesh and defines vertex-based animation.
                 */
                unsigned int numMeshChannels;

                /**
                 * The mesh animation channels. Each channel affects a single mesh.
                 * The array is numMeshChannels in size.
                 */
                MeshAnim** meshChannels;
            };
        }
    }
}

#endif	/* ANIMATION_HPP */

