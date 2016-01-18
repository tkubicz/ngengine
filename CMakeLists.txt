cmake_minimum_required (VERSION 2.8)

project (nge)

# Set the current build profile
set (CMAKE_BUILD_TYPE Debug)

# --------------------------------------
message(STATUS "Compiling ${CMAKE_PROJECT_NAME} version 1.0")
message(STATUS "Using cmake version ${CMAKE_VERSION}")
# --------------------------------------

# Specific the default install path
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install/")
endif()

# Set the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

# Set the directory of third-party software.
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/kaguya/include/")

# By default we are going to build tests
if (NOT DEFINED BUILD_TESTS)
	set (BUILD_TESTS true)
endif()

if (NOT DEFINED LUA_VERSION)
	set (LUA_VERSION 5.2)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	if (NOT ANDROID)
    	add_definitions(-g -std=c++11 -Wall -DUSE_GLFW)
    else()
    	add_definitions(-g -std=c++11)	
    	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${my_cxx_flags}" )
      	SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${my_cxx_flags}" )
    endif()
endif()

IF (ANDROID)
	find_package(OpenGLES)
	if (OPENGLES_FOUND)
		message(STATUS "OpenGL ES Found")
	else()
		message(STATUS "OpenGL ES Not found")
	endif()
endif()

option(NO_OPENGL "Disable OpenGL support" OFF)
option(NO_GLFW "Disable GLFW support" OFF)
option(NO_GLEW "Disable GLEW support" OFF)

#set (ENV{BOOST_ROOT} "/home/tku/Pobrane/Boost-for-Android/boost_1_53_0")

if (ANDROID)
	message(STATUS "Building for Android")
	find_host_package(Boost 1.53.0 REQUIRED)
	if (Boost_FOUND)
		#include_directories (${Boost_INCLUDE_DIRS})
		MESSAGE(STATUS "Boost include:" ${Boost_INCLUDE_DIRS})
		include_directories("/home/tku/Pobrane/Boost-for-Android/build/include/boost-1_53")
	endif()
endif()

if (NOT ANDROID)
# Find boost
	find_package (Boost 1.53.0 REQUIRED COMPONENTS unit_test_framework)
	if (Boost_FOUND)
		include_directories(${Boost_INCLUDE_DIRS})
		message(STATUS "Boost include dir:" ${Boost_INCLUDE_DIRS})
	endif()

	# Find OpenGL
	find_package (OpenGL REQUIRED)

	# Find GLFW
	find_package (GLFW REQUIRED)

	# Find GLEW
	find_package (GLEW REQUIRED)
endif()

# Any System
# Find zlib
find_package (ZLIB REQUIRED)

# Find lua
find_package (Lua ${LUA_VERSION} EXACT REQUIRED)
if (LUA_FOUND)
	message(STATUS "Lua include dir:" ${LUA_INCLUDE_DIR})
	include_directories(${LUA_INCLUDE_DIR})
endif()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/source/include/)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/source/src/NGE/)

if (ANDROID)
	set (EXTRA_LIBS ${EXTRA_LIBS} log)
	set (EXTRA_LIBS ${EXTRA_LIBS} android)
	set (EXTRA_LIBS ${EXTRA_LIBS} GLESv3)
	set (EXTRA_LIBS ${EXTRA_LIBS} GLESv2)
	set (EXTRA_LIBS ${EXTRA_LIBS} EGL)
	set (EXTRA_LIBS ${EXTRA_LIBS} z)
endif()

add_library(nge SHARED
	source/src/NGE/Appearance/Light.cpp
	source/src/NGE/Appearance/Scene/SceneNode.cpp

	source/src/NGE/Core/Process.cpp
	source/src/NGE/Core/ProcessManager.cpp

	source/src/NGE/Events/EventDelegate.cpp
	source/src/NGE/Events/EventManager.cpp
	source/src/NGE/Events/IEventManager.cpp
	source/src/NGE/Events/KeyboardEvent.cpp
	source/src/NGE/Events/MouseEvent.cpp
    source/src/NGE/Events/TouchEvent.cpp

	source/src/NGE/Geometry/Basic/Box.cpp
	source/src/NGE/Geometry/Basic/Floor.cpp
	source/src/NGE/Geometry/Basic/Plane.cpp
	source/src/NGE/Geometry/Basic/Sphere.cpp

	#source/src/NGE/Geometry/Models/MD5/MD5Loader.cpp
	#source/src/NGE/Geometry/Models/MD5/MD5Parser.cpp

	source/src/NGE/Geometry/Models/Obj/ObjFileImporter.cpp
	source/src/NGE/Geometry/Models/Obj/ObjFileMtlImporter.cpp
	source/src/NGE/Geometry/Models/Obj/ObjFileParser.cpp

	source/src/NGE/Geometry/Models/ColladaModel.cpp
	source/src/NGE/Geometry/Models/XModel.cpp

	source/src/NGE/Geometry/Nature/SkyColor.cpp
	source/src/NGE/Geometry/Nature/SkyDome.cpp
	source/src/NGE/Geometry/Nature/SkyPlane.cpp
	source/src/NGE/Geometry/Nature/DiamondSquareTerrain.cpp
	source/src/NGE/Geometry/Nature/Terrain.cpp
	source/src/NGE/Geometry/Nature/WaterTexture.cpp

	source/src/NGE/Geometry/Skeleton/Skeleton.cpp
	source/src/NGE/Geometry/Skeleton/SkeletonNode.cpp
	source/src/NGE/Geometry/Skeleton/SkinnedMesh.cpp

	source/src/NGE/Geometry/Face.cpp
	source/src/NGE/Geometry/Mesh.cpp
	source/src/NGE/Geometry/Ray.cpp

	source/src/NGE/GUI/GUIAlphaElement.cpp
	source/src/NGE/GUI/GUIButton.cpp
	source/src/NGE/GUI/GUIClippedRectangle.cpp
	source/src/NGE/GUI/GUIEvent.cpp
	source/src/NGE/GUI/GUIFrame.cpp
	source/src/NGE/GUI/GUILabel.cpp
	source/src/NGE/GUI/GUIPanel.cpp
	source/src/NGE/GUI/GUIRectangle.cpp
	source/src/NGE/GUI/GUISeparator.cpp
	source/src/NGE/GUI/GUITexCoordDescriptor.cpp
	source/src/NGE/GUI/GUIText.cpp
	source/src/NGE/GUI/GUITextBox.cpp

	source/src/NGE/Input/SoftwareMouse.cpp

	source/src/NGE/Math/MathUtils.cpp
	source/src/NGE/Math/NumberConverter.cpp

    source/src/NGE/Media/Files/File.cpp

	source/src/NGE/Media/Fonts/FontManager.cpp
	source/src/NGE/Media/Fonts/FreeTypeFont.cpp
	source/src/NGE/Media/Fonts/TextureMappedFont.cpp

	source/src/NGE/Media/Images/JpegImage.cpp
	source/src/NGE/Media/Images/PngImage.cpp
	source/src/NGE/Media/Images/TargaImage.cpp
	source/src/NGE/Media/Images/Texture.cpp
	source/src/NGE/Media/Images/TextureManager.cpp

	source/src/NGE/Media/Shaders/GLSLProgram.cpp
	source/src/NGE/Media/Shaders/ShaderManager.cpp

	source/src/NGE/Media/MediaManager.cpp
	source/src/NGE/Media/MediaPathManager.cpp

	source/src/NGE/Parsers/pugixml.cpp

	source/src/NGE/Physics/MassAggregate/Particle.cpp
	source/src/NGE/Physics/MassAggregate/ParticleContacts.cpp
	source/src/NGE/Physics/MassAggregate/ParticleForceGenerator.cpp
	source/src/NGE/Physics/MassAggregate/ParticleLinks.cpp
	source/src/NGE/Physics/MassAggregate/ParticleWorld.cpp

	source/src/NGE/Physics/RigidBody/BoundingSphere.cpp
	source/src/NGE/Physics/RigidBody/CollisionDetector.cpp
	source/src/NGE/Physics/RigidBody/CollisionPrimitive.cpp
	source/src/NGE/Physics/RigidBody/Contact.cpp
	source/src/NGE/Physics/RigidBody/ContactResolver.cpp
	source/src/NGE/Physics/RigidBody/ForceGenerator.cpp
	source/src/NGE/Physics/RigidBody/IntersectionTests.cpp
	source/src/NGE/Physics/RigidBody/Joint.cpp
	source/src/NGE/Physics/RigidBody/RigidBody.cpp
	source/src/NGE/Physics/RigidBody/World.cpp

	source/src/NGE/Rendering/Camera/Camera.cpp
	source/src/NGE/Rendering/Renderer.cpp

	source/src/NGE/Scripting/LuaScriptManager.cpp

	source/src/NGE/Tools/Logger.cpp
	source/src/NGE/Tools/Random.cpp
	source/src/NGE/Tools/Timing.cpp

	source/src/NGE/Windows/Application.cpp
	source/src/NGE/Windows/D3DWindow.cpp
	source/src/NGE/Windows/GLFW3Window.cpp
    source/src/NGE/Windows/AndroidWindow.cpp
)

if (ANDROID)
	target_link_libraries(nge ${EXTRA_LIBS})
endif()

install (TARGETS nge DESTINATION bin)
install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source/include/ DESTINATION include)

include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)