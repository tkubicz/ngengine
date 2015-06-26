/* 
 * File:   AABox.hpp
 * Author: tku
 *
 * Created on 16 luty 2014, 22:55
 */

#ifndef AABOX_HPP
#define	AABOX_HPP

#include "NGE/Math/Vector3.hpp"

namespace NGE {
	namespace Math {
		namespace Objects3D {

			/**
			 * Class that represents box aligned with the axes of the cooridnate system.
			 */
			template <typename T> class AABox {
			  protected:
				/**
				 * Point of origin of the box.
				 */
				Math::Vector3<T> origin;

				/**
				 * Half size of the box on every axes.
				 */
				Math::Vector3<T> halfSize;

			  public:

				/**
				 * Default empty constructor.
				 */
				AABox();

				/**
				 * Constructor that creates box from provided parameters.
				 * @param origin Point of origin of the box.
				 * @param halfSize Size of the box.
				 */
				AABox(const Math::Vector3<T>& origin, const Math::Vector3<T>& halfSize);

				/**
				 * Change box properties.
				 * @param origin Point of origin of the box.
				 * @param halfSize Size of the box.
				 */
				void SetBox(const Math::Vector3<T>& origin, const Math::Vector3<T>& halfSize);

				/**
				 * Get the vertex that is further along the normal's direction.
				 * It is useful to testing if the AAB is inside or outside the
				 * frustum.
				 * @param normal The normal that we are testing against.
				 * @return The positive vertex.
				 */
				Math::Vector3<T> GetPositiveVertex(const Math::Vector3<T>& normal) const;

				/**
				 * Get the opposite to positive vertex of the AAB.
				 * @param normal The normal that we are testing against.
				 * @return The negative vertex.
				 */
				Math::Vector3<T> GetNegativeVertex(const Math::Vector3<T>& normal) const;
			};

#include "NGE/Math/Objects3D/AABox.inc"

			typedef AABox<float> aaboxf;
			typedef AABox<double> aaboxd;
		}
	}
}

#endif	/* AABOX_HPP */

