#ifdef ANDROID
#include <GLES3/gl3.h>
#else 
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "NGE/Rendering/Renderer.hpp"

using namespace NGE::Rendering;

Renderer::Renderer() { }

Renderer::Renderer(const Renderer&) { }

Renderer::~Renderer() { }

MatrixStack<float>& Renderer::GetMatrixStack() {
	return stack;
}

void Renderer::GetRendererInformation() {
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	nge_log_info("Renderer --> OpenGL Information:\n\t GL Vendor: " + nge_to_string(vendor) +
			"\n\t GL Renderer: " + nge_to_string(renderer) +
			"\n\t GL Version (string): " + nge_to_string(version) +
			"\n\t GL Version (integer): " + nge_to_string(major) + "." + nge_to_string(minor) +
			"\n\t GLSL Version: " + nge_to_string(glslVersion));
}

NGE::Math::Vector3<float> Renderer::GetScreenCoords(const NGE::Math::Vector3<float>& worldPosition) {
	Math::Vector4<float> temp;
	int viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	temp.Set(worldPosition, 1.0f);
	temp *= stack.GetMatrix(MODELVIEW_MATRIX);
	temp *= stack.GetMatrix(PROJECTION_MATRIX);

	temp /= temp.w;

	temp.x = viewport[0] + float(viewport[2]) * (temp.x + 1.0f) / 2.0f;
	temp.y = viewport[1] + float(viewport[3]) * (temp.y + 1.0f) / 2.0f;
	temp.z = 0.5f * temp.z + 0.5f;
	return Math::Vector3<float>(temp.x, temp.y, temp.z);
}

NGE::Math::Vector3<float> Renderer::GetWorldCoords(const NGE::Math::Vector3<float>& windowPosition) {
	Math::Vector4<float> temp(windowPosition, 1.0f);
	int viewport[4];
	Math::Matrix4<float> spareMatrix;

	glGetIntegerv(GL_VIEWPORT, viewport);

	spareMatrix = stack.GetMatrix(PROJECTION_MATRIX);
	spareMatrix *= stack.GetMatrix(MODELVIEW_MATRIX);
	spareMatrix.SetInverse();

	// Odwrócenie współrzędnych Y tak by 0 było w lewym dolnym rogu

	temp.x = (windowPosition.x - viewport[0]) / viewport[2] * 2.0f - 1.0f;
	temp.y = (viewport[3] - windowPosition.y - viewport[1]) / viewport[3] * 2.0f - 1.0f;
	temp.z = windowPosition.z * 2.0f - 1.0f;

	temp *= spareMatrix;
	return (!temp.w ? Math::Vector3<float>() : Math::Vector3<float>(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w));
}

NGE::Geometry::Ray<float> Renderer::GetCursorRay(const NGE::Math::vec2i& mousePosition) {
	Math::Vector3<float> nearSource((float) mousePosition.x, (float) mousePosition.y, 0.0f);
	Math::Vector3<float> farSource((float) mousePosition.x, (float) mousePosition.y, 1.0f);

	Math::Vector3<float> nearPoint = GetWorldCoords(nearSource);
	Math::Vector3<float> farPoint = GetWorldCoords(farSource);

	Math::Vector3<float> direction = farPoint - nearPoint;
	direction.Normalize();

	return Geometry::Ray<float>(direction, nearPoint);
}

void Renderer::Enter2DMode() {
	int viewport[4];
	int width, height;

	glGetIntegerv(GL_VIEWPORT, viewport);
	width = viewport[2];
	height = viewport[3];

	stack.SetMatrixMode(PROJECTION_MATRIX);
	stack.PushMatrix();
	stack.Identity();

	Math::Matrix4<float> ortho;
	ortho.SetOrthographicProjection(0.f, (float) width, 0.f, (float) height, -1.f, 1.f);
	stack.Multiple(ortho);

	stack.SetMatrixMode(MODELVIEW_MATRIX);
	stack.PushMatrix();
	stack.Identity();

	glDisable(GL_DEPTH_TEST);
}

void Renderer::Exit2DMode() {
	stack.PopMatrix();
	stack.SetMatrixMode(PROJECTION_MATRIX);
	stack.PopMatrix();
	stack.SetMatrixMode(MODELVIEW_MATRIX);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::ClearBuffers(unsigned int buffers) {
	glClear(buffers);
}

void Renderer::ClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer::ClearColor(const Math::Vector4<float>& color) {
	glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::SetDimensions(const NGE::Math::vec2i& dimensions) {
	Renderer::dimensions = dimensions;
}

NGE::Math::vec2i& Renderer::GetDimensions() {
	return dimensions;
}