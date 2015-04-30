/* 
 * File:   XModel.hpp
 * Author: tku
 *
 * Created on 20 luty 2014, 17:04
 */

#ifndef XMODEL_HPP
#define	XMODEL_HPP

#include <vector>
#include <string>

#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Math/Matrix4.hpp"
#include "NGE/Math/Types/QuaternionKey.hpp"
#include "NGE/Math/Types/Vector3Key.hpp"
#include "NGE/Appearance/Scene/Scene.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Models
        {

            class XModel
            {
              public:

                /**
                 * Helper structure representing an x file mesh face.
                 */
                struct Face
                {
                    std::vector<unsigned int> indices;
                };

                /**
                 * Helper structure representing a texture filename inside a material and its
                 * potential source.
                 */
                struct TexEntry
                {
                    std::string name;

                    /**
                     * True if the texname was specified in a NormalmapFilename tag.
                     */
                    bool isNormalMap;

                    TexEntry()
                    {
                        isNormalMap = false;
                    }

                    TexEntry(const std::string& name, bool isNormalMap = false)
                    {
                        this->name = name;
                        this->isNormalMap = isNormalMap;
                    }
                };

                /**
                 * Helper structure representing an x file material.
                 */
                struct Material
                {
                    std::string name;

                    /**
                     * If true, name holds a name by which the actual material can be
                     * found in the material list.
                     */
                    bool isReference;

                    Math::Vector4<float> diffuse;
                    float specularExponenet;
                    Math::Vector3<float> specular;
                    Math::Vector3<float> emissive;
                    std::vector<TexEntry> textures;

                    Material()
                    {
                        isReference = false;
                    }
                };

                /**
                 * Helper structure to represent a bone weight.
                 */
                struct BoneWeight
                {
                    unsigned int vertex;
                    float weight;
                };

                /**
                 * Helper structure to represent a bone in a mesh.
                 */
                struct Bone
                {
                    std::string name;
                    std::vector<BoneWeight> weights;
                    Math::Matrix4<float> offsetMatrix;
                };

                /**
                 * Helper structure to represent an x file mesh.
                 */
                struct Mesh
                {
                    std::vector<Math::Vector3<float> > positions;
                    std::vector<Face> posFaces;
                    std::vector<Math::Vector3<float> > normals;
                    std::vector<Face> normFaces;
                    unsigned int numTextures;
                    std::vector<Math::Vector2<float> > texCoords[8];
                    unsigned int numColorSets;
                    std::vector<Math::Vector4<float> > colors[8];

                    std::vector<unsigned int> faceMaterials;
                    std::vector<Material> materials;

                    std::vector<Bone> bones;

                    Mesh()
                    {
                        numTextures = 0;
                        numColorSets = 0;
                    }
                };

                /**
                 * Helper structure to represent an x file frame.
                 */
                struct Node
                {
                    std::string name;
                    Math::Matrix4<float> trafoMatrix;
                    Node* parent;
                    std::vector<Node*> children;
                    std::vector<Mesh*> meshes;

                    Node() : parent(NULL) { }
                    Node(Node* parent) : parent(parent) { }
                    ~Node()
                    {
                        for (unsigned int a = 0; a < children.size(); a++)
                            delete children[a];
                        for (unsigned int a = 0; a < meshes.size(); a++)
                            delete meshes[a];
                    }
                };
                
                struct MatrixKey
                {
                    double time;
                    Math::Matrix4<float> matrix;
                };
                
                /**
                 * Helper structure representing a single animated bone in x file.
                 */
                struct AnimBone
                {
                    std::string boneName;
                    std::vector<Math::Types::Vector3Key<float> > posKeys;
                    std::vector<Math::Types::QuaternionKey<float> > rotKeys;
                    std::vector<Math::Types::Vector3Key<float> > scaleKeys;
                    std::vector<MatrixKey> transKeys;
                };
                
                /**
                 * Helper structure to represent an animation set in x file.
                 */
                struct Animation
                {
                    std::string name;
                    std::vector<AnimBone*> anims;
                    
                    ~Animation()
                    {
                        for (unsigned int i = 0; i < anims.size(); ++i)
                            delete anims[i];
                    }
                };
                
                /**
                 * Helper structure to represent a scene.
                 */
                struct Scene
                {
                    Node* rootNode;
                    
                    /**
                     * Global meshes found outside of any frames.
                     */
                    std::vector<Mesh*> globalMeshes;
                    
                    /**
                     * Global materials found outside of any meshes.
                     */
                    std::vector<Material> globalMaterials;
                    
                    std::vector<Animation*> anims;
                    unsigned int animTickPerSecond;
                    
                    Scene()
                    {
                        rootNode = NULL;
                        animTickPerSecond = 0;
                    }
                    
                    ~Scene()
                    {
                        delete rootNode;
                        for (unsigned int i = 0; i < globalMeshes.size(); ++i)
                            delete globalMeshes[i];
                        for (unsigned int i = 0; i < anims.size(); ++i)
                            delete anims[i];
                    }
                };

                bool Initialise(const std::string& filename);
                
                Scene* GetScene();
                Appearance::Scene::Scene* GetGenericScene();

              protected:

                /* Methods used to parse X file */

                static void* Dummy_alloc(void* /* opaque */, unsigned int items, unsigned int size);
                static void Dummy_free(void* /* opaque */, void* address);
                bool Parse();
                void ParseFile();
                void ParseDataObjectTemplate();
                void ParseDataObjectFrame(Node* parent);
                void ParseDataObjectTransformationMatrix(Math::Matrix4<float>& matrix);
                void ParseDataObjectMesh(Mesh* mesh);
                void ParseDataObjectMeshNormals(Mesh* mesh);
                void ParseDataObjectMeshTextureCoords(Mesh* mesh);
                void ParseDataObjectMeshVertexColors(Mesh* mesh);
                void ParseDataObjectMeshMaterialList(Mesh* mesh);
                void ParseDataObjectSkinMeshHeader(Mesh* mesh);
                void ParseDataObjectSkinWeights(Mesh* mesh);
                void ParseDataObjectMaterial(Material* material);
                
                void ParseDataObjectAnimTicksPerSecond();
                void ParseDataObjectAnimationSet();
                void ParseDataObjectAnimation(Animation* anim);
                void ParseDataObjectAnimationKey(AnimBone* animBone);
                
                void ParseDataObjectTextureFilename(std::string& name);
                void ParseUnknownDataObject();

                std::string GetNextToken();
                
                void ReadHeadOfDataObject(std::string* poName = NULL);

                void FindNextNoneWhiteSpace();

                void CheckForClosingBrace();
                
                void CheckForSemicolon();
                
                void CheckForSeparator();

                /**
                 * Test and possibly consume a separator char, but does nothing
                 * if there was no separator.
                 */
                void TestForSeparator();
                
                void GetNextTokenAsString(std::string& string);

                unsigned short ReadBinWord();
                unsigned int ReadBinDWord();
                unsigned int ReadInt();
                float ReadFloat();
                NGE::Math::Vector2<float> ReadVector2();
                NGE::Math::Vector3<float> ReadVector3();
                NGE::Math::Vector3<float> ReadRGB();
                NGE::Math::Vector4<float> ReadRGBA();

                void ReadUntilEndOfLine();
                
                /* Methods used to create generic model from X model */
                void ConvertMaterials(const std::vector<Material>& materials);
                void CreateAnimations();

              protected:
                /**
                 * Buffer to hold the loaded file.
                 */
                std::vector<char> buffer;

                /**
                 * Major version of X file.
                 */
                unsigned int majorVersion;

                /**
                 * Minor version of X file.
                 */
                unsigned int minorVersion;

                /**
                 * True if the file is in binary format, otherwise false - 
                 * file is in the text format.
                 */
                bool isBinaryFormat;

                /**
                 * Memory pointer 
                 */
                const char* p;

                /**
                 * Memory pointer
                 */
                const char* end;

                /**
                 * Float size used in the file. It can be 32 or 64 bits.
                 */
                unsigned int binaryFloatSize;

                /**
                 * Counter for number arrays in binary format.
                 */
                unsigned int binaryNumCount;

                /**
                 * The number of the line when reading in text format.
                 */
                unsigned int lineNumber;
                
                /**
                 * Imported data.
                 */
                Scene* scene;
                
                /**
                 * Generic scene.
                 */
                Appearance::Scene::Scene* genericScene;
            };
        }
    }
}

#endif	/* XMODEL_HPP */

