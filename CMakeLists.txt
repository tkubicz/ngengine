cmake_minimum_required (VERSION 3.0)

project (nge)

# Set the current build profile.
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

# Appends the modules path to MAKE_MODULE_PATH variable.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

# Current version of the project.
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)

include(GitGetRevisionDescription)
git_execute(COMMIT_HASH rev-parse --short HEAD)

# --------------------------------------
message(STATUS "Compiling ${CMAKE_PROJECT_NAME} version ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${COMMIT_HASH}")
message(STATUS "Using cmake version ${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
# --------------------------------------

# Specific the default install path
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install/")
endif()

# Set up the third party software.
include(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ThirdpartyMakeLists.cmake)

# Specify the compilation flags for different build types.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -DUSE_GLFW -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -DUSE_GLFW -O3 -Wall")

# By default we are going to build tests
if (NOT DEFINED BUILD_TESTS)
	set (BUILD_TESTS true)
endif()

# By default we want to show source code statistics
if (NOT DEFINED SHOW_SOURCE_CODE_STATS)
	set (SHOW_SOURCE_CODE_STATS true)
endif()

if (NOT DEFINED LUA_VERSION)
	set (LUA_VERSION 5.2)
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

if (ANDROID)
	message(STATUS "Building for Android")
endif()

if (NOT ANDROID)
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

# Set Settings.hpp file.
configure_file(
		source/include/NGE/Core/Settings.hpp.in
		${PROJECT_BINARY_DIR}/include/Settings.hpp
)

include_directories (${PROJECT_BINARY_DIR}/include)
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
	source/src/NGE/Actors/Actor.cpp
	source/src/NGE/Actors/ActorComponenet.cpp

	source/src/NGE/Appearance/Light.cpp
	source/src/NGE/Appearance/Scene/SceneNode.cpp

	source/src/NGE/Core/Process.cpp
	source/src/NGE/Core/ProcessManager.cpp

    source/src/NGE/Events/EventBus.cpp
	source/src/NGE/Events/EventDelegate.cpp
	source/src/NGE/Events/EventManager.cpp
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

	source/src/NGE/Media/Images/GetTextureEvent.cpp
	source/src/NGE/Media/Images/JpegImage.cpp
	source/src/NGE/Media/Images/LoadTextureEvent.cpp
	source/src/NGE/Media/Images/PngImage.cpp
	source/src/NGE/Media/Images/TargaImage.cpp
	source/src/NGE/Media/Images/Texture.cpp
	source/src/NGE/Media/Images/TextureManager.cpp

	source/src/NGE/Media/Shaders/GLSLProgram.cpp
    source/src/NGE/Media/Shaders/GetShaderEvent.cpp
    source/src/NGE/Media/Shaders/LoadShaderEvent.cpp
	source/src/NGE/Media/Shaders/ShaderManager.cpp

	source/src/NGE/Media/MediaManager.cpp
	source/src/NGE/Media/MediaPathManager.cpp

	thirdparty/pugixml/src/pugixml.cpp

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

    source/src/NGE/Scripting/LuaScriptEvent.cpp
	source/src/NGE/Scripting/LuaScriptManager.cpp
	source/src/NGE/Scripting/LuaScriptProcess.cpp

	source/src/NGE/Tools/Logger/Output/AbstractLoggerOutput.cpp
	source/src/NGE/Tools/Logger/Output/ConsoleLoggerOutput.cpp
	source/src/NGE/Tools/Logger/Output/FileLoggerOutput.cpp
	source/src/NGE/Tools/Logger/Output/StreamLoggerOutput.cpp

	source/src/NGE/Tools/Logger/LogTypes.cpp
	source/src/NGE/Tools/Logger/NewLogger.cpp

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

include(${CMAKE_CURRENT_SOURCE_DIR}/test/TestMakeLists.cmake)