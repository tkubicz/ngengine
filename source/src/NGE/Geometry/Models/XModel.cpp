#include <fstream>
#include <zlib.h>
#include <stdlib.h>

#include "NGE/Geometry/Models/XModel.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Math/NumberConverter.hpp"
#include "NGE/Appearance/Scene/Animation.hpp"
#include "NGE/Appearance/Scene/Scene.hpp"

using namespace NGE::Geometry::Models;

// Magic identifier for MSZIP compressed data.
#define MSZIP_MAGIC 0x4B43
#define MSZIP_BLOCK 32786

bool XModel::Initialise(const std::string& filename) {
	std::ifstream file;
	file.open(filename.c_str(), std::ios::in);

	if (!file) {
		log_error("XModel -> Failed to open file " + filename + ".");
		return false;
	}

	// Check the file size.
	file.seekg(0, std::ios::end);
	std::streampos length = file.tellg();
	if (length) {
		file.seekg(0, std::ios::beg);
		buffer.resize(static_cast<std::size_t> (length));
		file.read(&buffer.front(), static_cast<std::size_t> (length));
	}

	Parse();

	genericScene = new Appearance::Scene::Scene;

	CreateAnimations();

	return true;
}

void* XModel::Dummy_alloc(void*, unsigned int items, unsigned int size) {
	return ::operator new(items * size);
}

void XModel::Dummy_free(void*, void* address) {
	return ::operator delete(address);
}

bool XModel::Parse() {
	majorVersion = minorVersion = 0;
	isBinaryFormat = false;
	binaryNumCount = 0;
	lineNumber = 0;
	p = end = NULL;
	scene = NULL;

	// Vector to store uncompressed file for INFLATE'd files.
	std::vector<char> uncompressed;

	// Set up memory pointers.
	p = &buffer.front();
	end = p + buffer.size() - 1;

	// Check file header.
	if (strncmp(p, "xof ", 4) != 0) {
		log_error("XModel -> Header mismatch. File is not an XFile.");
		return false;
	}

	// Read version. It comes in four byte format such as "0302".
	majorVersion = (unsigned int) (p[4] - 48) * 10 + (unsigned int) (p[5] - 48);
	minorVersion = (unsigned int) (p[6] - 48) * 10 + (unsigned int) (p[7] - 48);

	bool compressed = false;

	// txt - pure ASCII text format.
	if (strncmp(p + 8, "txt ", 4) == 0)
		isBinaryFormat = false;

		// bin - Binary format.
	else if (strncmp(p + 8, "bin ", 4) == 0)
		isBinaryFormat = true;

		// tzip - Inflate compressed text format.
	else if (strncmp(p + 8, "tzip", 4) == 0) {
		isBinaryFormat = false;
		compressed = true;
	}
		// bzip - Inflate compressed binary format.
	else if (strncmp(p + 8, "bzip", 4) == 0) {
		isBinaryFormat = true;
		compressed = true;
	}
		// Unsupported format
	else {
		log_error("XModel -> Unsupported xfile format.");
		return false;
	}

	binaryFloatSize = (unsigned int) (p[12] - 48) * 1000
			+ (unsigned int) (p[13] - 48) * 100
			+ (unsigned int) (p[14] - 48) * 10
			+ (unsigned int) (p[15] - 48);

	if (binaryFloatSize != 32 && binaryFloatSize != 64) {
		log_error("XModel -> Unknown float size specified in xfile header.");
		return false;
	}

	p += 16;

	// If this is a compressed X file, apply the inflate algorithm to it.
	if (compressed) {
		/**
		 * Compressed X file format
		 * ---------------------------------
		 *  [xhead]
		 *  2 major
		 *  2 minor
		 *  4 type  // bzip, tzip
		 *  [mszip_master_head]
		 *  2 ofs   // offset to next section
		 *  2 magic // 'CK'
		 *  ... ofs bytes of data
		 *  ... next mszip_head
		 */

		// Build zlib stream.
		z_stream stream;
		stream.opaque = NULL;
		stream.zalloc = &XModel::Dummy_alloc;
		stream.zfree = &XModel::Dummy_free;
		stream.data_type = (isBinaryFormat) ? Z_BINARY : Z_ASCII;

		// Initialise the inflation algorithm.
		::inflateInit2(&stream, -MAX_WBITS);

		// Skip uknown data (checksum, flags?)
		p += 6;

		// First find out how much storage we'll need. Count selections.
		const char* p1 = p;
		unsigned int est_out = 0;

		while (p1 + 3 < end) {
			// Read next offset.
			uint16_t ofs = *((uint16_t*) p1);
			// TODO: Swap offset if the system is BIG ENDIAN
			//uint8_t* temp1 = reinterpret_cast<uint8_t*>(&ofs);
			//std::swap(temp1[0], temp1[1]);

			p1 += 2;

			if (ofs >= MSZIP_BLOCK) {
				log_error("XFile -> Invalid offset to next MSZIP compressed block.");
				return false;
			}

			// Check magic word.
			uint16_t magic = *((uint16_t*) p1);
			// TODO: Swap offset if the system is BIG ENDIAN
			//uint8_t* temp2 = reinterpret_cast<uint8_t*>(&magic);
			//std::swap(temp2[0], temp2[1]);
			p1 += 2;

			if (magic != MSZIP_MAGIC) {
				log_error("XFile -> Unsupported compressed format, expected MSZIP header.");
				return false;
			}

			// Advance to the next offset.
			p1 += ofs;
			// One decompressed block is 32786 in size.
			est_out += MSZIP_BLOCK;
		}

		// Allocate storage, terminate zero and do the actual uncompressing
		uncompressed.reserve(est_out + 1);
		char* out = &uncompressed.front();
		while (p + 3 < end) {
			uint16_t ofs = *((uint16_t*) p);
			// TODO: Swap offset if the system is BIG ENDIAN
			p += 4;

			// Push data to the stream.
			stream.next_in = (Bytef*) p;
			stream.avail_in = ofs;
			stream.next_out = (Bytef*) out;
			stream.avail_out = MSZIP_BLOCK;

			// Decompress the data.
			int ret = ::inflate(&stream, Z_SYNC_FLUSH);
			if (ret != Z_OK && ret != Z_STREAM_END) {
				log_error("XFile -> Failed to decompress MSZIP-compressed data.");
				return false;
			}

			::inflateReset(&stream);
			::inflateSetDictionary(&stream, (const Bytef*) out, MSZIP_BLOCK - stream.avail_out);

			// Advance to the next offset.
			out += MSZIP_BLOCK - stream.avail_out;
			p += ofs;
		}

		// Terminate zlib.
		::inflateEnd(&stream);

		// Update pointers to point to the uncompressed file data.
		p = &uncompressed[0];
		end = out;

		// TODO: Release compressed data.
		log_info("XFile -> Successfully decompressed MSZIP-compressed file.");
	} else {
		// Start reading here.
		ReadUntilEndOfLine();
	}

	scene = new Scene;
	ParseFile();

	//Filter the imported hierarchy for some degenerated cases
	/* TODO: Uncomment this
	if (scene->rootNode)
		FilterHierarchy(scene->rootNode); */
}

XModel::Scene* XModel::GetScene() {
	return scene;
}

NGE::Appearance::Scene::Scene* XModel::GetGenericScene() {
	return genericScene;
}

void XModel::ParseFile() {
	bool running = true;
	while (running) {
		// Read name of next object.
		std::string objectName = GetNextToken();
		if (objectName.length() == 0)
			break;

		// Parse specific object.
		if (objectName == "template")
			ParseDataObjectTemplate();
		else if (objectName == "Frame")
			ParseDataObjectFrame(NULL);
		else if (objectName == "Mesh") {
			// Some meshes have no frames at all
			Mesh* mesh = new Mesh;
			ParseDataObjectMesh(mesh);
			scene->globalMeshes.push_back(mesh);
		} else if (objectName == "AnimTicksPerSecond") {
			ParseDataObjectAnimTicksPerSecond();
		} else if (objectName == "AnimationSet") {
			ParseDataObjectAnimationSet();
		} else if (objectName == "Material") {
			// Material outside of a mesh or node
			Material material;
			ParseDataObjectMaterial(&material);
			scene->globalMaterials.push_back(material);
		} else if (objectName == "}") {
			// Whatever?
			log_info("XModel -> Found \'}\' in dataObject.");
		} else {
			// Unknown format
			log_info("XModel -> Unknown data object in x file: \'" + objectName + "\'.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectTemplate() {
	// Parse a template data object. Currently not stored.
	std::string name;
	ReadHeadOfDataObject(&name);

	// Read GUID.
	std::string guid = GetNextToken();

	// Read and ignore data members.
	bool running = true;
	while (running) {
		std::string s = GetNextToken();

		if (s == "}")
			break;

		if (s.length() == 0)
			log_error("XModel -> Unexcepted end of file reached while parsing template definition.");
	}
}

void XModel::ParseDataObjectFrame(Node* parent) {
	// A coordinate frame, or "frame of reference". The frame template
	// is open and can contain any object. The Direct3D extensions (D3DX)
	// mesh-loading functions recognize Mesh, FrameTransformMatrix and
	// Frame template instances as child objects when loading a Frame instance.
	std::string name;
	ReadHeadOfDataObject(&name);

	// Create a named node and place it at its parent, if given.
	Node* node = new Node(parent);
	node->name = name;
	if (parent)
		parent->children.push_back(node);
	else {
		// There might be multiple root nodes.
		if (scene->rootNode != NULL) {
			// Place a dummy root if not there.
			if (scene->rootNode->name != "$dummy_root") {
				Node* exroot = scene->rootNode;
				scene->rootNode = new Node(NULL);
				scene->rootNode->name = "$dummy_root";
				scene->rootNode->children.push_back(exroot);
				exroot->parent = scene->rootNode;
			}

			// Put the new node as its child instead.
			scene->rootNode->children.push_back(node);
			node->parent = scene->rootNode;
		} else {
			// It's the first node imported. Place it as root.
			scene->rootNode = node;
		}
	}

	// Now inside a frame.
	// Read tokens until closing brace is reached.
	bool running = true;
	while (running) {
		std::string objectName = GetNextToken();
		if (objectName.size() == 0) {
			log_error("XModel -> Unexcepted end of file reached while parsing frame.");
			return;
		}

		// Frame finished.
		if (objectName == "}")
			break;
			// Child frame
		else if (objectName == "Frame")
			ParseDataObjectFrame(node);
		else if (objectName == "FrameTransformMatrix")
			ParseDataObjectTransformationMatrix(node->trafoMatrix);
		else if (objectName == "Mesh") {
			Mesh* mesh = new Mesh;
			node->meshes.push_back(mesh);
			ParseDataObjectMesh(mesh);
		} else {
			log_info("XModel ->  Unknown data object in frame in x file.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectTransformationMatrix(Math::Matrix4<float>& matrix) {
	// Read header, we are not interested if it has a name.
	ReadHeadOfDataObject();

	// Read its components.
	matrix[0] = ReadFloat();
	matrix[1] = ReadFloat();
	matrix[2] = ReadFloat();
	matrix[3] = ReadFloat();
	matrix[4] = ReadFloat();
	matrix[5] = ReadFloat();
	matrix[6] = ReadFloat();
	matrix[7] = ReadFloat();
	matrix[8] = ReadFloat();
	matrix[9] = ReadFloat();
	matrix[10] = ReadFloat();
	matrix[11] = ReadFloat();
	matrix[12] = ReadFloat();
	matrix[13] = ReadFloat();
	matrix[14] = ReadFloat();
	matrix[15] = ReadFloat();

	// Trailing symbols.
	CheckForSemicolon();
	CheckForClosingBrace();
}

void XModel::ParseDataObjectMesh(Mesh* mesh) {
	std::string name;
	ReadHeadOfDataObject(&name);

	// Read vertex count.
	unsigned int numVertices = ReadInt();
	mesh->positions.resize(numVertices);

	// Read vertices.
	for (unsigned int i = 0; i < numVertices; ++i)
		mesh->positions[i] = ReadVector3();

	// Read position faces.
	unsigned int numPosFaces = ReadInt();
	mesh->posFaces.resize(numPosFaces);
	for (unsigned int i = 0; i < numPosFaces; ++i) {
		unsigned int numIndices = ReadInt();
		if (numIndices < 3) {
			log_error("XModel -> Invalid index count for face.");
			return;
		}

		// Read indices.
		Face& face = mesh->posFaces[i];
		for (unsigned int j = 0; j < numIndices; ++j)
			face.indices.push_back(ReadInt());
		CheckForSeparator();
	}

	// Here, outer data objects may follow.
	bool running = true;
	while (running) {
		std::string objectName = GetNextToken();

		if (objectName.size() == 0) {
			log_error("XModel -> Unexpected end of file while parsing mesh structure.");
			return;
		} else if (objectName == "}")
			// Mesh finished.
			break;
		else if (objectName == "MeshNormals")
			ParseDataObjectMeshNormals(mesh);
		else if (objectName == "MeshTextureCoords")
			ParseDataObjectMeshTextureCoords(mesh);
		else if (objectName == "MeshVertexColors")
			ParseDataObjectMeshVertexColors(mesh);
		else if (objectName == "MeshMaterialList")
			ParseDataObjectMeshMaterialList(mesh);
		else if (objectName == "VertexDuplicationIndices")
			// We will ignore vertex duplication indices.
			ParseUnknownDataObject();
		else if (objectName == "XSkinMeshHeader")
			ParseDataObjectSkinMeshHeader(mesh);
		else if (objectName == "SkinWeights")
			ParseDataObjectSkinWeights(mesh);
		else {
			log_info("XModel -> Unknown data object in mesh in x file.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectMeshNormals(Mesh* mesh) {
	ReadHeadOfDataObject();

	// Read count.
	unsigned int numNormals = ReadInt();
	mesh->normals.resize(numNormals);

	// Read normal vectors.
	for (unsigned int i = 0; i < numNormals; ++i)
		mesh->normals[i] = ReadVector3();

	// Read normal indices.
	unsigned int numFaces = ReadInt();
	if (numFaces != mesh->posFaces.size()) {
		log_error("XModel -> Normal face count does not match vertex face count.");
		return;
	}

	for (unsigned int i = 0; i < numFaces; ++i) {
		unsigned int numIndices = ReadInt();
		mesh->normFaces.push_back(Face());
		Face& face = mesh->normFaces.back();

		for (unsigned int j = 0; j < numIndices; ++j)
			face.indices.push_back(ReadInt());

		CheckForSeparator();
	}

	CheckForClosingBrace();
}

void XModel::ParseDataObjectMeshTextureCoords(Mesh* mesh) {
	ReadHeadOfDataObject();
	// TODO: Change this to variable of maximum textures
	if (mesh->numTextures + 1 > 8) {
		log_error("XModel -> Too many sets of texture coordinates.");
		return;
	}

	std::vector<Math::Vector2<float> >& coords = mesh->texCoords[mesh->numTextures++];

	unsigned int numCoords = ReadInt();
	if (numCoords != mesh->positions.size()) {
		log_error("XModel -> Texture coord count does not match vertex count.");
		return;
	}

	coords.resize(numCoords);
	for (unsigned int i = 0; i < numCoords; ++i)
		coords[i] = ReadVector2();

	CheckForClosingBrace();
}

void XModel::ParseDataObjectMeshVertexColors(Mesh* mesh) {
	ReadHeadOfDataObject();
	// TODO: Change this to variable of macimum color sets.
	if (mesh->numColorSets + 1 > 8) {
		log_error("XModel -> Too many color sets.");
		return;
	}

	std::vector<Math::Vector4<float> >& colors = mesh->colors[mesh->numColorSets];

	unsigned int numColors = ReadInt();
	if (numColors != mesh->positions.size()) {
		log_error("XModel -> Vertex color count does not match vertex count.");
		return;
	}

	colors.resize(numColors, Math::Vector4<float>(0, 0, 0, 1));
	for (unsigned int i = 0; i < numColors; ++i) {
		unsigned int index = ReadInt();
		if (index >= mesh->positions.size()) {
			log_error("XModel -> Vertex color index out of bounds.");
			return;
		}

		colors[index] = ReadRGBA();

		// Hack by thom: Maxon Cinema XPort plugin puts a third separator here, kwxPort puts a comma.
		if (!isBinaryFormat) {
			FindNextNoneWhiteSpace();
			if (*p == ';' || *p == ',')
				p++;
		}
	}

	CheckForClosingBrace();
}

void XModel::ParseDataObjectMeshMaterialList(Mesh* mesh) {
	ReadHeadOfDataObject();

	// Read material count.
	/* unsigned int numMaterials =*/ ReadInt();
	// Read non triangulated face material index count.
	unsigned int numMatIndices = ReadInt();

	// Some models have a material index count of 1... to be able to read them
	// we replicate this single material index on every face.
	if (numMatIndices != mesh->posFaces.size() && numMatIndices != 1) {
		log_error("XModel -> Per-Face material index count does not match face count.");
		return;
	}

	// Read per-face material indices.
	for (unsigned int i = 0; i < numMatIndices; ++i)
		mesh->faceMaterials.push_back(ReadInt());

	// In Version 03.02, the face indices ends with two semicolons.
	// Commented out version check, as version 03.03 exported from blender also has 2 semicolons.
	if (!isBinaryFormat) {
		if (p < end && *p == ';')
			++p;
	}

	// If there was only a single material index, replicate it on all faces.
	while (mesh->faceMaterials.size() < mesh->posFaces.size())
		mesh->faceMaterials.push_back(mesh->faceMaterials.front());

	// Read following data objects.
	bool running = true;
	while (running) {
		std::string objectName = GetNextToken();
		if (objectName.size() == 0) {
			log_error("XModel -> Unexpected end of file while parsing mesh material list.");
			return;
		} else if (objectName == "}")
			// Material list finished.
			break;
		else if (objectName == "{") {
			// Template materials.
			std::string matName = GetNextToken();
			Material material;
			material.isReference = true;
			material.name = matName;
			mesh->materials.push_back(material);

			CheckForClosingBrace();
		} else if (objectName == "Material") {
			mesh->materials.push_back(Material());
			ParseDataObjectMaterial(&mesh->materials.back());
		} else if (objectName == ";") {
			// Ignore
		} else {
			log_info("XModel -> Unknown data object in material list in x file.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectSkinMeshHeader(Mesh* mesh) {
	ReadHeadOfDataObject();

	/* unsigned int maxSkinWeightsPerVertex = */ ReadInt();
	/* unsigned int maxSkinWeightsPerFace = */ ReadInt();
	/* unsigned int numBonesInMesh = */ ReadInt();

	CheckForClosingBrace();
}

void XModel::ParseDataObjectSkinWeights(Mesh* mesh) {
	ReadHeadOfDataObject();

	std::string transformNodeName;
	GetNextTokenAsString(transformNodeName);

	mesh->bones.push_back(Bone());
	Bone& bone = mesh->bones.back();
	bone.name = transformNodeName;

	// Read vertex weights.
	unsigned int numWeights = ReadInt();
	bone.weights.reserve(numWeights);

	for (unsigned int i = 0; i < numWeights; ++i) {
		BoneWeight weight;
		weight.vertex = ReadInt();
		bone.weights.push_back(weight);
	}

	// Read vertex weights.
	for (unsigned int i = 0; i < numWeights; ++i)
		bone.weights[i].weight = ReadFloat();

	// Read matrix offset.
	bone.offsetMatrix[0] = ReadFloat();
	bone.offsetMatrix[1] = ReadFloat();
	bone.offsetMatrix[2] = ReadFloat();
	bone.offsetMatrix[3] = ReadFloat();
	bone.offsetMatrix[4] = ReadFloat();
	bone.offsetMatrix[5] = ReadFloat();
	bone.offsetMatrix[6] = ReadFloat();
	bone.offsetMatrix[7] = ReadFloat();
	bone.offsetMatrix[8] = ReadFloat();
	bone.offsetMatrix[9] = ReadFloat();
	bone.offsetMatrix[10] = ReadFloat();
	bone.offsetMatrix[11] = ReadFloat();
	bone.offsetMatrix[12] = ReadFloat();
	bone.offsetMatrix[13] = ReadFloat();
	bone.offsetMatrix[14] = ReadFloat();
	bone.offsetMatrix[15] = ReadFloat();

	CheckForSemicolon();
	CheckForClosingBrace();
}

void XModel::ParseDataObjectMaterial(Material* material) {
	std::string matName;
	ReadHeadOfDataObject(&matName);

	if (matName.empty())
		matName = std::string("material") + boost::lexical_cast<std::string>(lineNumber);
	material->name = matName;
	material->isReference = false;

	// Read material values.
	material->diffuse = ReadRGBA();
	material->specularExponenet = ReadFloat();
	material->specular = ReadRGB();
	material->emissive = ReadRGB();

	// Read other data objects.
	bool running = true;
	while (running) {
		std::string objectName = GetNextToken();
		if (objectName.size() == 0) {
			log_error("XModel -> Unexpected end of file while parsing mesh material.");
			return;
		} else if (objectName == "}")
			// Material finished.
			break;
		else if (objectName == "TextureFilename" || objectName == "TextureFileName") {
			// Some exporters write "TextureFileName" instead.
			std::string texname;
			ParseDataObjectTextureFilename(texname);
			material->textures.push_back(TexEntry(texname));
		} else if (objectName == "NormalmapFilename" || objectName == "NormalmapFileName") {
			// One exporter writes out the normal map in a separate filename tag.
			std::string texname;
			ParseDataObjectTextureFilename(texname);
			material->textures.push_back(TexEntry(texname, true));
		} else {
			log_info("XModel -> Unknown data object in material in x file.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectAnimTicksPerSecond() {
	ReadHeadOfDataObject();
	scene->animTickPerSecond = ReadInt();
	CheckForClosingBrace();
}

void XModel::ParseDataObjectAnimationSet() {
	std::string name;
	ReadHeadOfDataObject(&name);

	Animation* anim = new Animation();
	scene->anims.push_back(anim);
	anim->name = name;

	bool running = true;
	while (running) {
		std::string objectName = GetNextToken();
		if (objectName.length() == 0) {
			log_error("XModel -> Unexpected end of file while parsing animation set.");
			return;
		} else if (objectName == "}")
			// Animation set finished.
			break;
		else if (objectName == "Animation")
			ParseDataObjectAnimation(anim);
		else {
			log_info("XModel -> Unknown data object in animation set in x file.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectAnimation(Animation* anim) {
	ReadHeadOfDataObject();
	AnimBone* boneAnim = new AnimBone();
	anim->anims.push_back(boneAnim);

	bool running = true;
	while (running) {
		std::string objectName = GetNextToken();
		if (objectName.length() == 0) {
			log_error("XModel -> Unexpected end of file while parsing animation.");
			return;
		} else if (objectName == "}")
			// Animation finished.
			break;
		else if (objectName == "AnimationKey")
			ParseDataObjectAnimationKey(boneAnim);
		else if (objectName == "AnimationOptions")
			// Not interested.
			ParseUnknownDataObject();
		else if (objectName == "{") {
			// Read frame name.
			boneAnim->boneName = GetNextToken();
			CheckForClosingBrace();
		} else {
			log_info("XModel -> Unknown data object in animation in x file.");
			ParseUnknownDataObject();
		}
	}
}

void XModel::ParseDataObjectAnimationKey(AnimBone* animBone) {
	ReadHeadOfDataObject();

	// Read key type.
	unsigned int keyType = ReadInt();

	// Read number of keys.
	unsigned int numKeys = ReadInt();

	for (unsigned int i = 0; i < numKeys; ++i) {
		// Read time.
		unsigned int time = ReadInt();

		// Read keys.
		switch (keyType) {
			case 0: // Rotation quaternion.
			{
				// Read count.
				if (ReadInt() != 4) {
					log_error("XModel -> Invalid number of arguments for quaternion key in animation.");
					return;
				}

				Math::Types::QuaternionKey<float> key;
				Math::Quaternion<float> quat;

				quat.r = ReadFloat();
				quat.i = ReadFloat();
				quat.j = ReadFloat();
				quat.k = ReadFloat();

				key.SetTime((float) time);
				key.SetValue(quat);

				animBone->rotKeys.push_back(key);

				CheckForSemicolon();
				break;
			}

			case 1: // Scale vector.
			case 2: // Position vector.
			{
				// Read count.
				if (ReadInt() != 3) {
					log_error("XModel -> Invalid number of arguments for vector key in animation.");
					break;
				}

				Math::Types::Vector3Key<float> key;
				Math::Vector3<float> vec;
				vec = ReadVector3();

				key.SetTime((float) time);
				key.SetValue(vec);

				if (keyType == 2)
					animBone->posKeys.push_back(key);
				else
					animBone->scaleKeys.push_back(key);
				break;
			}

			case 3: // Combined transformation matrix.
			case 4: // Denoted both as 3 or as 4.
			{
				// Read count.
				if (ReadInt() != 16) {
					log_error("XModel -> Invalid number of arguments for matrix key in animation.");
					break;
				}

				// Read matrix.
				MatrixKey key;
				key.time = (double) time;

				key.matrix[0] = ReadFloat();
				key.matrix[1] = ReadFloat();
				key.matrix[2] = ReadFloat();
				key.matrix[3] = ReadFloat();
				key.matrix[4] = ReadFloat();
				key.matrix[5] = ReadFloat();
				key.matrix[6] = ReadFloat();
				key.matrix[7] = ReadFloat();
				key.matrix[8] = ReadFloat();
				key.matrix[9] = ReadFloat();
				key.matrix[10] = ReadFloat();
				key.matrix[11] = ReadFloat();
				key.matrix[12] = ReadFloat();
				key.matrix[13] = ReadFloat();
				key.matrix[14] = ReadFloat();
				key.matrix[15] = ReadFloat();

				animBone->transKeys.push_back(key);
				break;
			}

			default:
				log_error("XModel -> Unknown key type in animation.");
				break;
		}

		CheckForSeparator();
	}

	CheckForClosingBrace();
}

void XModel::ParseDataObjectTextureFilename(std::string& name) {
	ReadHeadOfDataObject();
	GetNextTokenAsString(name);
	CheckForClosingBrace();

	// FIX: some files (e.g. AnimationTest.x) have "" as texture file name
	if (!name.length()) {
		log_info("XModel -> Length of texture file name is zero. Skipping this texture.");
	}

	// Some exporters write double backslash paths out. We simply replace them if we find any.
	while (name.find("\\\\") != std::string::npos)
		name.replace(name.find("\\\\"), 2, "\\");
}

void XModel::ParseUnknownDataObject() {
	// Find opening delimiter.
	bool running = true;
	while (running) {
		std::string t = GetNextToken();
		if (t.length() == 0) {
			log_error("XModel -> Unexpected end of file while parsing unknown segment.");
			return;
		}

		if (t == "{")
			break;
	}

	unsigned int counter = 1;

	// Parse until closing delimiter
	while (counter > 0) {
		std::string t = GetNextToken();

		if (t.length() == 0) {
			log_error("XModel -> Unexpected end of file while parsing unknown segment.");
			return;
		}

		if (t == "{")
			++counter;
		else if (t == "}")
			--counter;
	}
}

std::string XModel::GetNextToken() {
	std::string s;

	// Process binary-formatted file.
	if (isBinaryFormat) {
		// In binary mode it will only return NAME and STRING token
		// and (correctly) skip over other tokens.

		if (end - p < 2) return s;
		unsigned int tok = ReadBinWord();
		unsigned int len;

		// Standalone tokens.
		switch (tok) {
			case 1:
				// Name token.
				if (end - p < 4)
					return s;
				len = ReadBinDWord();
				if (end - p < int(len))
					return s;
				s = std::string(p, len);
				p += len;
				return s;

			case 2:
				// String token.
				if (end - p < 4)
					return s;
				len = ReadBinDWord();
				if (end - p < int(len))
					return s;
				s = std::string(p, len);
				p += (len + 2);
				return s;

			case 3:
				// integer token
				p += 4;
				return "<integer>";

			case 5:
				// GUID token
				p += 16;
				return "<guid>";

			case 6:
				if (end - p < 4)
					return s;
				len = ReadBinDWord();
				p += (len * 4);
				return "<int_list>";

			case 7:
				if (end - p < 4)
					return s;
				len = ReadBinDWord();
				p += (len * binaryFloatSize);
				return "<flt_list>";

			case 0x0a:
				return "{";

			case 0x0b:
				return "}";

			case 0x0c:
				return "(";

			case 0x0d:
				return ")";

			case 0x0e:
				return "[";

			case 0x0f:
				return "]";

			case 0x10:
				return "<";

			case 0x11:
				return ">";

			case 0x12:
				return ".";

			case 0x13:
				return ",";

			case 0x14:
				return ";";

			case 0x1f:
				return "template";

			case 0x28:
				return "WORD";

			case 0x29:
				return "DWORD";

			case 0x2a:
				return "FLOAT";

			case 0x2b:
				return "DOUBLE";

			case 0x2c:
				return "CHAR";

			case 0x2d:
				return "UCHAR";

			case 0x2e:
				return "SWORD";

			case 0x2f:
				return "SDWORD";

			case 0x30:
				return "void";

			case 0x31:
				return "string";

			case 0x32:
				return "unicode";

			case 0x33:
				return "cstring";

			case 0x34:
				return "array";
		}
	}		// Process text-formatted file.
	else {
		FindNextNoneWhiteSpace();
		if (p >= end)
			return s;

		while ((p < end) && !isspace((unsigned char) *p)) {
			// Either keep token delimiters when already holding a token, or return
			// if first valid char.
			if (*p == ';' || *p == '}' || *p == '{' || *p == ',') {
				if (!s.size())
					s.append(p++, 1);
				// Stop for delimiter.
				break;
			}
			s.append(p++, 1);
		}
	}

	return s;
}

void XModel::ReadHeadOfDataObject(std::string* poName) {
	std::string nameOrBrace = GetNextToken();
	if (nameOrBrace != "{") {
		if (poName)
			*poName = nameOrBrace;

		if (GetNextToken() != "{")
			log_error("XModel -> Opening brace excepted.");
	}
}

void XModel::FindNextNoneWhiteSpace() {
	if (isBinaryFormat)
		return;

	bool running = true;
	while (running) {
		while (p < end && isspace((unsigned char) *p)) {
			if (*p == '\n')
				lineNumber++;
			++p;
		}

		if (p >= end)
			return;

		// Check if this is a comment
		if ((p[0] == '/' && p[1] == '/') || p[0] == '#')
			ReadUntilEndOfLine();
		else
			break;
	}
}

void XModel::CheckForClosingBrace() {
	if (GetNextToken() != "}")
		log_error("XModel -> Closing brace expected.");
}

void XModel::CheckForSemicolon() {
	if (isBinaryFormat)
		return;

	if (GetNextToken() != ";")
		log_error("XModel -> Semicolon excepted.");
}

void XModel::CheckForSeparator() {
	if (isBinaryFormat)
		return;

	std::string token = GetNextToken();
	if (token != "," && token != ";")
		log_error("XModel -> Separator character (\';\' or \',\') expected.");
}

void XModel::TestForSeparator() {
	if (isBinaryFormat)
		return;

	FindNextNoneWhiteSpace();
	if (p >= end)
		return;

	// Test and skip.
	if (*p == ';' || *p == ',')
		p++;
}

void XModel::GetNextTokenAsString(std::string& string) {
	if (isBinaryFormat) {
		string = GetNextToken();
		return;
	}

	FindNextNoneWhiteSpace();
	if (p >= end) {
		log_error("XModel -> Unexpected end of file while parsing string.");
		return;
	}

	if (*p != '"') {
		log_error("XModel -> Expected quotation mark.");
		return;
	}

	++p;

	while (p < end && *p != '"')
		string.append(p++, 1);

	if (p >= end - 1) {
		log_error("XModel -> Unexpected end of file while parsing string.");
		return;
	}

	if (p[1] != ';' || p[0] != '"') {
		log_error("XModel -> Expected quotation mark and semicolon at the end of a string.");
		return;
	}

	p += 2;
}

unsigned short XModel::ReadBinWord() {
	assert(end - p >= 2);
	const unsigned char* q = (const unsigned char*) p;
	unsigned short tmp = q[0] | (q[1] << 8);
	p += 2;
	return tmp;
}

unsigned int XModel::ReadBinDWord() {
	assert(end - p >= 4);
	const unsigned char* q = (const unsigned char*) p;
	unsigned int tmp = q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24);
	p += 4;
	return tmp;
}

unsigned int XModel::ReadInt() {
	if (isBinaryFormat) {
		if (binaryNumCount == 0 && end - p >= 2) {
			// 0x06 or 0x03
			unsigned short tmp = ReadBinWord();
			// Array of ints follows
			if (tmp == 0x06 && end - p >= 4)
				binaryNumCount = ReadBinDWord();
			else // Single int follows
				binaryNumCount = 1;
		}

		--binaryNumCount;
		if (end - p >= 4)
			return ReadBinDWord();
		else {
			p = end;
			return 0;
		}
	} else {
		FindNextNoneWhiteSpace();

		bool isNegative = false;
		if (*p == '-') {
			isNegative = true;
			p++;
		}

		// At least one digit expected.
		if (!isdigit(*p)) {
			log_error("XModel -> Number expected.");
			return 0;
		}

		// Read digits.
		unsigned int number = 0;
		while (p < end) {
			if (!isdigit(*p))
				break;
			number = number * 10 + (*p - 48);
			p++;
		}

		CheckForSeparator();
		return isNegative ? ((unsigned int) - int (number)) : number;
	}
}

float XModel::ReadFloat() {
	if (isBinaryFormat) {
		if (binaryNumCount == 0 && end - p >= 2) {
			// 0x07 or 0x42.
			unsigned short tmp = ReadBinWord();
			// Array of floats following.
			if (tmp == 0x07 && end - p >= 4)
				binaryNumCount = ReadBinDWord();
			else // Single float following
				binaryNumCount = 1;
		}

		--binaryNumCount;
		if (binaryFloatSize == 8) {
			if (end - p >= 8) {
				float result = (float) (*(double*) p);
				p += 8;
				return result;
			} else {
				p = end;
				return 0;
			}
		} else {
			if (end - p >= 4) {
				float result = *(float*) p;
				p += 4;
				return result;
			} else {
				p = end;
				return 0;
			}
		}
	}

	// Text version.
	FindNextNoneWhiteSpace();
	// Check for various special strings to allow reading files from faulty exporters eg. Blender.
	// Reading is safe because of the terminating zero.
	if (strncmp(p, "-1.#IND00", 9) == 0 || strncmp(p, "1.#IND00", 8) == 0) {
		p += 9;
		CheckForSeparator();
		return 0.0f;
	} else if (strncmp(p, "1.#QNAN0", 8) == 0) {
		p += 8;
		CheckForSeparator();
		return 0.0f;
	}

	float result = 0.0f;
	p = Math::NumberConverter::FastAToFMove(p, result);
	CheckForSeparator();

	return result;
}

NGE::Math::Vector2<float> XModel::ReadVector2() {
	Math::Vector2<float> vector;
	vector.x = ReadFloat();
	vector.y = ReadFloat();
	TestForSeparator();

	return vector;
}

NGE::Math::Vector3<float> XModel::ReadVector3() {
	Math::Vector3<float> vector;
	vector.x = ReadFloat();
	vector.y = ReadFloat();
	vector.z = ReadFloat();
	TestForSeparator();

	return vector;
}

NGE::Math::Vector3<float> XModel::ReadRGB() {
	Math::Vector3<float> color;
	color.x = ReadFloat();
	color.y = ReadFloat();
	color.z = ReadFloat();
	TestForSeparator();

	return color;
}

NGE::Math::Vector4<float> XModel::ReadRGBA() {
	Math::Vector4<float> color;
	color.x = ReadFloat();
	color.y = ReadFloat();
	color.z = ReadFloat();
	color.w = ReadFloat();
	TestForSeparator();

	return color;
}

void XModel::ReadUntilEndOfLine() {
	if (isBinaryFormat)
		return;

	while (p < end) {
		if (*p == '\n' || *p == '\r') {
			++p;
			lineNumber++;
			return;
		}

		++p;
	}
}

void XModel::ConvertMaterials(const std::vector<Material>& materials) {
	// Count the non-referrer materials in the array
	unsigned int numMaterials = 0;
	for (unsigned int i = 0; i < materials.size(); ++i)
		if (!materials[i].isReference)
			numMaterials++;

	if (numMaterials == 0)
		return;

	// Resize the scene's material list to offer enough space for the new materials.
	Appearance::Material** prevMats = genericScene->GetMaterials();
	genericScene->SetMaterials(new Appearance::Material*[genericScene->GetNumMaterials() + numMaterials]);
	if (prevMats) {
		memcpy(genericScene->GetMaterials(), prevMats, genericScene->GetNumMaterials() * sizeof (Appearance::Material*));
		delete[] prevMats;
	}

	// Convert all the materials given in the array.
	for (unsigned int i = 0; i < materials.size(); ++i) {
		const XModel::Material& oldMat = materials[i];
		if (oldMat.isReference)
			continue;

		Appearance::Material* mat = new Appearance::Material();
		std::string name;
		name = oldMat.name;
		mat->SetName(oldMat.name);

		// In assimp here is "shading model".

		// Material Colors.
		// TODO: What about ambient color in material?
		mat->SetEmissiveColor(oldMat.emissive.x, oldMat.emissive.y, oldMat.emissive.z, 1.0);
		mat->SetDiffuseColor(oldMat.diffuse);
		mat->SetSpecularColor(oldMat.specular.x, oldMat.specular.y, oldMat.specular.z, 1.0);
		mat->SetShininess(oldMat.specularExponenet);

		// Texture, if there is one.
		if (oldMat.textures.size() == 1) {
			const XModel::TexEntry& oTex = oldMat.textures.back();
			if (oTex.name.length()) {
				// If there is only one texture assume it containts the diffuse color.
				std::string tex(oTex.name);
				if (oTex.isNormalMap) {
					// TODO: Add material as normal map
				} else {
					// TODO: Add material as diffuse color texture.
				}
			}
		} else {
			// TODO: Some kind of searching for typical strings in texture's files like 'bump' or 'diffuse'.
			// Do I need it? Most likely yes...
		}

		genericScene->GetMaterials()[genericScene->GetNumMaterials()] = mat;

	}
}

void XModel::CreateAnimations() {
	std::vector<Appearance::Scene::Animation*> newAnims;

	for (unsigned int i = 0; i < scene->anims.size(); ++i) {
		const XModel::Animation* anim = scene->anims[i];
		// Some exporters creates empty animation tags.
		if (anim->anims.size() == 0)
			continue;

		// Create a new animation to hold the data.
		Appearance::Scene::Animation* nAnim = new Appearance::Scene::Animation();
		newAnims.push_back(nAnim);
		nAnim->SetName(anim->name);

		// Duration will be determined by the maximum length.
		nAnim->SetDuration(0);
		nAnim->SetTicksPerSecond(scene->animTickPerSecond);
		nAnim->SetNumChannels((unsigned int) anim->anims.size());
		nAnim->SetChannels(new Appearance::Scene::NodeAnim*[nAnim->GetNumChannels()]);

		for (unsigned int j = 0; j < anim->anims.size(); ++j) {
			const XModel::AnimBone* bone = anim->anims[j];
			Appearance::Scene::NodeAnim* nBone = new Appearance::Scene::NodeAnim();
			nBone->SetNodeName(bone->boneName);
			nAnim->GetChannels()[j] = nBone;

			// Keyframes are given as combined transformation matrix keys.
			if (bone->transKeys.size() > 0) {
				nBone->SetNumPositionKeys((unsigned int) bone->transKeys.size());
				nBone->SetPositionKeys(new Math::Types::Vector3Key<float>[nBone->GetNumPositionKeys()]);
				nBone->SetNumRotationKeys((unsigned int) bone->transKeys.size());
				nBone->SetRotationKeys(new Math::Types::QuaternionKey<float>[nBone->GetNumRotationKeys()]);
				nBone->SetNumScalingKeys((unsigned int) bone->transKeys.size());
				nBone->SetScalingKeys(new Math::Types::Vector3Key<float>[nBone->GetNumScalingKeys()]);

				for (unsigned int k = 0; k < bone->transKeys.size(); ++k) {
					// Deconstruct each matrix into seperate position, rotation and scaling.
					double time = bone->transKeys[k].time;
					Math::mat4f trans = bone->transKeys[k].matrix;

					// Extract position.
					Math::vec3f pos(trans[12], trans[13], trans[14]);

					nBone->GetPositionKeys()[k].SetTime(time);
					nBone->GetPositionKeys()[k].SetValue(pos);

					// Extract scaling.
					Math::vec3f scale;
					scale.x = Math::vec3f(trans[0], trans[1], trans[2]).Length();
					scale.y = Math::vec3f(trans[4], trans[5], trans[6]).Length();
					scale.z = Math::vec3f(trans[8], trans[9], trans[10]).Length();
					nBone->GetScalingKeys()[k].SetTime(time);
					nBone->GetScalingKeys()[k].SetValue(scale);

					// Reconstruct rotation matrix without scaling.
					Math::mat3f rotmat(
							trans[0] / scale.x, trans[1] / scale.x, trans[2] / scale.x,
							trans[3] / scale.y, trans[4] / scale.y, trans[5] / scale.y,
							trans[6] / scale.z, trans[7] / scale.z, trans[8] / scale.z
							);

					// Convert it into quaternion.
					nBone->GetRotationKeys()[k].SetTime(time);
					nBone->GetRotationKeys()[k].SetValue(rotmat);
				}

				// Longest lasting key sequence determines duration.
				nAnim->SetDuration(std::max(nAnim->GetDuration(), bone->transKeys.back().time));
			} else {
				// Seperate key sequences for position, rotation and scaling.
				nBone->SetNumPositionKeys((unsigned int) bone->posKeys.size());
				nBone->SetPositionKeys(new Math::Types::Vector3Key<float>[nBone->GetNumPositionKeys()]);
				for (unsigned int k = 0; k < nBone->GetNumPositionKeys(); ++k) {
					Math::vec3f pos = bone->posKeys[k].GetValue();

					nBone->GetPositionKeys()[k].SetTime(bone->posKeys[k].GetTime());
					nBone->GetPositionKeys()[k].SetValue(pos);
				}

				// Rotation.
				nBone->SetNumRotationKeys((unsigned int) bone->rotKeys.size());
				nBone->SetRotationKeys(new Math::Types::QuaternionKey<float>[nBone->GetNumRotationKeys()]);
				for (unsigned int k = 0; k < nBone->GetNumRotationKeys(); ++k) {
					Math::mat3f rotMat = bone->rotKeys[k].GetValue().GetMatrix();

					nBone->GetRotationKeys()[k].SetTime(bone->rotKeys[k].GetTime());
					nBone->GetRotationKeys()[k].SetValue(Math::Quaternion<float>(rotMat));
					// Needs quaternion inversion.
					Math::Quaternion<float> q = nBone->GetRotationKeys()[k].GetValue();
					q.r *= -1.0f;
					nBone->GetRotationKeys()[k].SetValue(q);
				}

				// Scaling
				nBone->SetNumScalingKeys((unsigned int) bone->scaleKeys.size());
				nBone->SetScalingKeys(new Math::Types::Vector3Key<float>[nBone->GetNumScalingKeys()]);
				for (unsigned int k = 0; k < nBone->GetNumScalingKeys(); ++k)
					nBone->GetScalingKeys()[k] = bone->scaleKeys[k];

				// Longest lasting key sequence determines duration.
				if (bone->posKeys.size() > 0)
					nAnim->SetDuration(std::max(nAnim->GetDuration(), (double) bone->posKeys.back().GetTime()));
				if (bone->rotKeys.size() > 0)
					nAnim->SetDuration(std::max(nAnim->GetDuration(), (double) bone->rotKeys.back().GetTime()));
				if (bone->scaleKeys.size() > 0)
					nAnim->SetDuration(std::max(nAnim->GetDuration(), (double) bone->scaleKeys.back().GetTime()));
			}
		}
	}

	// Store all converted animations in the scene.
	if (newAnims.size() > 0) {
		genericScene->SetNumAnimations((unsigned int) newAnims.size());
		genericScene->SetAnimations(new Appearance::Scene::Animation*[genericScene->GetNumAnimations()]);
		for (unsigned int i = 0; i < newAnims.size(); ++i)
			genericScene->GetAnimations()[i] = newAnims[i];
	}
}
