#include <fstream>
#include "NGE/Geometry/Models/Obj/ObjFileImporter.hpp"
#include "NGE/Geometry/Models/Obj/ObjFileParser.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Geometry::Models::Obj;

ObjFileImporter::ObjFileImporter() : buffer(), rootObject(nullptr), strAbsPath("") { }

ObjFileImporter::~ObjFileImporter() {
	if (rootObject != nullptr) {
		delete rootObject;
		rootObject = nullptr;
	}
}

bool ObjFileImporter::CanRead(const std::string& file, bool checkSig) const {
	if (!checkSig) // Check file extension.
	{
		//return SimpleCheckExtension
	} else // Check file header.
	{
		//static const char* tokens[] = {"mtllib", "usemtl", "v ", "vt ", "vn ", "o ", "g ", "s ", "f "};
		// return BaseImporter::SearchFileHeaderForToken
	}

	return true;
}

void ObjFileImporter::InternReadFile(const std::string& fileName, Appearance::Scene::Scene* scene) {
	// Read file into memory.

	std::ifstream file;
	file.open(fileName.c_str(), std::ios::in);

	if (!file) {
		log_error("ObjFileImporter --> Failed to open " + fileName + ".");
		return;
	}

	// Check the file size.
	file.seekg(0, std::ios::end);
	std::streampos length = file.tellg();
	if (static_cast<std::size_t> (length) < 16) {
		log_error("ObjFileImporter --> OBJ-file is too small.");
		return;
	}

	// Allocate buffer and read file into it.
	file.seekg(0, std::ios::beg);
	buffer.resize(static_cast<std::size_t> (length));
	file.read(&buffer.front(), static_cast<std::size_t> (length));

	// Get the model name.
	std::string modelName;
	std::string::size_type pos = fileName.find_last_of("\\/");
	if (pos != std::string::npos) {
		modelName = fileName.substr(pos + 1, fileName.size() - pos - 1);
	} else {
		modelName = fileName;
	}

	// Parse the file into a temporary representation.
	ObjFileParser parser(buffer, modelName);

	// Create the proper return structures out of it.
	CreateDataFromFile(parser.GetModel(), scene);

	buffer.clear();
}

void ObjFileImporter::CreateDataFromFile(const Obj::Model* model, Appearance::Scene::Scene* scene) {
	if (model == nullptr)
		return;

	// Create the root node of the scene.
	scene->SetRootNode(new Appearance::Scene::SceneNode);
	if (model->modelName.empty()) {
		// Set the name of the scene.
		scene->GetRootNode()->SetName(model->modelName);
	} else {
		// This is an error, so break down the application.
		log_error("ObjFileImporter --> Error while creating data from file.");
		return;
	}

	// Create nodes for the whole scene.
	std::vector<Geometry::Mesh*> meshArray;
	for (size_t index = 0; index < model->objects.size(); index++) {
		//CreateNodes()
	}

	// Create mesh pointer buffer for this scene
	if (scene->GetNumMeshes() > 0) {
		//scene->GetMeshes() = new Geometry::Mesh*[meshArray.size()];
		for (size_t index = 0; index < meshArray.size(); index++) {
			scene->GetMeshes()[index] = meshArray[index];
		}
	}

	// Create all materials.
	//CreateMaterials(model, scene);
}

NGE::Appearance::Scene::SceneNode* ObjFileImporter::CreateNodes(const Obj::Model* model, const Obj::Object* data, unsigned int meshIndex, Appearance::Scene::SceneNode* parent, Appearance::Scene::Scene* scene, std::vector<Geometry::Mesh*>& meshArray) {
	if (model == nullptr)
		return nullptr;

	if (data == nullptr)
		return nullptr;

	// Store older mesh size to be able to computes mesh offsets for new mesh instances.
	const size_t oldMeshSize = meshArray.size();
	Appearance::Scene::SceneNode* node = new Appearance::Scene::SceneNode;

	node->SetName(data->objName);

	// If we have a parent node, store it.
	//if (parent != nullptr)
	//AppendChildToParentNode(parent, node);

	for (unsigned int i = 0; i < data->meshes.size(); i++) {
		unsigned int meshId = data->meshes[i];
		Geometry::Mesh* mesh = new Geometry::Mesh;
		CreateTopology(model, data, meshId, mesh);
		if (mesh->GetVertices().size() > 0) {
			meshArray.push_back(mesh);
		} else {
			delete mesh;
		}
	}

	// Create all nodes from the sub-objects stored in the current object.
	if (!data->subObjects.empty()) {
		size_t numChilds = data->subObjects.size();
		node->SetNumChildren(static_cast<unsigned int> (numChilds));
		//node->GetChildren() = new Appearance::Scene::SceneNode*[numChilds];
		node->SetNumMeshes(1);
		//node->GetMeshes() = new unsigned int[1];
	}

	// Set mesh instances into scene and node-instances.
	const size_t meshSizeDiff = meshArray.size() - oldMeshSize;
	if (meshSizeDiff > 0) {
		//node->GetMeshes() = new unsigned int[meshSizeDiff];
		node->SetNumMeshes(static_cast<unsigned int> (meshSizeDiff));
		size_t index = 0;
		for (size_t i = oldMeshSize; i < meshArray.size(); ++i) {
			node->GetMeshes()[index] = scene->GetNumMeshes();
			scene->SetNumMeshes(scene->GetNumMeshes() + 1);
			index++;
		}
	}

	return node;
}

void ObjFileImporter::CreateTopology(const Obj::Model* model, const Obj::Object* data, unsigned int meshIndex, Geometry::Mesh* mesh) {
	if (model == nullptr)
		return;
	if (data == nullptr)
		return;

	// Create faces
	Obj::Mesh* objMesh = model->meshes[meshIndex];
	if (objMesh == nullptr)
		return;

	for (size_t index = 0; index < objMesh->faces.size(); index++) {
		//Obj::Face * const inp = objMesh->faces[index];
		// TODO: if point
		// TODO: if line
		mesh->SetNumFaces(mesh->GetNumFaces() + 1);
	}

	//unsigned int idxCount = 0u;
	if (mesh->GetNumFaces() > 0) {

	}
}

