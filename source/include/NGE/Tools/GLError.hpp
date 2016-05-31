/* 
 * File:   GLError.hpp
 * Author: tku
 *
 * Created on 31 May 2016, 15:39
 */

#ifndef GLERROR_HPP
#define GLERROR_HPP

#include "NGE/Tools/Logger/NewLogger.hpp"

#define check_gl_error() NGE::Tools::GLError::CheckGLError(__FILE__, __LINE__, __PRETTY_FUNCTION__)

namespace NGE {
	namespace Tools {

		class GLError {
		  public:

			static void CheckGLError(const char* file, int line, const char* function) {
				GLenum err(glGetError());
				while (err != GL_NO_ERROR) {
					std::string error;
					switch (err) {
						case GL_INVALID_ENUM:
							error = "INVALID_ENUM";
							break;
						case GL_INVALID_VALUE:
							error = "INVALID_VALUE";
							break;
						case GL_INVALID_OPERATION:
							error = "INVALID_OPERATION";
							break;
						case GL_INVALID_FRAMEBUFFER_OPERATION:
							error = "INVALID_FRAMEBUFFER_OPERATION";
							break;
						case GL_OUT_OF_MEMORY:
							error = "OUT_OF_MEMORY";
							break;
						case GL_STACK_UNDERFLOW:
							error = "STACK_UNDERFLOW";
							break;
						case GL_STACK_OVERFLOW:
							error = "STACK_OVERFLOW";
							break;
					}

					Logger::NewLogger::GetInstance().WriteLog("GL error: GL_{}", Logger::LogTypes::LOG_LEVEL::ERROR, file, line, function, error);
					err = glGetError();
				}
			}
		};
	}
}

#endif /* GLERROR_HPP */

