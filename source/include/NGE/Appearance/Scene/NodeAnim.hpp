/* 
 * File:   NodeAnim.hpp
 * Author: tku
 *
 * Created on 10 marzec 2014, 14:34
 */

#ifndef NODEANIM_HPP
#define	NODEANIM_HPP

#include "NGE/Math/Types/Vector3Key.hpp"
#include "NGE/Math/Types/QuaternionKey.hpp"

namespace NGE {
	namespace Appearance {
		namespace Scene {

			/**
			 * Describes the animation of a single node. The name specifies the
			 * bone/node which is affected by this animation channel. The keyframes
			 * are given in three seperate series of values, one each for position,
			 * rotation and scaling. The transformation matrix computed from these
			 * values replaces the node's original transformation matrix at specific
			 * time.
			 * This means all keys are absolute and not relative to the bone default pose.
			 * The order in which the transofmrations are applied is - as usual - scaling,
			 * rotation, translation.
			 * 
			 * @note All keys are returned in their correct, chronological order.
			 * Duplicate keys don't pass the validation step. Most likely there
			 * will be no negative time values, but they are not forbidden also (so
			 * implementations need to cope with them).
			 */
			class NodeAnim {
			  public:

				NodeAnim() {
					numPositionKeys = numRotationKeys = numScalingKeys = 0;
					positionKeys = NULL;
					rotationKeys = NULL;
					scalingKeys = NULL;

					// TODO: Animation behaviour
				}

				virtual ~NodeAnim() {
					delete[] positionKeys;
					delete[] rotationKeys;
					delete[] scalingKeys;
				}

				std::string GetNodeName() const {
					return nodeName;
				}

				void SetNodeName(const std::string& nodeName) {
					this->nodeName = nodeName;
				}

				unsigned int GetNumPositionKeys() const {
					return numPositionKeys;
				}

				void SetNumPositionKeys(unsigned int numPositionKeys) {
					this->numPositionKeys = numPositionKeys;
				}

				Math::Types::Vector3Key<float>* GetPositionKeys() {
					return positionKeys;
				}

				void SetPositionKeys(Math::Types::Vector3Key<float>* positionKeys) {
					this->positionKeys = positionKeys;
				}

				unsigned int GetNumRotationKeys() const {
					return numRotationKeys;
				}

				void SetNumRotationKeys(unsigned int numRotationKeys) {
					this->numRotationKeys = numRotationKeys;
				}

				Math::Types::QuaternionKey<float>* GetRotationKeys() {
					return rotationKeys;
				}

				void SetRotationKeys(Math::Types::QuaternionKey<float>* rotationKeys) {
					this->rotationKeys = rotationKeys;
				}

				unsigned int GetNumScalingKeys() const {
					return numScalingKeys;
				}

				void SetNumScalingKeys(unsigned int numScalingKeys) {
					this->numScalingKeys = numScalingKeys;
				}

				Math::Types::Vector3Key<float>* GetScalingKeys() {
					return scalingKeys;
				}

				void SetScalingKeys(Math::Types::Vector3Key<float>* scalingKeys) {
					this->scalingKeys = scalingKeys;
				}

			  protected:
				/**
				 * The name of the node affected by this animation. The node
				 * must exist and it must be unique.
				 */
				std::string nodeName;

				/**
				 * The number of position keys.
				 */
				unsigned int numPositionKeys;

				/**
				 * The position keys of this animation channel. Positions are
				 * specified as 3D vector. The array is numPositionKeys in size.
				 * If there are position keys, there will also be at least one
				 * scaling and one rotation key.
				 */
				Math::Types::Vector3Key<float>* positionKeys;

				/**
				 * The number of rotation keys.
				 */
				unsigned int numRotationKeys;

				/**
				 * The rotation keys of this animation channel. Rotations are
				 * given as quaterions, which are 4D vectors. The array is 
				 * numRotationKeys in size.
				 * If there are rotation keys, there will also be at least one
				 * scaling and one position key.
				 */
				Math::Types::QuaternionKey<float>* rotationKeys;

				/**
				 * The number of scaling keys.
				 */
				unsigned int numScalingKeys;

				/**
				 * The scaling keys of this animation channel. Scalings are specified
				 * as 3D vector. The array is numScalingKeys in size.
				 * If there are scaling keys, there will also be at least one
				 * position and one rotation key.
				 */
				Math::Types::Vector3Key<float>* scalingKeys;

				// TODO: Animation behaviour?
			};
		}
	}
}

#endif	/* NODEANIM_HPP */

