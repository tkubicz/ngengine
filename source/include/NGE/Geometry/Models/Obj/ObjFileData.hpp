/* 
 * File:   ObjFileData.hpp
 * Author: tku
 *
 * Created on 7 październik 2014, 19:17
 */

#ifndef OBJFILEDATA_HPP
#define	OBJFILEDATA_HPP

#include <vector>
#include <map>
#include <string>

#include "NGE/Math/Matrix4.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector2.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Models
        {
            namespace Obj
            {
#ifndef MAX_NUMBER_OF_TEXTURECOORDS
#define MAX_NUMBER_OF_TEXTURECOORDS  0x8
#endif

                class Object;
                class Face;
                class Material;

                class Face
                {
                  public:
                    std::vector<unsigned int>* vertices;
                    std::vector<unsigned int>* normals;
                    std::vector<unsigned int>* textureCoords;
                    Material* material;

                  public:

                    Face(std::vector<unsigned int>* vertices, std::vector<unsigned int>* normals,
                         std::vector<unsigned int>* texCoords) : vertices(vertices), normals(normals), textureCoords(texCoords), material(0) { }

                    virtual ~Face()
                    {
                        delete vertices;
                        vertices = nullptr;

                        delete normals;
                        normals = nullptr;

                        delete textureCoords;
                        textureCoords = nullptr;
                    }
                };

                /**
                 * Stores all objects of any objfile objects definition.
                 */
                class Object
                {
                  public:

                    enum ObjectType
                    {
                        ObjType,
                        GroupType
                    };

                    /**
                     * Object name.
                     */
                    std::string objName;

                    /**
                     * Transformation matrix, stored in OpenGL format.
                     */
                    NGE::Math::mat4f transformation;

                    /**
                     * All sub-objects referenced by this object.
                     */
                    std::vector<Object*> subObjects;

                    /**
                     * Assigned meshes.
                     */
                    std::vector<unsigned int> meshes;

                  public:

                    /**
                     * Default constructor.
                     */
                    Object() : objName("") { }

                    virtual ~Object()
                    {
                        for (std::vector<Object*>::iterator it = subObjects.begin(); it != subObjects.end(); ++it)
                        {
                            delete *it;
                        }

                        subObjects.clear();
                    }
                };

                /**
                 * Data structure to store all material specific data.
                 */
                class Material
                {
                  public:
                    /**
                     * Name of material description.
                     */
                    std::string materialName;

                    std::string texture;
                    std::string textureSpecular;
                    std::string textureAmbient;
                    std::string textureBump;
                    std::string textureSpecularity;
                    std::string textureOpacity;

                    /**
                     * Ambient color.
                     */
                    Math::vec3f ambient;

                    /**
                     * Diffuse color.
                     */
                    Math::vec3f diffuse;

                    /**
                     * Specular color.
                     */
                    Math::vec3f specular;

                    /**
                     * Alpha value.
                     */
                    float alpha;

                    /**
                     * Shineness factor.
                     */
                    float shineness;

                    /**
                     * Illumination model.
                     */
                    int illuminationModel;

                    /**
                     * Index of refraction.
                     */
                    float ior;

                  public:

                    Material() : diffuse(0.6f, 0.6f, 0.6f),
                    alpha(1.f),
                    shineness(0.0f),
                    illuminationModel(1),
                    ior(1.f) { }

                    virtual ~Material() { }
                };

                /**
                 * Data structure to store a mesh.
                 */
                class Mesh
                {
                  public:
                    static const unsigned int noMaterial = ~0u;

                    /**
                     * Array with pointer to all stored faces.
                     */
                    std::vector<Face*> faces;

                    /**
                     * Assigned material.
                     */
                    Material* material;

                    /**
                     * Number of stored indices.
                     */
                    unsigned int numIndices;

                    /**
                     * Number of UV.
                     */
                    unsigned int uvCoordinates[MAX_NUMBER_OF_TEXTURECOORDS];

                    /**
                     * Material index.
                     */
                    unsigned int materialIndex;

                    /**
                     * True, if normals are stored.
                     */
                    bool hasNormals;

                  public:

                    /**
                     * Default constructor.
                     */
                    Mesh() : material(nullptr), numIndices(0), materialIndex(noMaterial), hasNormals(false)
                    {
                        memset(uvCoordinates, 0, sizeof (unsigned int) * MAX_NUMBER_OF_TEXTURECOORDS);
                    }

                    /**
                     * Destructor.
                     */
                    virtual ~Mesh()
                    {
                        for (std::vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
                        {
                            delete *it;
                        }
                    }
                };

                /**
                 * Data structure to store all obj-specific model datas.
                 */
                class Model
                {
                  public:
                    typedef std::map<std::string, std::vector<unsigned int>*> GroupMap;
                    typedef std::map<std::string, std::vector<unsigned int>*>::iterator GroupMapIt;
                    typedef std::map<std::string, std::vector<unsigned int>*>::const_iterator ConstGroupMapIt;

                    /**
                     * Model name.
                     */
                    std::string modelName;

                    /**
                     * List of assigned objects.
                     */
                    std::vector<Object*> objects;

                    /**
                     * Pointer to current object.
                     */
                    Object* current;

                    /**
                     * Pointer to current material.
                     */
                    Material* currentMaterial;

                    /**
                     * Pointer to default material.
                     */
                    Material* defaultMaterial;

                    /**
                     * Vector with all generated materials.
                     */
                    std::vector<std::string> materialLib;

                    /**
                     * Vector with all generated group.
                     */
                    std::vector<std::string> groupLib;

                    /**
                     * Vector with all generated vertices.
                     */
                    std::vector<Math::vec3f> vertices;

                    /**
                     * Vector with all generated normals.
                     */
                    std::vector<Math::vec3f> normals;

                    /**
                     * Group map.
                     */
                    GroupMap groups;

                    /**
                     * Group to face id assignment.
                     */
                    std::vector<unsigned int>* groupFaceIDs;

                    /**
                     * Active group.
                     */
                    std::string activeGroup;

                    /**
                     * Vector with generated texture coordinates.
                     */
                    std::vector<Math::vec2f> textureCoords;

                    /**
                     * Current mesh instance.
                     */
                    Mesh* currentMesh;

                    /**
                     * Vector with stored meshes.
                     */
                    std::vector<Mesh*> meshes;

                    /**
                     * Material map.
                     */
                    std::map<std::string, Material*> materialMap;

                  public:

                    /**
                     * Default constructor.
                     */
                    Model() : modelName(""),
                    current(nullptr),
                    currentMaterial(nullptr),
                    defaultMaterial(nullptr),
                    activeGroup(""),
                    currentMesh(nullptr) { }

                    /**
                     * Destructor.
                     */
                    virtual ~Model()
                    {
                        // Clear all stored object instances.
                        for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
                        {
                            delete *it;
                        }
                        objects.clear();

                        // Clear all stored mesh instances
                        for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
                        {
                            delete *it;
                        }
                        meshes.clear();

                        // Clear all stored groups.
                        for (GroupMapIt it = groups.begin(); it != groups.end(); ++it)
                        {
                            delete it->second;
                        }
                        groups.clear();

                        // TODO: delete material map?
                    }
                };

            }
        }
    }
}

#endif	/* OBJFILEDATA_HPP */

