/* 
 * File:   MeshAnim.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 14:46
 */

#ifndef MESHANIM_HPP
#define	MESHANIM_HPP

#include "NGE/Appearance/Scene/MeshKey.hpp"


namespace NGE
{
    namespace Appearance
    {
        namespace Scene
        {

            /**
             * Describes vertex-based animations for a single mesh or a group of
             * meshes. Meshes carry the animation data for each frame in their
             * array. The purpose of MeshAnim is to define keyframes linking
             * each mesh attachment to a particular point in time.
             */
            class MeshAnim
            {
              public:

                MeshAnim() { }

                virtual ~MeshAnim()
                {
                    delete[] keys;
                }

              protected:

                /**
                 * Name of the mesh to be animated. An empty string is not allowed,
                 * animated meshes need to be named (not necessarily uniquely, the name
                 * can basically serve as wildcard to select a group of meshes with 
                 * similar animation setup).
                 */
                std::string name;

                /**
                 * Size of keys array. Must be 1, at least.
                 */
                unsigned int numKeys;

                /**
                 * Key frames of the animation. May not be NULL.
                 */
                MeshKey* keys;
            };
        }
    }
}

#endif	/* MESHANIM_HPP */

