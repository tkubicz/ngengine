#include "NGE/Geometry/Nature/WaterTexture.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Nature;

WaterTexture::WaterTexture() {
	//NUMBER_WAVES = 4;
	//TEXTURE_SIZE = 1024;
	passedTime = 0;
}

unsigned int WaterTexture::Initialize(float waterPlaneLength, NGE::Media::Shaders::GLSLProgram* shader) {
	this->shader = shader;

	vertices.reserve(4);
	vertices.push_back(Math::vec3f(-1.0f, -1.0f, 0.0f));
	vertices.push_back(Math::vec3f(1.0f, -1.0f, 0.0f));
	vertices.push_back(Math::vec3f(-1.0f, 1.0f, 0.0f));
	vertices.push_back(Math::vec3f(1.0f, 1.0f, 0.0f));

	for (unsigned i = 0; i < vertices.size(); ++i) {
		vertices[i].x *= 64; // TEXTURE_SIZE / 2
		vertices[i].y *= 64; // TEXTURE_SIZE / 2
	}

	/*normals.reserve(4);
	normals.push_back(Math::vec3f(0.0f, 0.0f, 1.0f));
	normals.push_back(Math::vec3f(0.0f, 0.0f, 1.0f));
	normals.push_back(Math::vec3f(0.0f, 0.0f, 1.0f));
	normals.push_back(Math::vec3f(0.0f, 0.0f, 1.0f));*/

	texCoords.reserve(4);
	texCoords.push_back(Math::vec2f(0.0f, 0.0f));
	texCoords.push_back(Math::vec2f(1.0f, 0.0f));
	texCoords.push_back(Math::vec2f(0.0f, 1.0f));
	texCoords.push_back(Math::vec2f(1.0f, 1.0f));

	indices.reserve(6);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	glGenBuffers(1, &vertexBuffer);
	//glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &texCoordBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * normals.size(), &normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 2 * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);

	glGenTextures(1, &mirrorTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mirrorTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024 /* texture size */, 1024 /* texture size */, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &depthMirrorTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, depthMirrorTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &waterTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, waterTexture);

	// Dodanie bufora koloru
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);

	// Dodanie bufora głębokości
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthMirrorTexture);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Tools::Logger::WriteErrorLog("WaterTexture --> GL_FRAMEBUFFER_COMPLETE error 0x" + to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader->BindShader();

	Math::mat4f lookAt;
	Math::mat4f modelviewMatrix = lookAt.LookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	Math::mat4f projectionMatrix;
	projectionMatrix.SetOrthographicProjection(-TEXTURE_SIZE / 2, TEXTURE_SIZE / 2, -TEXTURE_SIZE / 2, TEXTURE_SIZE / 2, 1.0f, 100.0f);

	shader->SendUniform4x4("u_modelviewMatrix", modelviewMatrix);
	shader->SendUniform4x4("u_projectionMatrix", projectionMatrix);
	shader->SendUniform("u_waterPlaneLength", waterPlaneLength);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// TODO: Fix that!
#ifndef ANDROID
	glClearDepth(1.0);
#else
	glClearDepthf(1.0);
#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return mirrorTexture;
}

void WaterTexture::DeleteBuffers() {
	if (vertexBuffer != 0)
		glDeleteBuffers(1, &vertexBuffer);

	if (texCoordBuffer != 0)
		glDeleteBuffers(1, &texCoordBuffer);

	if (indexBuffer != 0)
		glDeleteBuffers(1, &normalBuffer);
}

void WaterTexture::DeleteArrays() {
	if (vertexArray != 0)
		glDeleteVertexArrays(1, &vertexArray);
}

void WaterTexture::Update(float dt) {
	passedTime = dt;
}

void WaterTexture::Render() {
	if (shader == NULL)
		return;

	WaveParameters waveParameters[NUMBER_WAVES];
	WaveDirections waveDirections[NUMBER_WAVES];
	float overallSteepness = 0.2f;

	memset(waveParameters, 0, sizeof (waveParameters));
	memset(waveDirections, 0, sizeof (waveDirections));

	// Wave one
	waveParameters[0].speed = 0.05f;
	waveParameters[0].amplitude = 0.02f;
	waveParameters[0].wavelength = 0.3f;
	waveParameters[0].steepness = overallSteepness / (waveParameters[0].wavelength * waveParameters[0].amplitude * float(NUMBER_WAVES));
	waveDirections[0].x = 1.0f;
	waveDirections[0].z = 1.5f;

	// Wave two
	waveParameters[1].speed = 0.1f;
	waveParameters[1].amplitude = 0.01f;
	waveParameters[1].wavelength = 0.4f;
	waveParameters[1].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * float(NUMBER_WAVES));
	waveDirections[1].x = 0.8f;
	waveDirections[1].z = 0.2f;

	// Wave three
	waveParameters[2].speed = 0.04f;
	waveParameters[2].amplitude = 0.035f;
	waveParameters[2].wavelength = 0.1f;
	waveParameters[2].steepness = overallSteepness / (waveParameters[2].wavelength * waveParameters[2].amplitude * float(NUMBER_WAVES));
	waveDirections[2].x = -0.2f;
	waveDirections[2].z = -0.1f;

	// Wave four
	waveParameters[3].speed = 0.05f;
	waveParameters[3].amplitude = 0.007f;
	waveParameters[3].wavelength = 0.2f;
	waveParameters[3].steepness = overallSteepness / (waveParameters[3].wavelength * waveParameters[3].amplitude * float(NUMBER_WAVES));
	waveDirections[3].x = -0.4f;
	waveDirections[3].z = -0.3f;

	glViewport(0, 0, (GLsizei) TEXTURE_SIZE, (GLsizei) TEXTURE_SIZE);

	glBindFramebuffer(GL_FRAMEBUFFER, waterTexture);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->BindShader();

	shader->SendUniform("u_passedTime", passedTime);
	shader->SendUniformArray4("u_waveParameters", 4 * NUMBER_WAVES, (GLfloat*) waveParameters);
	shader->SendUniformArray2("u_waveDirections", 2 * NUMBER_WAVES, (GLfloat*) waveDirections);

	glFrontFace(GL_CCW);

	glBindVertexArray(vertexArray);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, parentWidth, parentHeight);

	shader->UnbindShader();
}

void WaterTexture::Reshape(int width, int height) {
	parentHeight = height;
	parentWidth = width;
}