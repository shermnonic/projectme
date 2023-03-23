cmake_minimum_required(VERSION 3.1)

project(projectme
    VERSION 0.1
    DESCRIPTION "projectme by 386dx25"
    LANGUAGES CXX)
    
# toylib submodule
set(BUILD_GLTOYS_APPS false CACHE BOOL "BUILD_GLTOYS_APPS")
set(BUILD_TOYLIB_TEST_APPS false CACHE BOOL "BUILD_TOYLIB_TEST_APPS")
add_subdirectory("external/toylib/source/")
include_directories("external/toylib/source/") # FIXME: Provide clean include path in gltoys project

# Optional audio support via un4seen BASS library
set(PROJECTME_USE_BASS "FALSE" CACHE BOOL "Enable audio support via BASS library." )
if(PROJECTME_USE_BASS )
	message( STATUS "BASS enabled." )
    set(BASS_INCLUDE_PATH "3rdparty/bass" CACHE PATH "BASS include path")
    if (WIN32)
        set(BASS_LIBRARY "${BASS_INCLUDE_PATH}/win64/bass.lib"     CACHE FILEPATH "BASS library")
    else() # assume Linux
        set(BASS_LIBRARY "${BASS_INCLUDE_PATH}/linux64/libbass.so" CACHE FILEPATH "BASS library")
    endif()	
    message(STATUS "BASS_LIBRARY=${BASS_LIBRARY}")
    include_directories(${BASS_INCLUDE_PATH})
else()
	add_definitions(-DPROJECTME_BASS_DISABLED)
endif()

# Settings
set(CMAKE_CXX_STANDARD 17)
# Qt5 specific: Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# Qt5 specific:  As moc files are generated in the binary dir, tell CMake to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
find_package(Boost REQUIRED) # only header-only dependencies yet that don't have a component: property-tree
find_package(Qt5 COMPONENTS Widgets OpenGL CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories( 
    ${Boost_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
)

add_subdirectory("source/editor")
