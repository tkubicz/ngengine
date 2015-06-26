/* 
 * File:   Mesh.hpp
 * Author: tku
 *
 * Created on 6 maj 2013, 13:40
 */

#ifndef MESH_HPP
#define	MESH_HPP

#include <vector>

#include "NGE/Core/Core.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Geometry/Bone.hpp"
#include "NGE/Geometry/Face.hpp"
#include "NGE/Appearance/Material.hpp"

namespace NGE {
	namespace Geometry {

		class Mesh {
		  public:

			/**
			 * Enumarates the types of geometric primitives.
			 */
			enum PrimitiveType {
				/**
				 * A point primitive.
				 * This is just a single vertex in the virtual world.
				 */
				POINT = 0x1,

				/**
				 * A line primitive.
				 * This is a line defined through a start and an end position.
				 */
				LINE = 0x2,

				/**
				 * A triangular primitive.
				 * A triangle consists of three indices.
				 */
				TRIANGLE = 0x4,

				/**
				 * A higher-level polygon with more than 3 edges.
				 * A triangle is a polygon, but polygon in this context means
				 * "all polygons that are not triangles". The "Triangulate-Step"
				 * is provided for your convenience, it splits all polygons in
				 * trinagles (which are much easier to handle).
				 */
				POLYGON = 0x8
			};

		  protected:
			std::vector<Math::vec3f> vertices;
			std::vector<Math::vec2f> texCoords;
			std::vector<Math::vec3f> normals;
			std::vector<Math::vec3f> tangents;
			std::vector<Math::vec3f> bitangents;
			std::vector<Math::vec4f> colors;
			std::vector<GLuint> indices;

			std::vector<Bone> bones;
			std::vector<Appearance::Material> materials;
			std::vector<Face> faces;

			GLuint vertexArray;
			GLuint vertexBuffer, texCoordBuffer, normalBuffer, tangentBuffer, bitangentBuffer, colorBuffer, indexBuffer;

			unsigned int numFaces;

		  protected:
			virtual void DeleteBuffers();
			virtual void DeleteArrays();

		  public:
			Mesh();
			virtual ~Mesh();

			GLuint& GetVertexArray();
			GLuint& GetVertexBuffer();
			GLuint& GetTexCoordBuffer();
			GLuint& GetNormalBuffer();
			GLuint& GetTangentBuffer();
			GLuint& GetBitangentBuffer();
			GLuint& GetColorBuffer();
			GLuint& GetIndexBuffer();

			std::vector<Math::vec3f>& GetVertices();
			std::vector<Math::vec2f>& GetTexCoords();
			std::vector<Math::vec3f>& GetNormals();
			std::vector<Math::vec3f>& GetTangents();
			std::vector<Math::vec3f>& GetBitangents();
			std::vector<Math::vec4f>& GetColors();
			std::vector<GLuint>& GetIndices();

			std::vector<Face> GetFaces() const;

			unsigned int GetNumFaces() const;
			void SetNumFaces(unsigned int numFaces);

		};
	}
}

#endif	/* MESH_HPP */

