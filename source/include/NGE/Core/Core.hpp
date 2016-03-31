/* 
 * File:   Core.hpp
 * Author: tku
 *
 * Created on 3 February 2015, 15:24
 */

#ifndef CORE_HPP
#define	CORE_HPP

//#define ANDROID

#ifdef ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "NGE/ThirdPart/android_native_app_glue.h"
#else 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif	/* CORE_HPP */