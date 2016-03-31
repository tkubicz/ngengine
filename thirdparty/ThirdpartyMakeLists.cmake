cmake_minimum_required (VERSION 3.0)

# Set the directory of third-party software.
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/kaguya/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pugixml/src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Catch/include/")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cppformat/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cppformat" thirdparty/cppformat)
