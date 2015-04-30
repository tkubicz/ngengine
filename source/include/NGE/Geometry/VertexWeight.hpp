/* 
 * File:   VertexWeight.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 13:59
 */

#ifndef VERTEXWEIGHT_HPP
#define	VERTEXWEIGHT_HPP

namespace NGE
{
    namespace Geometry
    {

        /**
         * A single influence of a bone on a vertex.
         */
        class VertexWeight
        {
          public:

            /**
             * Default constructor.
             */
            VertexWeight() { }

            /**
             * Constructor with parameters.
             * @param vertexId Identifier of the vertex.
             * @param weight Weight factor.
             */
            VertexWeight(unsigned int vertexId, float weight) : vertexId(vertexId), weight(weight) { }

          protected:
            /**
             * Index of the vertex which is influenced by the bone.
             */
            unsigned int vertexId;
            
            /**
             * The strength of the influence in the range (0..1).
             * The influence from all bones at one vertex amounts to 1.
             */
            float weight;
        };
    }
}

#endif	/* VERTEXWEIGHT_HPP */

