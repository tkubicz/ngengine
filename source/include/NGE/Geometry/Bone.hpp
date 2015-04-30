/* 
 * File:   Bone.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 14:04
 */

#ifndef BONE_HPP
#define	BONE_HPP

#include "NGE/Geometry/VertexWeight.hpp"
#include "NGE/Math/Matrix4.hpp"

namespace NGE
{
    namespace Geometry
    {

        /**
         * A single bone of a mesh.
         * A bone has a name by which it can be found in the frame hierarchy and
         * by which it can be addressed by animations. In addition it has a number
         * of influences on vertices.
         */
        class Bone
        {
          public:

            /**
             * Default constructor.
             */
            Bone()
            {
                numWeights = 0;
                weights = NULL;
            }

            /**
             * Copy constructor
             * @param other Other bone which parameters will be copied.
             */
            Bone(const Bone& other)
            {
                numWeights = other.numWeights;
                offsetMatrix = other.offsetMatrix;
                name = other.name;

                if (other.weights && other.numWeights)
                {
                    weights = new VertexWeight[numWeights];
                    memcpy(weights, other.weights, numWeights * sizeof (VertexWeight));
                }
            }

            /**
             * Destructor.
             * Deletes the array of vertex weights.
             */
            virtual ~Bone()
            {
                delete[] weights;
            }

            std::string GetName() const
            {
                return name;
            }

            void SetName(const std::string& name)
            {
                this->name = name;
            }

            unsigned int GetNumWeights() const
            {
                return numWeights;
            }

            void SetNumWeights(unsigned int numWeights)
            {
                this->numWeights = numWeights;
            }

            VertexWeight* GetWeights() const
            {
                return weights;
            }

            void SetWeights(VertexWeight* weights)
            {
                this->weights = weights;
            }

            Math::mat4f GetOffsetMatrix() const
            {
                return offsetMatrix;
            }

            void SetOffsetMatrix(const Math::mat4f& offsetMatrix)
            {
                this->offsetMatrix = offsetMatrix;
            }

          protected:
            /**
             * The name of the bone.
             */
            std::string name;

            /**
             * The number of vertices affected by this bone.
             */
            unsigned int numWeights;

            /**
             * The vertices affected by this bone.
             */
            VertexWeight* weights;

            /**
             * Matrix that transforms from mesh space to bone spae in bind pose.
             */
            Math::mat4f offsetMatrix;
        };
    }
}

#endif	/* BONE_HPP */

