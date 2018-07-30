CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

#[PROJECT_NAME]
PROJECT(DCGameObject)

#[BUILD_TYPE]
SET(CMAKE_BUILD_TYPE Debug)

# Set the output folders where the program will be created
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
SET(CMAKE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/../cmake_include)
SET(EXTERNALS_PATH ${PROJECT_SOURCE_DIR}/../externals)

INCLUDE(${CMAKE_INCLUDE_PATH}/CMake-Include.txt)

#[PRJ_INCLUDE]
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(include/components)
INCLUDE_DIRECTORIES(include/help)
INCLUDE_DIRECTORIES(include/types)
INCLUDE_DIRECTORIES(include/managers)

#[PRJ_HEADER_FILES]
SET(HEADERS
	include/components/component.h
	include/components/gameobject.h
	include/components/scene.h
	include/components/transform.h
	include/help/deletehelp.h
	include/help/floathelp.h
	include/help/vectorhelp.h
	include/types/rtti.h
	include/managers/gameobjectmanager.h
)

#[PRJ_SOURCE_FILES]
SET(SOURCES
	src/components/gameobject.cpp
	src/components/scene.cpp
	src/components/transform.cpp
	src/managers/gameobjectmanager.cpp
)

# Generate the static library from the sources
ADD_LIBRARY(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

#[ORGANIZATION]

SOURCE_GROUP_BY_FOLDER("${SOURCES}")
SOURCE_GROUP_BY_FOLDER("${HEADERS}")

#[EXTERNAL_PROJECTS]
ADD_SUBDIRECTORY(${EXTERNALS_PATH}/dcpp_math/project ${PROJECT_BINARY_DIR}/dcpp_math)
INCLUDE_DIRECTORIES(${EXTERNALS_PATH}/dcpp_math/project/include)


SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
 
# Set the location for library installation
# Use "sudo make install" to apply
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIBRARY_OUTPUT_PATH})
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION ${LIBRARY_OUTPUT_PATH})
