/* 
 * File:   MD5Parser.hpp
 * Author: tku
 *
 * Created on 20 październik 2014, 15:28
 */

#ifndef MD5PARSER_HPP
#define	MD5PARSER_HPP

#include "NGE/Geometry/Face.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Quaternion.hpp"
#include "NGE/Parsers/ParsingUtils.hpp"
#include <vector>
#include <string>

namespace NGE {
	namespace Geometry {
		namespace Models {
			namespace MD5 {

				/**
				 * Represents a single element in a MD5 file.
				 * Elements are always contained in sections.
				 */
				class Element {
				  protected:
					/**
					 * Points to the starting point of the element.
					 * Whitespace at the beginning and at the end have been removed.
					 * Elements are terminated with \0.
					 */
					char* start;

					/**
					 * Original line number (can be used in error messages, if
					 * a parsing error occurs).
					 */
					unsigned int lineNumber;

				  public:

					unsigned int GetLineNumber() const {
						return lineNumber;
					}

					void SetLineNumber(unsigned int lineNumber) {
						this->lineNumber = lineNumber;
					}

					char* GetStart() const {
						return start;
					}

					void SetStart(char* start) {
						this->start = start;
					}

				};

				typedef std::vector<Element> ElementList;

				/**
				 * Represents a section of the MD5 file (such as the mesh or the joints section).
				 * A secion is always enclosed in { and } brackets.
				 */
				class Section {
				  protected:
					/**
					 * Original line number (can be used in error messages if 
					 * a parsing error occurs).
					 */
					unsigned int lineNumber;

					/**
					 * List of all elements which have been parsed in this section.
					 */
					ElementList elements;

					/**
					 * Name of the section.
					 */
					std::string name;

					/**
					 * For global elements: the value of the element as string.
					 * If !length() the section is not a global element.
					 */
					std::string globalValue;

				  public:

					ElementList GetElements() const {
						return elements;
					}

					void SetElements(ElementList elements) {
						this->elements = elements;
					}

					std::string GetGlobalValue() const {
						return globalValue;
					}

					void SetGlobalValue(std::string globalValue) {
						this->globalValue = globalValue;
					}

					unsigned int GetLineNumber() const {
						return lineNumber;
					}

					void SetLineNumber(unsigned int lineNumber) {
						this->lineNumber = lineNumber;
					}

					std::string GetName() const {
						return name;
					}

					void SetName(std::string name) {
						this->name = name;
					}

				};

				typedef std::vector<Section> SectionList;

				/**
				 * Basic information about a joint.
				 */
				class BaseJointDescription {
				  protected:
					/**
					 * Name of the bone.
					 */
					std::string name;

					/**
					 * Parent index of the bone.
					 */
					int parentIndex;

				  public:

					std::string GetName() const {
						return name;
					}

					void SetName(std::string name) {
						this->name = name;
					}

					int GetParentIndex() const {
						return parentIndex;
					}

					void SetParentIndex(int parentIndex) {
						this->parentIndex = parentIndex;
					}

				};

				/**
				 * Represents a bone (joint) descriptor in a MD5Mesh file.
				 */
				class BoneDesc : public BaseJointDescription {
				  protected:
					/**
					 * Absolute position of the bone.
					 */
					Math::vec3f positionXYZ;

					/**
					 * Absolute rotation of the bone.
					 */
					Math::vec3f rotationQuat;
					Math::quaternionf rotationQuatConverted;

					/**
					 * Absolute transformation of the bone (temporary).
					 */
					Math::mat4f transform;

					/**
					 * Inverse transformation of the bone (temporary).
					 */
					Math::mat4f invTransform;

					/**
					 * Internal.
					 */
					unsigned int map;

				  public:

					Math::mat4f GetInvTransform() const {
						return invTransform;
					}

					void SetInvTransform(Math::mat4f invTransform) {
						this->invTransform = invTransform;
					}

					Math::vec3f GetPositionXYZ() const {
						return positionXYZ;
					}

					void SetPositionXYZ(Math::vec3f positionXYZ) {
						this->positionXYZ = positionXYZ;
					}

					Math::vec3f GetRotationQuat() const {
						return rotationQuat;
					}

					void SetRotationQuat(Math::vec3f rotationQuat) {
						this->rotationQuat = rotationQuat;
					}

					Math::quaternionf GetRotationQuatConverted() const {
						return rotationQuatConverted;
					}

					void SetRotationQuatConverted(Math::quaternionf rotationQuatConverted) {
						this->rotationQuatConverted = rotationQuatConverted;
					}

					Math::mat4f GetTransform() const {
						return transform;
					}

					void SetTransform(Math::mat4f transform) {
						this->transform = transform;
					}
				};

				typedef std::vector<BoneDesc> BoneList;

				class AnimBoneDesc : public BaseJointDescription {
				  protected:
					/**
					 * Flags?
					 */
					unsigned int flags;

					/**
					 * Index of the first key that corresponds to this anim bone.
					 */
					unsigned int firstKeyIndex;

				  public:

					unsigned int GetFirstKeyIndex() const {
						return firstKeyIndex;
					}

					void SetFirstKeyIndex(unsigned int firstKeyIndex) {
						this->firstKeyIndex = firstKeyIndex;
					}

					unsigned int GetFlags() const {
						return flags;
					}

					void SetFlags(unsigned int flags) {
						this->flags = flags;
					}
				};

				typedef std::vector<AnimBoneDesc> AnimBoneList;

				/**
				 * Represents a frame descriptor in a MD5Anim file.
				 */
				class BaseFrameDesc {
				  protected:
					Math::vec3f positionXYZ;
					Math::vec3f roationQuat;

				  public:

					Math::vec3f GetPositionXYZ() const {
						return positionXYZ;
					}

					void SetPositionXYZ(Math::vec3f positionXYZ) {
						this->positionXYZ = positionXYZ;
					}

					Math::vec3f GetRoationQuat() const {
						return roationQuat;
					}

					void SetRoationQuat(Math::vec3f roationQuat) {
						this->roationQuat = roationQuat;
					}

				};

				typedef std::vector<BaseFrameDesc> BaseFrameList;

				/**
				 * Represents a base frame descriptor in a MD5Anim file.
				 */
				class CameraAnimFrameDesc : BaseFrameDesc {
				  protected:
					float fov;

				  public:

					float GetFov() const {
						return fov;
					}

					void SetFov(float fov) {
						this->fov = fov;
					}

				};

				typedef std::vector<CameraAnimFrameDesc> CameraFrameList;

				/**
				 * Represents a frame descriptor in a MD5Anim file.
				 */
				class FrameDesc {
				  protected:
					/**
					 * Index of the frame.
					 */
					unsigned int index;

					/**
					 * Animation keyframes - a large blob of data at first.
					 */
					std::vector<float> values;

				  public:

					unsigned int GetIndex() const {
						return index;
					}

					void SetIndex(unsigned int index) {
						this->index = index;
					}

					std::vector<float> GetValues() const {
						return values;
					}

					void SetValues(std::vector<float> values) {
						this->values = values;
					}
				};

				typedef std::vector<FrameDesc> FrameList;

				/**
				 * Represents a vertex descriptor in a MD5 file.
				 */
				class VertexDesc {
				  protected:
					/**
					 * UV coordinate of the vertex.
					 */
					Math::vec2f uv;

					/**
					 * Index of the first weight of the vertex in the vertex
					 * weight list.
					 */
					unsigned int firstWeight;

					/**
					 * Number of weights assigned to this vertex.
					 */
					unsigned int numWeights;

				  public:

					VertexDesc() : firstWeight(0), numWeights(0) { }

					unsigned int GetFirstWeight() const {
						return firstWeight;
					}

					void SetFirstWeight(unsigned int firstWeight) {
						this->firstWeight = firstWeight;
					}

					unsigned int GetNumWeights() const {
						return numWeights;
					}

					void SetNumWeights(unsigned int numWeights) {
						this->numWeights = numWeights;
					}

					Math::vec2f GetUv() const {
						return uv;
					}

					void SetUv(Math::vec2f uv) {
						this->uv = uv;
					}
				};

				typedef std::vector<VertexDesc> VertexList;

				/**
				 * Represents a vertex weight descriptor in a MD5 file.
				 */
				class WeightDesc {
				  protected:
					/**
					 * Index of the bone to which this weight refers.
					 */
					unsigned int bone;

					/**
					 * The weight value.
					 */
					float weight;

					/**
					 * The offset position of this weight
					 * (in the coordinate system defined by the parent bone).
					 */
					Math::vec3f offsetPosition;

				  public:

					unsigned int GetBone() const {
						return bone;
					}

					void SetBone(unsigned int bone) {
						this->bone = bone;
					}

					Math::vec3f GetOffsetPosition() const {
						return offsetPosition;
					}

					void SetOffsetPosition(const Math::vec3f& offsetPosition) {
						this->offsetPosition = offsetPosition;
					}

					float GetWeight() const {
						return weight;
					}

					void SetWeight(float weight) {
						this->weight = weight;
					}
				};

				typedef std::vector<WeightDesc> WeightList;
				typedef std::vector<Geometry::Face> FaceList;

				/**
				 * Represents a mesh in a MD5 file.
				 */
				class MeshDesc {
				  protected:
					/**
					 * Weights of the mesh.
					 */
					WeightList weights;

					/**
					 * Vertices of the mesh.
					 */
					VertexList vertices;

					/**
					 * Faces of the mesh.
					 */
					FaceList faces;

					/**
					 * Name of the shader (or texture) to be assigned to the mesh.
					 */
					std::string shader;

				  public:

					FaceList GetFaces() const {
						return faces;
					}

					void SetFaces(FaceList faces) {
						this->faces = faces;
					}

					std::string GetShader() const {
						return shader;
					}

					void SetShader(std::string shader) {
						this->shader = shader;
					}

					VertexList GetVertices() const {
						return vertices;
					}

					void SetVertices(VertexList vertices) {
						this->vertices = vertices;
					}

					WeightList GetWeights() const {
						return weights;
					}

					void SetWeights(WeightList weights) {
						this->weights = weights;
					}
				};

				typedef std::vector<MeshDesc> MeshList;

				/**
				 * Convert a quaternion to its usual representation.
				 * @param in
				 * @param out
				 */
				inline void ConvertQuaternion(const Math::vec3f& in, Math::quaternionf& out) {
					out.i = in.x;
					out.j = in.y;
					out.k = in.z;

					const float t = 1.0f - (in.x * in.x) - (in.y * in.y) - (in.z * in.z);
					if (t < 0.0f)
						out.r = 0.0f;
					else
						out.r = sqrt(t);
				}

				/**
				 * Parse the data sections of a MD5 mesh file.
				 */
				class MD5MeshParser {
				  public:
					/**
					 * Constructs a new MD5MeshParser instance from an existing
					 * preparsed list of file sections.
					 * @param sections List of file sections (output of MD5Parser).
					 */
					MD5MeshParser(SectionList& sections);

				  protected:
					/**
					 * List of all meshes.
					 */
					MeshList meshses;

					/**
					 * List of all joints.
					 */
					BoneList joints;
				};

				/**
				 * Parse the data sections of a MD5 animation file.
				 */
				class MD5AnimParser {
				  public:
					/**
					 * Constructs a new MD5AnimParser instance from an existing
					 * preparsed list of file sections.
					 * @param sections List of file sections (output of MD5Parser).
					 */
					MD5AnimParser(SectionList& sections);

				  protected:
					/**
					 * Output frame rate.
					 */
					float frameRate;

					/**
					 * List of animation bones.
					 */
					AnimBoneList animatedBones;

					/**
					 * List of base frames.
					 */
					BaseFrameList baseFrames;

					/**
					 * List of animation frames.
					 */
					FrameList frames;

					/**
					 * Number of animated componenets.
					 */
					unsigned int numAnimatedComponenets;
				};

				/**
				 * Parse the data sections of a MD5 camera animation file.
				 */
				class MD5CameraParser {
				  public:
					/**
					 * Constructs a new MD5CameraParser instance from an existing
					 * preparsed list of file section.
					 * @param sections List of file sections (output of MD5Parser).
					 */
					MD5CameraParser(SectionList& sections);

				  protected:
					/**
					 * Output frame rate.
					 */
					float frameRate;

					/**
					 * List of cuts.
					 */
					std::vector<unsigned int> cuts;

					/**
					 * Frames.
					 */
					CameraFrameList frames;
				};

				/**
				 * Parses the block structure of MD5MESH and MD5ANIM files (but does
				 * no further processing).
				 */
				class MD5Parser {
				  public:
					/**
					 * Constructs a new MD5Parser instance from an existing buffer.
					 * @param buffer File buffer.
					 * @param fileSize Length of the file in bytes (excluding a terminal 0).
					 */
					MD5Parser(char* buffer, unsigned int fileSize);

					/**
					 * Reports a specific error message and throw an exception.
					 * @param error Error message to be reported.
					 * @param line Index of the line where the error occured.
					 */
					static void ReportError(const char* error, unsigned int line);

					/**
					 * Report a specific warning.
					 * @param warn Warn message to be reported.
					 * @param line Index of the line where the error occured.
					 */
					static void ReportWarning(const char* warn, unsigned int line);

					void ReportError(const char* error) {
						return ReportError(error, lineNumber);
					}

					void ReportWarning(const char* warn) {
						return ReportWarning(warn, lineNumber);
					}

				  protected:
					/**
					 * List of all sections which have been read.
					 */
					SectionList sections;

				  private:
					/**
					 * Parse a file section. The current file pointer must be outside
					 * of a section.
					 * @param out Receives the section data.
					 * @return True if the end of the file has been reached.
					 */
					bool ParseSection(Section& out);

					/**
					 * Parse the file header.
					 */
					void ParseHeader();

					/**
					 * Override these functions to make sure the line counter gets incremented.
					 */
					bool SkipLine(const char* in, const char** out) {
						++lineNumber;
						//return ::SkipLine(in, out);
						return Parsers::ParsingUtils::SkipLine(in, out);
					}

					bool SkipLine() {
						return SkipLine(buffer, (const char**) & buffer);
					}

					bool SkipSpacesAndLineEnd(const char* in, const char** out) {
						bool had = false;
						bool running = true;
						while (running) {
							if (*in == '\r' || *in == '\n') {
								// We open files in binary mode, so there could be \r\n sequences.
								if (!had) {
									had = true;
									++lineNumber;
								}
							} else if (*in == '\t' || *in == ' ')
								had = false;
							else
								break;
							in++;
						}
						*out = in;
						return *in != '\0';
					}

					bool SkipSpacesAndLineEnd() {
						return SkipSpacesAndLineEnd(buffer, (const char**) &buffer);
					}

					bool SkipSpaces();

					char* buffer;
					unsigned int fileSize;
					unsigned int lineNumber;
				};
			}
		}
	}
}

#endif	/* MD5PARSER_H */

