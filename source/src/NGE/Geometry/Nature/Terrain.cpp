#include "NGE/Geometry/Nature/Terrain.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Core/Core.hpp"

#include <fstream>
#include <float.h>
#include <math.h>
#include <vector>

using namespace NGE::Geometry::Nature;

Terrain::Terrain(float heightScale, float blockScale) : heightScale(heightScale), blockScale(blockScale) {
	debug = wireframe = false;
	vertexBuffer = indexBuffer = colorBuffer = texCoordBuffer = normalBuffer = 0;
}

void Terrain::SetHeightScale(float heightScale) {
	this->heightScale = heightScale;
}

void Terrain::SetBlockScale(float blockScale) {
	this->blockScale = blockScale;
}

float Terrain::GetHeightValue(const unsigned char* data, unsigned char numBytes) {
	switch (numBytes) {
		case 1:
			return (unsigned char) (data[0]) / float(0xff);
			break;

		case 2:
			return (unsigned short) (data[1] << 8 | data[0]) / float(0xffff);
			//return (unsigned short) ((data[0] << 8) | data[1]);
			break;

		case 4:
			return (unsigned int) (data[3] << 24 || data[2] << 16 | data[1] << 8 | data[0]) / float(0xffffffff);
			break;

		default:
			log_error("Terrain --> Height filed with non standard pixel size");
			break;
	}

	return 0.0f;
}

float Terrain::GetPercentage(float value, const float min, const float max) {
	value = Math::MathUtils::Clamp(value, min, max);
	return (value - min) / (max - min);
}

void Terrain::GenerateVertices(const vector<float> heights) {
	float terrainBlockWidth = (terrainWidth - 1) * blockScale;
	float terrainBlockHeight = (terrainHeight - 1) * blockScale;

	float halfTerrainBlockWidth = terrainBlockWidth * 0.5f;
	float halfTerrainBlockHeight = terrainBlockHeight * 0.5f;

	unsigned int numVertices = terrainHeight * terrainWidth;
	vertices.reserve(numVertices);
	colors.resize(numVertices);

	for (int height = 0; height < terrainHeight; ++height) {
		for (int width = 0; width < terrainWidth; ++width) {
			int index = (height * terrainWidth) + width;
			float S = float(width) / float(terrainWidth - 1);
			float T = float(height) / float(terrainHeight - 1);

			float X = (S * terrainBlockWidth) - halfTerrainBlockWidth;
			float Y = heights[index] * heightScale;
			float Z = (T * terrainBlockHeight) - halfTerrainBlockHeight;

			vertices.push_back(Math::vec3f(X, Y, Z));

			float tex0Contribution = 1.0f - GetPercentage(heights[index], 0.0f, 0.75f);
			float tex2Contribution = 1.0f - GetPercentage(heights[index], 0.75f, 1.0f);
			colors[index] = Math::vec4f(tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution);
		}
	}

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (float) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void Terrain::GenerateIndices() {
	if (terrainHeight < 2 || terrainWidth < 2) {
		log_error("Terrain --> Terrain hasn't been loaded, or is of incorrect size");
		return;
	}

	const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;
	indices.resize(numTriangles * 3);

	unsigned int vertexIndex, index = 0;
	for (unsigned int height = 0; height < unsigned(terrainHeight - 1); ++height) {
		for (unsigned int width = 0; width < unsigned(terrainWidth - 1); ++width) {
			vertexIndex = (height * terrainWidth) + width;
			// Górny trójkąt
			indices[index++] = vertexIndex; // V0
			indices[index++] = vertexIndex + terrainWidth + 1; // V3
			indices[index++] = vertexIndex + 1; // V1
			// Dolny trójkąt
			indices[index++] = vertexIndex; // V0
			indices[index++] = vertexIndex + terrainWidth; // V2
			indices[index++] = vertexIndex + terrainWidth + 1; // V3
		}
	}

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Terrain::GenerateTexCoords() {
	if (terrainHeight < 2 || terrainWidth < 2) {
		log_error("Terrain --> Terrain hasn't been loaded, or is of incorrect size");
		return;
	}

	texCoords.reserve(terrainWidth * terrainHeight);

	for (int height = 0; height < terrainHeight; ++height) {
		for (int width = 0; width < terrainWidth; ++width) {
			float s = (float(width) / float(terrainWidth - 1)) * 64.0f;
			float t = (float(height) / float(terrainHeight - 1)) * 64.0f;
			texCoords.push_back(Math::vec2f(s, t));
		}
	}

	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * texCoords.size() * 2, &texCoords[0], GL_STATIC_DRAW);
}

void Terrain::GenerateNormals() {
	vector<Math::vec3f> faceNormals;
	vector<int> shareCount;

	normals.resize(vertices.size());
	shareCount.resize(vertices.size(), 0);

	unsigned numTriangles = indices.size() / 3;
	faceNormals.resize(numTriangles);

	for (unsigned i = 0; i < numTriangles; ++i) {
		Math::vec3f* v1 = &vertices[indices[i * 3]];
		Math::vec3f* v2 = &vertices[indices[(i * 3) + 1]];
		Math::vec3f* v3 = &vertices[indices[(i * 3) + 2]];

		Math::vec3f vec1, vec2, normal;

		vec1 = *v2 - *v1;
		vec2 = *v3 - *v1;

		normal = vec1.CrossProduct(vec2);
		normal.Normalize();

		for (int j = 0; j < 3; ++j) {
			int index = indices[(i * 3) + j];
			normals[index] += normal;
			shareCount[index]++;
		}
	}

	for (unsigned i = 0; i < vertices.size(); ++i) {
		normals[i] /= (float) shareCount[i];
		normals[i].Normalize();

		// SLOPE Base blend
		//float texture0Contribution = Math::MathUtils::Clamp(normals[i].DotProduct(Math::vec3f(0.0f, 1.0f, 0.0f)) - 0.1f, 0.0f, 1.0f);
		//colors[i] = Math::vec4f(texture0Contribution, texture0Contribution, texture0Contribution, colors[i].w);
	}

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * normals.size() * 3, &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * colors.size() * 4, &colors[0], GL_STATIC_DRAW);
}

bool Terrain::LoadHeightmap(const std::string& rawFile, int bitsPerPixel, int width, int height) {
	std::ifstream fileIn(rawFile.c_str(), std::ios::binary);

	if (!fileIn.is_open()) {
		log_error("Terrain --> Could not open file: [" + rawFile + "]");
		return false;
	}

	std::string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
	fileIn.close();

	const unsigned int bytesPerPixel = bitsPerPixel / 8;
	const unsigned int expectedFileSize = bytesPerPixel * width * height;

	terrainWidth = width;
	terrainHeight = height;

	if (stringBuffer.size() != expectedFileSize) {
		log_error("Terrain --> Expected file size: [" + to_string(expectedFileSize) + " bytes] differs from actual file size: ["
				+ to_string(stringBuffer.size()) + " bytes]");
		return false;
	}

	vector<float> heights;
	heights.reserve(width * height);

	for (int i = 0; i < (width * height); ++i) {
		float value = GetHeightValue(reinterpret_cast<unsigned char*> (&stringBuffer[i * bytesPerPixel]), bytesPerPixel);
		heights.push_back(value);
	}

	GenerateVertices(heights);
	GenerateIndices();
	GenerateTexCoords();
	GenerateNormals();

	// TODO: Przerzucić to gdzie indziej
	GenerateDebug();

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer((GLint) 1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer((GLint) 2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer((GLint) 3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);

	material0.SetAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	material0.SetDiffuseColor(0.3f, 0.3f, 0.3f, 1.0f);
	material0.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material0.SetEmissiveColor(0.0f, 0.3f, 0.0f, 1.0f);
	material0.SetShininess(80.0f);

	light0.SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	light0.SetDiffuse(1.0f, 1.0f, 0.0f, 1.0f);
	light0.SetSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	light0.SetPosition(100.0f, 1000.0f, 80.0f, 0.0f);
	light0.SetConstantAttenuation(0.3f);
	light0.SetLinearAttenuation(0.1f);
	light0.SetQuadraticAttenuation(0.01f);
	light0.SetSpotExponenet(0);
	light0.SetSpotCutOff(0);
	light0.SetSpotDirection(0, 1, 0);

	return true;
}

bool Terrain::LoadXMLSettings(const pugi::xml_node& node) {
	if (std::string(node.name()) != "Terrain") {
		log_error("Terrain --> Need a \"Terrain\" node");
		return false;
	}

	if (node.attribute("width").empty()) {
		log_error("Terrain --> Need a \"width\" attribute");
		return false;
	} else
		terrainWidth = node.attribute("width").as_int();

	if (node.attribute("height").empty()) {
		log_error("Terrain --> Need a \"height\" attribute");
		return false;
	} else
		terrainHeight = node.attribute("height").as_int();

	if (node.attribute("bpp").empty()) {
		log_error("Terrain --> Need a \"bpp\" attribute");
		return false;
	} else
		bpp = node.attribute("bpp").as_int();

	if (!node.attribute("heightScale").empty())
		heightScale = node.attribute("heightScale").as_float();

	if (!node.attribute("blockScale").empty())
		blockScale = node.attribute("blockScale").as_float();

	if (!node.attribute("debug").empty())
		debug = node.attribute("debug").as_bool();

	if (node.attribute("file").empty()) {
		log_error("Terrain --> Need a \"file\" attribute");
		return false;
	} else {
		if (!LoadHeightmap(node.attribute("file").as_string(), bpp, terrainWidth, terrainHeight))
			return false;
	}

	if (node.child("Shader").empty()) {
		log_error("Terrain --> Need a \"Shader\" child node");
		return false;
	} else {
		shader = Media::MediaManager::getInstance().getShaderManager().getProgram(node.child("Shader"));
		if (shader == NULL)
			return false;
	}

	if (!node.child("Textures").empty()) {
		for (pugi::xml_node texNode : node.child("Textures").children()) {
			if (std::string(texNode.name()) == "Texture2D") {
				Media::Images::Texture* tex = Media::MediaManager::getInstance().getTextureManager().getTexture(texNode);
				if (tex != nullptr)
					textures.push_back(tex);
			}
		}
	}

	return true;
}

void Terrain::Update(float dt) {
	const float LIGHT_SPEED = 20.0f;
	static bool increasing = true;
	if (lightPosZ < 90.0f && increasing)
		lightPosZ += LIGHT_SPEED * dt;
	else if (increasing)
		increasing = false;

	if (lightPosZ > -90.0f && !increasing)
		lightPosZ -= LIGHT_SPEED * dt;
	else if (!increasing)
		increasing = true;

	//light0.SetPosition(0.0f, 15.0f, lightPosZ, 1.0f);
}

void Terrain::Render() {
	if (wireframe) {
		// Fix that, it is not working in opengl es.
#ifndef ANDROID
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	}

	Rendering::Renderer::GetInstance().GetMatrixStack().Multiple(localToWorldMatrix);

	shader->bindShader();

	shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->sendUniform3x3("normal_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX).GetNormalMatrix());

	for (short i = 0; i < textures.size(); ++i) {
		textures[i]->activate(i);
		shader->sendUniform("texture" + to_string(i), i);
	}

	//shader->SendUniform("texture0", 0);

	shader->sendUniform("material0.ambient", material0.GetAmbientColor());
	shader->sendUniform("material0.diffuse", material0.GetDiffuseColor());
	shader->sendUniform("material0.specular", material0.GetSpecularColor());
	shader->sendUniform("material0.emissive", material0.GetEmissiveColor());
	shader->sendUniform("material0.shininess", material0.GetShininess());

	shader->sendUniform("light0.ambient", light0.GetAmbient());
	shader->sendUniform("light0.diffuse", light0.GetDiffuse());
	shader->sendUniform("light0.specular", light0.GetSpecular());
	shader->sendUniform("light0.constant_attenuation", light0.GetConstantAttenuation());
	shader->sendUniform("light0.linear_attenuation", light0.GetLinearAttenuation());
	shader->sendUniform("light0.quadratic_attenuation", light0.GetQuadraticAttenuation());
	shader->sendUniform("light0.position", light0.GetPosition());
	shader->sendUniform("light0.spot_cut_off", light0.GetSpotCutOff());
	shader->sendUniform("light0.spot_direction", light0.GetSpotDirection());
	shader->sendUniform("light0.spot_exponenet", light0.GetSpotExponenet());

	glBindVertexArray(vertexArray);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->unbindShader();

	if (wireframe) {
		// Fix that, it is not working in opengl es.
#ifndef ANDROID
		glPolygonMode(GL_FRONT, previousPolygonMode[0]);
		glPolygonMode(GL_BACK, previousPolygonMode[1]);
#endif
	}
}

void Terrain::RenderDebug() {
	Render();
	RenderNormals();
}

void Terrain::GenerateDebug() {
	normalVertices.reserve(vertices.size() * 2);

	for (unsigned i = 0; i < vertices.size(); ++i) {
		normalVertices.push_back(vertices[i]);
		normalVertices.push_back(vertices[i] + normals[i]);
	}

	glGenBuffers(1, &normalVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * normalVertices.size() * 3, &normalVertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &normalVertexArray);
	glBindVertexArray(normalVertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, normalVertexBuffer);
	glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	Media::MediaManager::getInstance().getShaderManager().loadProgram("floorShader", "floorShader.xml");
	debugShader = Media::MediaManager::getInstance().getShaderManager().getProgram("floorShader");
}

void Terrain::RenderNormals() {
	debugShader->bindShader();
	debugShader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	debugShader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	debugShader->sendUniform("color", Math::vec4f(1.0f, 1.0f, 0.0f, 1.0f));

	glBindVertexArray(normalVertexArray);
	glDrawArrays(GL_LINES, 0, normalVertices.size());
	glBindVertexArray(0);

	debugShader->unbindShader();
}

void Terrain::SetWireframe(bool value) {
	wireframe = value;
	// Fix that. It is not working in OpenGL ES.
#ifndef ANDROID
	glGetIntegerv(GL_POLYGON_MODE, previousPolygonMode);
#endif
}

bool Terrain::GetWireframe() {
	return wireframe;
}

float Terrain::GetHeightAt(const NGE::Math::vec3f& position) {
	float height = -FLT_MAX;

	if (terrainWidth < 2 || terrainHeight < 2) {
		log_error("Terrain --> Terrain hasn't been loaded, or is of incorrect size");

	}

	float terrainBlockWidth = (terrainWidth - 1) * blockScale;
	float terrainBlockHeight = (terrainHeight - 1) * blockScale;
	float halfBlockWidth = terrainBlockWidth * 0.5f;
	float halfBlockHeight = terrainBlockHeight * 0.5f;

	Math::vec3f terrainPos = position;
	Math::vec3f invBlockScale(1.0f / blockScale, 0.0f, 1.0f / blockScale);

	Math::vec3f offset(halfBlockWidth, 0.0f, halfBlockHeight);

	// Pobranie 4 wierzchołków które tworzą trójkąty na podanej pozycji
	Math::vec3f vertexIndices = (terrainPos + offset);
	vertexIndices.x *= invBlockScale.x;
	vertexIndices.y *= invBlockScale.y;
	vertexIndices.z *= invBlockScale.z;

	int u0 = (int) floor(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int) floor(vertexIndices.z);
	int v1 = v0 + 1;

	if (u0 >= 0 && u1 < (int) terrainWidth && v0 >= 0 && v1 < (int) terrainHeight) {
		Math::vec3f p00 = vertices[ (v0 * terrainWidth) + u0]; // Górny lewy wierzchołek
		Math::vec3f p10 = vertices[ (v0 * terrainWidth) + u1]; // Górny prawy wierzchołek
		Math::vec3f p01 = vertices[ (v1 * terrainWidth) + u0]; // Dolny lewy wierzchołek
		Math::vec3f p11 = vertices[ (v1 * terrainWidth) + u1]; // Dolny prawy wierzchołek

		// Nad którym trójkątem znajduje się pozycja
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		Math::vec3f dU, dV;
		// Górny trójkąt
		if (percentU > percentV) {
			dU = p10 - p00;
			dV = p11 - p10;
		}			// Dolny trójkąt
		else {
			dU = p11 - p01;
			dV = p01 - p00;
		}

		Math::vec3f heightPos = p00 + (dU * percentU) + (dV * percentV);
		height = heightPos.y;
	}

	return height;
}