/* 
 * File:   DiamonSquareTerrain.hpp
 * Author: tku
 *
 * Created on 6 luty 2015, 16:33
 */

#ifndef DIAMONSQUARETERRAIN_HPP
#define	DIAMONSQUARETERRAIN_HPP

#include <vector>
#include "NGE/Tools/Random.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Nature
        {

            /**
             * This class is an implementation of diamond-square algorithm
             * used to generate fairly realistic terrain using fractals.
             * It is also known as random midpoint displacement fractal,
             * the cloud fractal or the plasma fractal.
             * The algorithm starts with a 2D grid then randomly generates
             * terrain height from four seed values arranged in a grid points
             * so that the entier plane is covered in squares.
             * 
             * You can read more about it here: http://en.wikipedia.org/wiki/Diamond-square_algorithm
             */
            class DiamonSquareTerrain
            {
              public:

                /**
                 * Default constructor. It takes one parameter used to calculate
                 * the size of the terrain using the equation 2^detail + 1.
                 * @param detail Parameter used to calculate the size of the terrain.
                 */
                DiamonSquareTerrain(unsigned int detail = 10);

                /**
                 * Generate the terrain.
                 * @param roughness @see roughness
                 */
                void Generate(const float roughness);

                std::vector<float>& GetHeights()
                {
                    return heights;
                }

                void Render();

                GLuint vertexBuffer, indexBuffer, vba;
                unsigned int indicesSize;
                Media::Shaders::GLSLProgram* shader;
                std::vector<Math::vec3f> vertices;
                std::vector<Math::vec4f> colors;
                std::vector<unsigned int> indices;

                void GenerateVertices();
                void GenerateIndices();
                void GenerateVBA();
                void DestroyGeometry();

              protected:

                /**
                 * Helper function to get values from one dimensional array
                 * using two dimensional parameters x and y.
                 * @param x First coordinate.
                 * @param y Second coordinate.
                 * @return Value at the provided coordinates or -1 if the
                 * coordinates were wrong.
                 */
                float GetHeight(const int x, const int y) const;

                /**
                 * Helper function to set value into one dimensional array
                 * using two dimensional coordinates.
                 * @param x First coordinate.
                 * @param y Second coordinate.
                 * @param value Value to be set at provided coordinates.
                 */
                void SetHeight(const int x, const  int y, const float value);

                /**
                 * Helper function to calculate the average from the input
                 * array. First it filters the wrong values (-1) and later
                 * it calculates the average.
                 * @param input The input array.
                 * @return Average of values from input array.
                 */
                float Average(const std::vector<float>& input);

                /**
                 * Function for square phase. It averages four corner points 
                 * before applying a random offset.
                 * @param x X coordinate in heights array.
                 * @param y Y coordinate in heights array.
                 * @param size Size of the square.
                 * @param offset Random offset to apply.
                 */
                void Square(const int x, const int y, const int size, const double offset);

                /**
                 * Function for diamond phase. It averages four edge points before
                 * applying a random offset.
                 * @param x X coordinate in heights array.
                 * @param y Y coordinate in heights array.
                 * @param size Size of the diamond.
                 * @param offset Random offset to apply.
                 */
                void Diamond(const int x, const int y, const int size, const double offset);

                /**
                 * Function to recursively divide the height map. At each division
                 * it splits the map into squares and update their center point
                 * during the square phase. Then it splits the map into diamonds
                 * and update their center points during the diamond phase.
                 * @param size Size of the map to divide.
                 */
                void Divide(const int size);
                
                /**
                 * Generate the random number for terrain offset.
                 * @return 
                 */
                double Random();

              protected:
                /**
                 * The size of the terrain is a result of equation
                 * 2^detail + 1, where detail is a parameter provided in
                 * constructor.
                 */
                unsigned int size;

                /**
                 * Maximum size of the terrain is size - 1.
                 */
                unsigned int max;

                /**
                 * Variable which determines wheter the terrain is smooth
                 * (values near zero) our mountainous (values near one).
                 */
                float roughness;
                
                /**
                 * Random number generator.
                 */
                Tools::Random* random;

                /**
                 * Array that holds information about generated heights.
                 * In constructor it is resized to size * size.
                 */
                std::vector<float> heights;
            };
        }
    }
}

#endif	/* DIAMONSQUARETERRAIN_HPP */

