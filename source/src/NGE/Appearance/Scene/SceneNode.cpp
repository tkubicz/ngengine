#include "NGE/Appearance/Scene/SceneNode.hpp"
using namespace NGE::Appearance::Scene;

SceneNode::SceneNode()
{
    parent = NULL;
    children = NULL;
    meshes = NULL;
    numChildren = numMeshes = 0;
}

SceneNode::SceneNode(const std::string& name)
{
    parent = NULL;
    children = NULL;
    meshes = NULL;
    numChildren = numMeshes = 0;
    this->name = name;
}

SceneNode::~SceneNode()
{
    // Delete all children recursively.
    if (children && numChildren)
    {
        for (unsigned int i = 0; i < numChildren; ++i)
            delete children[i];
    }

    delete[] children;
    delete[] meshes;
}

inline SceneNode* SceneNode::FindNode(const std::string& name)
{
    return FindNode(name.c_str());
}

inline SceneNode* SceneNode::FindNode(const char* name)
{
    if (!strcmp(this->name.c_str(), name))
        return this;
    for (unsigned int i = 0; i < numChildren; ++i)
    {
        SceneNode* p = children[i]->FindNode(name);
        if (p)
            return p;
    }
    // There is no sub node with this name.
    return NULL;
}

std::string SceneNode::GetName()
{
    return name;
}

void SceneNode::SetName(const std::string& name)
{
    this->name = name;
}

NGE::Math::mat4f SceneNode::GetTransformation()
{
    return transformation;
}

void SceneNode::SetTransformation(const Math::mat4f& transformation)
{
    this->transformation = transformation;
}

SceneNode* SceneNode::GetParent()
{
    return parent;
}

void SceneNode::SetParent(SceneNode* parent)
{
    this->parent = parent;
}

unsigned int SceneNode::GetNumChildren()
{
    return numChildren;
}

SceneNode** SceneNode::GetChildren()
{
    return children;
}

unsigned int SceneNode::GetNumMeshses()
{
    return numMeshes;
}

void SceneNode::SetNumMeshes(unsigned int numMeshes)
{
    this->numMeshes = numMeshes;
}

void SceneNode::SetNumChildren(unsigned int numChildren)
{
    this->numChildren = numChildren;
}

unsigned int* SceneNode::GetMeshes()
{
    return meshes;
}










