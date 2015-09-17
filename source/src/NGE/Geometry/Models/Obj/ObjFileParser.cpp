#include <fstream>

#include "NGE/Geometry/Models/Obj/ObjFileMtlImporter.hpp"
#include "NGE/Geometry/Models/Obj/ObjFileParser.hpp"
#include "NGE/Geometry/Models/Obj/ObjTools.hpp"
#include "NGE/Math/NumberConverter.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Geometry::Models::Obj;

const std::string ObjFileParser::DEFAULT_MATERIAL = "DefaultMaterial";

ObjFileParser::ObjFileParser(std::vector<char>& data, const std::string& modelName) :
dataIt(data.begin()), dataItEnd(data.end()), model(nullptr), line(0) {
	std::fill_n(buffer, BUFFERSIZE, 0);

	// Create the model instance to store all the data.
	model = new Obj::Model();
	model->modelName = modelName;

	model->defaultMaterial = new Obj::Material();
	model->defaultMaterial->materialName = DEFAULT_MATERIAL;
	model->materialLib.push_back(DEFAULT_MATERIAL);
	model->materialMap[DEFAULT_MATERIAL] = model->defaultMaterial;

	ParseFile();
}

ObjFileParser::~ObjFileParser() {
	delete model;
	model = nullptr;
}

NGE::Geometry::Models::Obj::Model* ObjFileParser::GetModel() const {
	return model;
}

void ObjFileParser::ParseFile() {
	if (dataIt == dataItEnd)
		return;

	while (dataIt != dataItEnd) {
		switch (*dataIt) {
			case 'v': // Parse a vertex texture coordinate
			{
				++dataIt;
				if (*dataIt == ' ') {
					// Read in vertex definition.
					GetVector3(model->vertices);
				} else if (*dataIt == 't') {
					// Read in texture coordinate (2D)
					++dataIt;
					GetVector2(model->textureCoords);
				} else if (*dataIt == 'n') {
					// Read in normal vector definition.
					++dataIt;
					GetVector3(model->normals);
				}
				break;
			}

			case 'p': // Parse a face, line or point statement.
			case 'l':
			case 'f':
			{
				// TODO: Fix that.
				GetFace();
				break;
			}

			case '#': // Parse a comment.
			{
				GetComment();
				break;
			}

			case 'u': // Parse a material desc. Setter.
			{
				GetMaterialDesc();
				break;
			}

			case 'm': // Parse a material lib
			{
				GetMaterialLib();
				break;
			}

			case 'g': // Parse group name.
			{
				GetGroupName();
				break;
			}

			case 's': // Parse group number.
			{
				GetGroupNumber();
				break;
			}

			case 'o': // Parse object name.
			{
				GetObjectName();
				break;
			}

			default:
			{
				dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
				break;
			}

		}
	}
}

void ObjFileParser::CopyNextWorld(char* buffer, size_t length) {
	size_t index = 0;
	dataIt = GetNextWord<DataArrayIt>(dataIt, dataItEnd);
	while (dataIt != dataItEnd && !IsSeparator(*dataIt)) {
		buffer[index] = *dataIt;
		index++;
		if (index == length - 1)
			break;
		++dataIt;
	}
	buffer[index] = '\0';
}

void ObjFileParser::CopyNextLine(char* buffer, size_t length) {
	size_t index = 0;
	while (dataIt != dataItEnd) {
		if (*dataIt == '\n' || *dataIt == '\r' || index == length - 1)
			break;

		buffer[index] = *dataIt;
		++index;
		++dataIt;
	}
	buffer[index] = '\0';
}

void ObjFileParser::GetVector3(std::vector<Math::vec3f>& point3DArray) {
	float x, y, z;

	CopyNextWorld(buffer, BUFFERSIZE);
	x = (float) Math::NumberConverter::FastAToF(buffer);

	CopyNextWorld(buffer, BUFFERSIZE);
	y = (float) Math::NumberConverter::FastAToF(buffer);

	CopyNextWorld(buffer, BUFFERSIZE);
	z = (float) Math::NumberConverter::FastAToF(buffer);

	point3DArray.push_back(Math::vec3f(x, y, z));
	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::GetVector2(std::vector<Math::vec2f>& point2DArray) {
	float x, y;
	CopyNextWorld(buffer, BUFFERSIZE);
	x = (float) Math::NumberConverter::FastAToF(buffer);

	CopyNextWorld(buffer, BUFFERSIZE);
	y = (float) Math::NumberConverter::FastAToF(buffer);

	point2DArray.push_back(Math::vec2f(x, y));
	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::GetFace() {
	CopyNextLine(buffer, BUFFERSIZE);
	if (dataIt == dataItEnd)
		return;

	char* ptr = buffer;
	char* end = &ptr[BUFFERSIZE];
	ptr = GetNextToken<char*>(ptr, end);
	if (ptr == end || *ptr == '\0')
		return;

	std::vector<unsigned int>* indices = new std::vector<unsigned int>;
	std::vector<unsigned int>* texID = new std::vector<unsigned int>;
	std::vector<unsigned int>* normalID = new std::vector<unsigned int>;
	bool hasNormal = false;

	const bool vt = (!model->textureCoords.empty());
	const bool vn = (!model->normals.empty());
	int step = 0, pos = 0;
	while (ptr != end) {
		step = 1;

		if (IsLineEnd(*ptr))
			break;

		if (*ptr == '/') {
			// if type == POINT
			// Obj: Separator unexpected in point statement.

			if (pos == 0) {
				// If there are no texture coordinates in the file, but normals.
				if (!vt && vn) {
					pos = 1;
					step++;
				}
			}
			pos++;
		} else if (IsSeparator(*ptr)) {
			pos = 0;
		} else {
			// Obj uses 1 base arrays.
			const int val = atoi(ptr);
			int tmp = val;
			while ((tmp = tmp / 10) != 0)
				++step;

			if (val > 0) {
				// Store parsed index.
				if (0 == pos)
					indices->push_back(val - 1);
				else if (1 == pos)
					texID->push_back(val - 1);
				else if (2 == pos) {
					normalID->push_back(val - 1);
					hasNormal = true;
				} else
					ReportErrorTokenInFace();
			}
		}
		ptr += step;
	}

	if (indices->empty()) {
		nge_log_error("ObjFileParser --> Ignoring empty face.");
		dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
		return;
	}

	Obj::Face* face = new Obj::Face(indices, normalID, texID);

	// Set active material, if one set.
	if (NULL != model->currentMaterial)
		face->material = model->currentMaterial;
	else
		face->material = model->defaultMaterial;

	// Create a default object, if nothing is there.
	if (NULL == model->current)
		CreateObject("defaultobject");

	// Assign face to mesh
	if (NULL == model->currentMesh)
		CreateMesh();

	// Store the face.
	model->currentMesh->faces.push_back(face);
	model->currentMesh->numIndices += (unsigned int) face->vertices->size();
	model->currentMesh->uvCoordinates[0] += (unsigned int) face->textureCoords[0].size();

	if (!model->currentMesh->hasNormals && hasNormal)
		model->currentMesh->hasNormals = true;

	// Skip the rest of the line.
	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::GetMaterialDesc() {
	// Each material request a new object.
	// Sometimes the object is already created (see 'o' tag by example), byt it is not initialized!
	// So, we create a new object only if the current os is already initialized!
	if (model->current != NULL &&
			(model->current->meshes.size() > 1 ||
			(model->current->meshes.size() == 1 && model->meshes[model->current->meshes[0]]->faces.size() != 0))
			) {
		model->current = nullptr;
	}

	// Get next data for material data.
	dataIt = GetNextToken<DataArrayIt>(dataIt, dataItEnd);
	if (dataIt == dataItEnd)
		return;

	char* start = &(*dataIt);
	while (dataIt != dataItEnd && IsSeparator(*dataIt))
		++dataIt;

	// Get name.
	std::string name(start, &(*dataIt));
	if (name.empty())
		return;

	// Search for material.
	std::map<std::string, Obj::Material*>::iterator it = model->materialMap.find(name);
	if (it == model->materialMap.end()) {
		// Not found, use default material.
		model->currentMaterial = model->defaultMaterial;
		nge_log_error("ObjFileParser --> Failed to locate material " + name + ", skipping.");
	} else {
		// Found, using detected material.
		model->currentMaterial = (*it).second;
		if (NeedsNewMesh(name)) {
			CreateMesh();
		}
		model->currentMesh->materialIndex = GetMaterialIndex(name);
	}

	// Skip rest of the line.
	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::GetComment() {
	while (dataIt != dataItEnd) {
		if ('\n' == (*dataIt)) {
			++dataIt;
			break;
		} else {
			++dataIt;
		}
	}
}

void ObjFileParser::GetMaterialLib() {
	// Translate tuple.
	dataIt = GetNextToken<DataArrayIt>(dataIt, dataItEnd);
	if (dataIt == dataItEnd)
		return;

	char* start = &(*dataIt);
	while (dataIt != dataItEnd && !IsNewLine(*dataIt))
		dataIt++;

	// Check for existance.
	const std::string matName(start, &(*dataIt));

	std::ifstream file;
	file.open(matName.c_str(), std::ios::in);

	if (!file) {
		nge_log_error("ObjFileParser --> Failed to open " + matName + ".");
		dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
		return;
	}

	// Check the file size.
	file.seekg(0, std::ios::end);
	std::streampos length = file.tellg();

	std::vector<char> buffer;
	file.seekg(0, std::ios::beg);
	buffer.resize(static_cast<std::size_t> (length));
	file.read(&buffer.front(), static_cast<std::size_t> (length));
	file.close();

	ObjFileMtlImporter mtlImporter(buffer, matName, model);
}

void ObjFileParser::GetNewMaterial() {
	dataIt = GetNextToken<DataArrayIt>(dataIt, dataItEnd);
	dataIt = GetNextWord<DataArrayIt>(dataIt, dataItEnd);
	if (dataIt == dataItEnd)
		return;

	char* start = &(*dataIt);
	std::string mat(start, *dataIt);
	while (dataIt != dataItEnd && IsSeparator(*dataIt))
		dataIt++;

	std::map<std::string, Obj::Material*>::iterator it = model->materialMap.find(mat);
	if (it == model->materialMap.end()) {
		// Show warning, if material was not found.
		nge_log_info("ObjFileParser --> Unsupported material requested: " + mat);
		model->currentMaterial = model->defaultMaterial;
	} else {
		// Set new material
		if (NeedsNewMesh(mat)) {
			CreateMesh();
		}
		model->currentMesh->materialIndex = GetMaterialIndex(mat);
	}

	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

int ObjFileParser::GetMaterialIndex(const std::string& strMaterialName) {
	int matIndex = -1;
	if (strMaterialName.empty())
		return matIndex;

	for (size_t index = 0; index < model->materialLib.size(); ++index) {
		if (strMaterialName == model->materialLib[index]) {
			matIndex = (int) index;
			break;
		}
	}

	return matIndex;
}

void ObjFileParser::GetGroupName() {
	// Get next word from data buffer.
	dataIt = GetNextToken<DataArrayIt>(dataIt, dataItEnd);
	dataIt = GetNextWord<DataArrayIt>(dataIt, dataItEnd);

	if (IsEndOfBuffer(dataIt, dataItEnd))
		return;

	// Store the group name in the group library.
	char* start = &(*dataIt);
	while (dataIt != dataItEnd && !IsSeparator(*dataIt))
		dataIt++;

	std::string groupName(start, &(*dataIt));

	// Change active group, if necessary.
	if (model->activeGroup != groupName) {
		// Search for already existing entry.
		Obj::Model::ConstGroupMapIt it = model->groups.find(groupName);

		// We are mapping groups into the object structure.
		CreateObject(groupName);

		// New group name, creating a new entry.
		if (it == model->groups.end()) {
			std::vector<unsigned int>* faceIDArray = new std::vector<unsigned int>;
			model->groups[groupName] = faceIDArray;
			model->groupFaceIDs = faceIDArray;
		} else {
			model->groupFaceIDs = (*it).second;
		}
		model->activeGroup = groupName;
	}

	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::GetGroupNumber() {
	// Not used.
	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::GetObjectName() {
	dataIt = GetNextToken<DataArrayIt>(dataIt, dataItEnd);
	if (dataIt == dataItEnd)
		return;

	char* start = &(*dataIt);
	while (dataIt != dataItEnd && !IsSeparator(*dataIt))
		++dataIt;

	std::string objectName(start, &(*dataIt));
	if (!objectName.empty()) {
		// Reset current object.
		model->current = NULL;

		// Search for actual object.
		for (std::vector<Obj::Object*>::const_iterator it = model->objects.begin(); it != model->objects.end(); ++it) {
			if ((*it)->objName == objectName) {
				model->current = *it;
				break;
			}
		}

		// Allocate a new object, if current one was not found before.
		if (NULL == model->current)
			CreateObject(objectName);
	}

	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
}

void ObjFileParser::CreateObject(const std::string& objectName) {
	if (model == nullptr) {
		nge_log_error("ObjFileParser --> Model pointer is null.");
		return;
	}

	model->current = new Obj::Object;
	model->current->objName = objectName;
	model->objects.push_back(model->current);

	CreateMesh();

	if (model->currentMaterial) {
		model->currentMesh->materialIndex = GetMaterialIndex(model->currentMaterial->materialName);
		model->currentMesh->material = model->currentMaterial;
	}
}

void ObjFileParser::CreateMesh() {
	if (model == nullptr) {
		nge_log_error("ObjFileParser --> Model pointer is null.");
		return;
	}

	model->currentMesh = new Obj::Mesh;
	model->meshes.push_back(model->currentMesh);
	unsigned int meshId = model->meshes.size() - 1;
	if (nullptr != model->current) {
		model->current->meshes.push_back(meshId);
	} else {
		nge_log_error("ObjFileParser --> No object detected to attach a new mesh instance.");
	}
}

bool ObjFileParser::NeedsNewMesh(const std::string& materialName) {
	if (model->currentMesh == 0) {
		// No mesh data yet.
		return true;
	}

	bool newMat = false;
	int matIndex = GetMaterialIndex(materialName);
	int currentMatIndex = model->currentMesh->materialIndex;
	if (currentMatIndex != int(Obj::Mesh::noMaterial) || currentMatIndex != matIndex) {
		// New material -> only one material per mesh, so we need to create a new material.
		newMat = true;
	}

	return newMat;
}

void ObjFileParser::ReportErrorTokenInFace() {
	dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
	nge_log_error("ObjFileParser --> Not supported token in face description detected.");
}
