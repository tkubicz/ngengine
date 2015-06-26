/* 
 * File:   Terrain.hpp
 * Author: tku
 *
 * Created on 11 kwiecień 2013, 23:05
 */

#ifndef TERRAIN_HPP
#define	TERRAIN_HPP

#include <vector>
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Matrix4.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Appearance/Light.hpp"
#include "NGE/Appearance/Material.hpp"

namespace NGE {
	namespace Geometry {
		namespace Nature {

			class Terrain {
			  public:
				Terrain(float heightScale = 20.0f, float blockScale = 1.0f);

				void SetHeightScale(float heightScale);
				void SetBlockScale(float blockScale);

				bool LoadHeightmap(const std::string& rawFile, int bitsPerPixel, int width, int height);
				bool LoadXMLSettings(const pugi::xml_node& node);

				void Update(float dt);
				void Render();
				void RenderDebug();

				void SetShader(Media::Shaders::GLSLProgram* shader);
				void SetDebugShader(Media::Shaders::GLSLProgram* shader);

				void SetWireframe(bool value);
				bool GetWireframe();

				/**
				 * Pobranie wysokości terenu
				 * @param position Pozycja terenu
				 * @return Wysokość w podanym punkcie
				 */
				float GetHeightAt(const Math::vec3f& position);

			  private:
				inline float GetHeightValue(const unsigned char* data, unsigned char numBytes);
				inline float GetPercentage(float value, const float min, const float max);

				void GenerateVertices(const vector<float> heights);
				void GenerateIndices();
				void GenerateTexCoords();
				void GenerateNormals();

				void GenerateDebug();
				void RenderNormals();

				GLuint vertexArray, vertexBuffer, indexBuffer, colorBuffer, texCoordBuffer, normalBuffer;

				// VAO, VBO i wierzchołki sluzace do renderowania normalnych w celach testowych
				GLuint normalVertexArray, normalVertexBuffer;
				std::vector<Math::vec3f> normalVertices;

				std::vector<Math::vec3f> vertices;
				std::vector<GLuint> indices;
				std::vector<Math::vec4f> colors;
				std::vector<Math::vec2f> texCoords;
				std::vector<Math::vec3f> normals;

				Media::Shaders::GLSLProgram* shader;
				Media::Shaders::GLSLProgram* debugShader;

				//Media::Images::Texture* texture;
				std::vector<Media::Images::Texture*> textures;

				Appearance::Light light0;
				Appearance::Material material0;
				float lightPosZ;

				float heightScale, blockScale;
				bool debug, wireframe;
				int previousPolygonMode[2];
				int terrainWidth, terrainHeight, bpp;

				Math::mat4f localToWorldMatrix;
			};
		}
	}
}

#endif	/* TERRAIN_HPP */

