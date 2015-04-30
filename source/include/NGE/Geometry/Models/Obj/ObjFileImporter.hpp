/* 
 * File:   ObjFileImporter.hpp
 * Author: tku
 *
 * Created on 8 październik 2014, 14:23
 */

#ifndef OBJFILEIMPORTER_HPP
#define	OBJFILEIMPORTER_HPP

#include "NGE/Geometry/Models/Obj/ObjFileData.hpp"
#include "NGE/Appearance/Scene/Scene.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Models
        {
            namespace Obj
            {

                /**
                 * Imports a waveform obj file.
                 */
                class ObjFileImporter
                {
                  public:
                    /**
                     * Default constructor.
                     */
                    ObjFileImporter();

                    /**
                     * Destructor
                     */
                    virtual ~ObjFileImporter();

                    /**
                     * Returns wheter the class can handle the format of the given file.
                     * @param file
                     * @param checkSig
                     * @return 
                     */
                    bool CanRead(const std::string& file, bool checkSig) const;

                    // TODO: Change this back to private afer testing!
                  public:

                    /**
                     * File import implementation.
                     */
                    void InternReadFile(const std::string& fileName, Appearance::Scene::Scene* scene);

                    /**
                     * Create the data from imported content.
                     * @param model
                     * @param scene
                     */
                    void CreateDataFromFile(const Obj::Model* model, Appearance::Scene::Scene* scene);

                    /**
                     * Create all nodes stored in imported content.
                     * @param model
                     * @param data
                     * @param meshIndex
                     * @param parent
                     * @param scene
                     * @param meshArray
                     * @return 
                     */
                    Appearance::Scene::SceneNode* CreateNodes(const Obj::Model* model, const Obj::Object* data, unsigned int meshIndex,
                                                              Appearance::Scene::SceneNode* parent, Appearance::Scene::Scene* scene, std::vector<Geometry::Mesh*>& meshArray);

                    /**
                     * Create topology data like faces and meshes for the geometry.
                     * @param model
                     * @param data
                     * @param meshIndex
                     * @param mesh
                     */
                    void CreateTopology(const Obj::Model* model, const Obj::Object* data, unsigned int meshIndex, Geometry::Mesh* mesh);

                  private:
                    /**
                     * Data buffer.
                     */
                    std::vector<char> buffer;

                    /**
                     * Pointer to root object instance.
                     */
                    Obj::Object* rootObject;

                    /**
                     * Absolute path name of model in filesystem.
                     */
                    std::string strAbsPath;
                };
            }
        }
    }
}

#endif	/* OBJFILEIMPORTER_HPP */

