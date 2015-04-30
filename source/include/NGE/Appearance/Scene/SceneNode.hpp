/* 
 * File:   SceneNode.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 13:24
 */

#ifndef SCENENODE_HPP
#define	SCENENODE_HPP

#include <string>
#include "NGE/Math/Matrix4.hpp"

namespace NGE
{
    namespace Appearance
    {
        namespace Scene
        {

            /**
             * A node in the imported hierarchy.
             * 
             * Each node has a name, a parent node (except for the root node),
             * a transformation relative to its parent and possibly several child nodes.
             * Simple file formats don't support hirearchical structures - for these
             * formats the imported scene does consist of only a single root node without
             * children.
             */
            class SceneNode
            {
              public:
                /**
                 * Default constructor.
                 */
                SceneNode();

                /**
                 * Constructor from a specific name.
                 * @param name Name of the node.
                 */
                SceneNode(const std::string& name);

                virtual ~SceneNode();

                /**
                 * Search for a node with a specific name, beginning at this node.
                 * Usually this method is called on the root node of the scene.
                 * @param name Name of the node to search.
                 * @return NULL or a valid node if the search was successful.
                 */
                inline SceneNode* FindNode(const std::string& name);
                inline SceneNode* FindNode(const char* name);

                /**
                 * Get name of the current node.
                 * @return Name of the current node.
                 */
                std::string GetName();

                /**
                 * Set name of the current node.
                 * @param name Name to be assigned.
                 */
                void SetName(const std::string& name);

                /**
                 * Get transformation matrix.
                 * @return Transformation matrix.
                 */
                Math::mat4f GetTransformation();

                /**
                 * Set the transformation matrix.
                 * @param transformation Transformation to be assigned.
                 */
                void SetTransformation(const Math::mat4f& transformation);

                /**
                 * Get parent node.
                 * @return Pointer to the parent node, or NULL if this is 
                 * a root node.
                 */
                SceneNode* GetParent();

                /**
                 * Set the parent node.
                 * @param parent Pointer to parent node.
                 */
                void SetParent(SceneNode* parent);

                /**
                 * Get children count.
                 * @return Number of children of this node.
                 */
                unsigned int GetNumChildren();

                /**
                 * Set the number of children of this node.
                 * @param numChildren New number of children.
                 */
                void SetNumChildren(unsigned int numChildren);

                /**
                 * Get chilren of this node.
                 * @return The array of pointers to children.
                 */
                SceneNode** GetChildren();

                /**
                 * Get number of meshes assigned to this node.
                 * @return Number of meshes.
                 */
                unsigned int GetNumMeshses();

                /**
                 * Set number of meshes assigned to this node.
                 * @param numMeshes New number of meshes.
                 */
                void SetNumMeshes(unsigned int numMeshes);

                /**
                 * Get meshes.
                 * @return 
                 */
                unsigned int* GetMeshes();

              protected:

                /**
                 * The name of the node.
                 */
                std::string name;

                /**
                 * The transformation relative to the node's parent.
                 */
                Math::mat4f transformation;

                /**
                 * Parent node. NULL if this node is the root node.
                 */
                SceneNode* parent;

                /**
                 * The number of child nodes of this node.
                 */
                unsigned int numChildren;

                /**
                 * The child nodes of this node. NULL if numChildren == 0.
                 */
                SceneNode** children;

                /**
                 * The number of meshes of this node.
                 */
                unsigned int numMeshes;

                /**
                 * The meshes of this node. Each entry is an index into the mesh.
                 */
                unsigned int* meshes;
            };
        }
    }
}

#endif	/* SCENENODE_HPP */

