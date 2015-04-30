/* 
 * File:   ObjFileMtlImporter.h
 * Author: tku
 *
 * Created on 13 październik 2014, 13:54
 */

#ifndef OBJFILEMTLIMPORTER_HPP
#define	OBJFILEMTLIMPORTER_HPP

#include "NGE/Geometry/Models/Obj/ObjFileData.hpp"


namespace NGE
{
    namespace Geometry
    {
        namespace Models
        {
            namespace Obj
            {

                /**
                 * ObjFileMtlImporter is a class that loads material description
                 * from a mtl file.
                 */
                class ObjFileMtlImporter
                {
                  public:
                    static const size_t BUFFERSIZE = 2048;
                    typedef std::vector<char> DataArray;
                    typedef std::vector<char>::iterator DataArrayIt;
                    typedef std::vector<char>::const_iterator ConstDataArrayIt;

                  public:
                    /**
                     * Default constructor.
                     * @param buffer
                     * @param absPath
                     * @param model
                     */
                    ObjFileMtlImporter(std::vector<char>& buffer, const std::string& absPath, Obj::Model* model);

                    /**
                     * Destructor.
                     */
                    virtual ~ObjFileMtlImporter();

                  private:
                    /**
                     * Copy constructor, empty.
                     * @param other
                     */
                    ObjFileMtlImporter(const ObjFileMtlImporter& other);

                    /**
                     * Assignment operator, returns only a reference of this instance.
                     * @param other
                     * @return 
                     */
                    ObjFileMtlImporter& operator=(const ObjFileMtlImporter& other);

                    /**
                     * Load the whole material description.
                     */
                    void Load();

                    /**
                     * Get color data.
                     * @param color
                     */
                    void GetColorRGBA(Math::vec3f* color);

                    /**
                     * Get illumination model from loaded data.
                     * @param illumModel
                     */
                    void GetIlluminationModel(int& illumModel);

                    /**
                     * Gets a float value from data.
                     * @param value
                     */
                    void GetFloatValue(float& value);

                    /**
                     * Creates a new material from loaded data.
                     */
                    void CreateMaterial();

                    /**
                     * Get texture name from loaded data.
                     */
                    void GetTexture();

                  private:
                    /**
                     * Absolute pathname.
                     */
                    std::string absPath;

                    /**
                     * Data iterator showing to the current position in data buffer.
                     */
                    DataArrayIt dataIt;

                    /**
                     * Data iterator to end of buffer.
                     */
                    DataArrayIt dataItEnd;

                    /**
                     * Used model instance.
                     */
                    Obj::Model* model;

                    /**
                     * Current line in file.
                     */
                    unsigned int line;

                    /**
                     * Helper buffer.
                     */
                    char buffer[BUFFERSIZE];
                };
            }
        }
    }
}

#endif	/* OBJFILEMTLIMPORTER_HPP */

