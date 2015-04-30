/* 
 * File:   MD5Loader.h
 * Author: tku
 *
 * Created on 20 październik 2014, 15:10
 */

#ifndef MD5LOADER_H
#define	MD5LOADER_H

#include <string>

namespace NGE
{
    namespace Geometry
    {
        namespace Models
        {
            namespace MD5
            {

                /**
                 * Importer class for the MD5 file format.
                 */
                class MD5Importer
                {
                  public:
                    /**
                     * Default constructor.
                     */
                    MD5Importer();

                    /**
                     * Destructor.
                     */
                    virtual ~MD5Importer();

                    /**
                     * Returns whether the class can handle the format of the given file.
                     * @param file Filepath to the file that will be read.
                     * @return True if this is a proper file, otherwise false.
                     */
                    bool CanRead(const std::string& file) const;
                };
            }
        }
    }
}

#endif	/* MD5LOADER_H */

