/* 
 * File:   Scene.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 13:54
 */

#ifndef SCENE_HPP
#define	SCENE_HPP

#include "NGE/Appearance/Scene/SceneNode.hpp"
#include "NGE/Appearance/Scene/Animation.hpp"
#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Geometry/Mesh.hpp"

namespace NGE
{
    namespace Appearance
    {
        namespace Scene
        {

            /**
             * The root structure of the imported data.
             * Everything that was imported from the given file can be accessed
             * from here.
             */
            class Scene
            {
              public:

                /**
                 * Default constructor.
                 */
                Scene() { }

                /**
                 * Destructor.
                 */
                virtual ~Scene() { }

                inline bool HasMeshes() const
                {
                    return meshes != NULL && numMeshes > 0;
                }

                inline bool HasMaterials() const
                {
                    return materials != NULL && numMaterials > 0;
                }

                inline bool HasTextures() const
                {
                    return textures != NULL && numTextures > 0;
                }

                inline bool HasAnimations() const
                {
                    return animations != NULL && numAnimations > 0;
                }

                unsigned int GetNumAnimations() const
                {
                    return numAnimations;
                }

                unsigned int GetNumMeshes() const
                {
                    return numMeshes;
                }

                void SetNumMeshes(unsigned int numMeshes)
                {
                    this->numMeshes = numMeshes;
                }

                Geometry::Mesh** GetMeshes()
                {
                    return meshes;
                }

                void SetNumAnimations(unsigned int numAnimations)
                {
                    this->numAnimations = numAnimations;
                }

                Animation** GetAnimations()
                {
                    return animations;
                }

                void SetAnimations(Animation** animations)
                {
                    this->animations = animations;
                }

                unsigned int GetNumMaterials() const
                {
                    return numMaterials;
                }

                void SetNumMaterials(unsigned int numMaterials)
                {
                    this->numMaterials = numMaterials;
                }

                Material** GetMaterials()
                {
                    return materials;
                }

                void SetMaterials(Material** materials)
                {
                    this->materials = materials;
                }

                SceneNode* GetRootNode()
                {
                    return rootNode;
                }

                void SetRootNode(SceneNode* rootNode)
                {
                    this->rootNode = rootNode;
                }

              protected:
                /**
                 * The root node of the hierarchy.
                 */
                SceneNode* rootNode;

                /**
                 * Number of meshes in the scene.
                 */
                unsigned int numMeshes;

                /**
                 * The array of meshes.
                 */
                Geometry::Mesh** meshes;

                /**
                 * The number of materials in the scene.
                 */
                unsigned int numMaterials;

                /**
                 * The array of materials.
                 */
                Material** materials;

                /**
                 * The number of animations in the scene.
                 */
                unsigned int numAnimations;

                /**
                 * The array of animations.
                 * All animations imported from the given file are listed here.
                 * The array is numAnimations in size.
                 */
                Animation** animations;

                /**
                 * The nuber of textures embedded into the file.
                 */
                unsigned int numTextures;

                /**
                 * The array of embedded textures.
                 * Not many file formats embed their textures into the file.
                 * An example is Quake's MDL format.
                 */
                Media::Images::Texture** textures;

                // TOOD: Lights and Cameras?
            };
        }
    }
}

#endif	/* SCENE_HPP */

