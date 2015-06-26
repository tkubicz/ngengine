/* 
 * File:   ObjFileParser.h
 * Author: tku
 *
 * Created on 8 październik 2014, 15:33
 */

#ifndef OBJFILEPARSER_HPP
#define	OBJFILEPARSER_HPP

#include "NGE/Geometry/Models/Obj/ObjFileData.hpp"

namespace NGE {
	namespace Geometry {
		namespace Models {
			namespace Obj {

				/**
				 * Parser for an obj waveform files.
				 */
				class ObjFileParser {
				  public:
					static const size_t BUFFERSIZE = 4096;
					typedef std::vector<char> DataArray;
					typedef std::vector<char>::iterator DataArrayIt;
					typedef std::vector<char>::const_iterator ConstDataArrayIt;

				  public:
					/**
					 * Constructor with data array.
					 * @param data Data read from file.
					 * @param modelName Model name.
					 */
					ObjFileParser(std::vector<char>& data, const std::string& modelName);

					/**
					 * Destructor
					 */
					virtual ~ObjFileParser();

					/**
					 * Model getter.
					 * @return Parsed model.
					 */
					Obj::Model* GetModel() const;

				  private:
					/**
					 * Parse the loaded file.
					 */
					void ParseFile();

					/**
					 * Method to copy the new delimited word in the current line.
					 * @param buffer
					 * @param length
					 */
					void CopyNextWorld(char* buffer, size_t length);

					/**
					 * Method to copy the new line.
					 * @param buffer
					 * @param length
					 */
					void CopyNextLine(char* buffer, size_t length);

					/**
					 * Stores the following 3d vector.
					 * @param point3DArray
					 */
					void GetVector3(std::vector<Math::vec3f>& point3DArray);

					/**
					 * Stores the following 2d vector.
					 * @param point2DArray
					 */
					void GetVector2(std::vector<Math::vec2f>& point2DArray);

					/**
					 * Stores the following face.
					 */
					void GetFace();

					/**
					 * Gets the material description.
					 */
					void GetMaterialDesc();

					/**
					 * Gets a comment.
					 */
					void GetComment();

					/**
					 * Gets a material library.
					 */
					void GetMaterialLib();

					/**
					 * Creates a new material.
					 */
					void GetNewMaterial();

					/**
					 * Gets the groupname from file.
					 */
					void GetGroupName();

					/**
					 * Gets the group number from file.
					 */
					void GetGroupNumber();

					/**
					 * Returns the index of the material.
					 * @param strMaterialName Name of the material.
					 * @return Index of the material, of -1 if no material
					 * was found.
					 */
					int GetMaterialIndex(const std::string& strMaterialName);

					/**
					 * Parse object name.
					 */
					void GetObjectName();

					/**
					 * Creates a new object.
					 * @param objectName New object name.
					 */
					void CreateObject(const std::string& objectName);

					/**
					 * Creates a new mesh.
					 */
					void CreateMesh();

					/**
					 * Returns true, if a new mesh instance must be created.
					 * @param materialName
					 * @return 
					 */
					bool NeedsNewMesh(const std::string& materialName);

					/**
					 * Error report in token.
					 */
					void ReportErrorTokenInFace();

				  private:
					/**
					 * Default material name.
					 */
					static const std::string DEFAULT_MATERIAL;

					/**
					 * Iterator to current position of the buffer.
					 */
					DataArrayIt dataIt;

					/**
					 * Iterator to end position of buffer.
					 */
					DataArrayIt dataItEnd;

					/**
					 * Pointer to model instance.
					 */
					Obj::Model* model;

					/**
					 * Current line (for debugging).
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

#endif	/* OBJFILEPARSER_HPP */

