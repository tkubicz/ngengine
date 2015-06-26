/* 
 * File:   MeshKey.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 14:49
 */

#ifndef MESHKEY_HPP
#define	MESHKEY_HPP

namespace NGE {
	namespace Appearance {
		namespace Scene {

			/**
			 * Binds anim mesh to specific point in time.
			 */
			class MeshKey {
			  public:

				MeshKey() { }

				MeshKey(double time, const unsigned int value) : time(time), value(value) { }

				bool operator==(const MeshKey& o) const {
					return o.value == this->value;
				}

				bool operator!=(const MeshKey& o) const {
					return o.value != this->value;
				}

				bool operator<(const MeshKey& o) const {
					return time < o.time;
				}

				bool operator>(const MeshKey& o) const {
					return time > o.time;
				}

			  protected:
				/**
				 * The time of this key.
				 */
				double time;

				/**
				 * Index into animMeshes array of the mesh coresponding to the
				 * meshAnim hosting this key frame.
				 */
				unsigned int value;
			};
		}
	}
}

#endif	/* MESHKEY_HPP */

