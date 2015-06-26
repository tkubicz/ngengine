/* 
 * File:   Settings.hpp
 * Author: tku
 *
 * Created on 17 luty 2014, 17:31
 */

#ifndef SETTINGS_HPP
#define	SETTINGS_HPP

/**
 * This file contains preprocessor definitons which are used in graphics engine.
 */

/**
 * When this flag is set, additional methods in various classes are available.
 * It is used to debug engine - get additional information. It should not be
 * used in productive environment.
 */
#ifndef NGE_DEBUG
#define NGE_DEBUG
#endif

/**
 * When this flag is set to true, the engine needs libjpeg library to compile.
 * libjpeg is used for JPEG and JPG image loader (JpegImage class)
 */
#ifndef NGE_LIB_JPEG
#define NGE_LIB_JPEG true
#endif

/**
 * Rendering API - it can be OpenGL, OpenGL ES or DirectX.
 */

#endif	/* SETTINGS_HPP */

