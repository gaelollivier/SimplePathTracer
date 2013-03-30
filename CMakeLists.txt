CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
CMAKE_POLICY (SET CMP0015 NEW)

PROJECT ("SimplePathTracer")

SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

FIND_PACKAGE (SFML 2 REQUIRED system window graphics audio network)

FIND_LIBRARY (
	     ASSIMP_LIBRARIES
	     assimp
	     PATHS
	     /usr/local/lib
)

SET (CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra")

INCLUDE_DIRECTORIES (
		    ${CMAKE_SOURCE_DIR}
		    /usr/local/include
)

FILE (
     GLOB_RECURSE
     SOURCES
     src/*
)

ADD_EXECUTABLE (
	       SimplePathTracer
	       ${SOURCES}
)

TARGET_LINK_LIBRARIES (
		      SimplePathTracer
		      ${SFML_LIBRARIES}
		      ${ASSIMP_LIBRARIES}
)
