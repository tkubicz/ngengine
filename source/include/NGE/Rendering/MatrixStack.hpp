#ifndef MATRIXSTACK_HPP
#define	MATRIXSTACK_HPP

#include <stack>

#include "NGE/Math/Matrix4.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Tools/Logger.hpp"

namespace NGE {
	// This enum shoud be in "Rendering" namespace, but it can't due C++ limitation.
	// After C++0x it should be moved inside namespace.

	enum MatrixMode {
		MODELVIEW_MATRIX = 0, PROJECTION_MATRIX, TEXTURE_MATRIX
	};

	namespace Rendering {

		template <typename T>
		class MatrixStack {
		  protected:
			typedef Math::Matrix4<T> mat;
			std::stack<mat>* currentStack;
			std::stack<mat> modelviewStack;
			std::stack<mat> projectionStack;
			std::stack<mat> textureStack;

		  public:

			void Initialize();
			void Deinitialize();
			//void ClearAll();

			void SetMatrixMode(MatrixMode mode);
			void PushMatrix();
			void PopMatrix();
			void LoadMatrix(const Math::Matrix4<T>& mat);
			Math::Matrix4<T> GetMatrix(MatrixMode mode);

			void Identity();
			void Multiple(const Math::Matrix4<T>& mat);

			void Translate(T x, T y, T z);
			void Translate(const Math::Vector3<T>& vec);

			void Rotate(T angle, T x, T y, T z);
			void Rotate(T angle, const Math::Vector3<T>& vec);

			void Scale(T x, T y, T z);
			void Scale(const Math::Vector3<T>& vec);
		};

		template <typename T> void MatrixStack<T>::Initialize() {
			Math::Matrix4<T> mat;

			if (modelviewStack.empty())
				modelviewStack.push(mat);

			if (projectionStack.empty())
				projectionStack.push(mat);

			if (textureStack.empty())
				textureStack.push(mat);

			// Set current stack to modelview
			currentStack = &modelviewStack;
		}

		template <typename T> void MatrixStack<T>::Deinitialize() {
			// Clear modelview stack
			while (!modelviewStack.empty())
				modelviewStack.pop();

			// Clear projection stack
			while (!projectionStack.empty())
				projectionStack.pop();

			// Clear texture stack
			while (!textureStack.empty())
				textureStack.pop();

			currentStack = NULL;
		}

		template <typename T> void MatrixStack<T>::SetMatrixMode(MatrixMode mode) {
			switch (mode) {
				case MODELVIEW_MATRIX:
					currentStack = &modelviewStack;
					break;

				case PROJECTION_MATRIX:
					currentStack = &projectionStack;
					break;

				case TEXTURE_MATRIX:
					currentStack = &textureStack;
					break;

				default:
					log_error("Invalid MatrixMode parameter");
					break;
			}
		}

		template <typename T> void MatrixStack<T>::PushMatrix() {
			if (!currentStack)
				Initialize();

			Math::Matrix4<T> mat = currentStack->top();
			currentStack->push(mat);
		}

		template <typename T> void MatrixStack<T>::PopMatrix() {
			if (!currentStack->empty())
				currentStack->pop();
		}

		template <typename T> void MatrixStack<T>::LoadMatrix(const Math::Matrix4<T>& mat) {
			if (!currentStack->empty())
				currentStack->top() = mat;
		}

		template <typename T> Math::Matrix4<T> MatrixStack<T>::GetMatrix(MatrixMode mode) {
			switch (mode) {
				case MODELVIEW_MATRIX:
					return modelviewStack.top();

				case PROJECTION_MATRIX:
					return projectionStack.top();

				case TEXTURE_MATRIX:
					return textureStack.top();

				default:
					log_error("Invalid MatrixMode parameter");
					break;
			}

			return Math::Matrix4<T>();
		}

		template <typename T> void MatrixStack<T>::Identity() {
			if (!currentStack->empty())
				currentStack->top().SetIdentity();
		}

		template <typename T> void MatrixStack<T>::Multiple(const Math::Matrix4<T>& mat) {
			if (!currentStack->empty())
				currentStack->top() *= mat;
		}

		template <typename T> void MatrixStack<T>::Translate(T x, T y, T z) {
			if (!currentStack->empty()) {
				Math::Matrix4<T> translation;
				translation.Translate(x, y, z);

				currentStack->top() *= translation;
			}
		}

		template <typename T> void MatrixStack<T>::Translate(const Math::Vector3<T>& vec) {
			Translate(vec.x, vec.y, vec.z);
		}

		template <typename T> void MatrixStack<T>::Rotate(T angle, T x, T y, T z) {
			if (!currentStack->empty()) {
				Math::Matrix4<T> rotation;
				rotation.RotateAxis(Math::MathUtils::DegToRad(angle), Math::Vector3<T > (x, y, z));
				currentStack->top() *= rotation;
			}
		}

		template <typename T> void MatrixStack<T>::Rotate(T angle, const Math::Vector3<T>& vec) {
			Rotate(angle, vec.x, vec.y, vec.z);
		}

		template <typename T> void MatrixStack<T>::Scale(T x, T y, T z) {
			if (!currentStack->empty()) {
				Math::Matrix4<T> scale;
				scale.Scale(x, y, z);
				currentStack->top() *= scale;
			}
		}

		template <typename T> void MatrixStack<T>::Scale(const Math::Vector3<T>& vec) {
			Scale(vec.x, vec.y, vec.z);
		}
	}
}

#endif
