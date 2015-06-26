/* 
 * File:   Face.hpp
 * Author: tku
 *
 * Created on 15 październik 2014, 17:03
 */

#ifndef FACE_HPP
#define	FACE_HPP

namespace NGE {
	namespace Geometry {

		/**
		 * A single face in a mesh, reffering to multiple vertices.
		 * 
		 * If numIndices is 3, we call the face a 'triangle', for numIndices > 3
		 * is is called 'plygon'.
		 */
		class Face {
		  protected:
			unsigned int numIndices;
			unsigned int* indices;

		  public:

			/**
			 * Default constructor.
			 */
			Face();

			/**
			 * Copy constructor.
			 * Copy the array of indices.
			 * @param copy 
			 */
			Face(const Face& copy);

			/**
			 * Destructor.
			 */
			virtual ~Face();

			/**
			 * Assignment operator. Copy the index array.
			 * @param o
			 * @return 
			 */
			const Face& operator=(const Face& o);

			/**
			 * Comparsion operator. Checks whether the index array of two
			 * faces is identical.
			 * @param o
			 * @return 
			 */
			bool operator==(const Face& o) const;

			/**
			 * Inverse comparsion operator. Checks wheter the index array
			 * of two faces is not identical.
			 * @param o
			 * @return 
			 */
			bool operator!=(const Face& o) const;

			/**
			 * Get the array of indices.
			 * @return The array of indices associated with the face.
			 */
			unsigned int* GetIndices() const;

			/**
			 * Set the array of indices. Remember to update numIndices,
			 * when setting the new array.
			 * @param indices The new array of indices.
			 */
			void SetIndices(unsigned int* indices);

			/**
			 * Get the current number of indices.
			 * @return Size of the indices array.
			 */
			unsigned int GetNumIndices() const;

			/**
			 * Set the numIndices.
			 * @param numIndices New size of the indices array.
			 */
			void SetNumIndices(unsigned int numIndices);
		};
	}
}

#endif	/* FACE_HPP */

