/* 
 * File:   Core.hpp
 * Author: tku
 *
 * Created on 3 luty 2015, 15:24
 */

#ifndef CORE_HPP
#define	CORE_HPP

#ifdef ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "NGE/ThirdPart/android_native_app_glue.h"
#else 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

/** 
 * Include Logger - it's accessable from any part of the code.
 */
#include "NGE/Tools/Logger.hpp"

#endif	/* CORE_HPP */