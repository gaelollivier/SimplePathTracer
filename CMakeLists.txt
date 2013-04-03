###############################################################################
# SimplePathTracer
###############################################################################

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
CMAKE_POLICY (SET CMP0015 NEW)

PROJECT ("SimplePathTracer")

###############################################################################
# Project variables
###############################################################################

SET (
    SOURCES_DIR
    src/
)

SET (
    BIN_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
)

SET (
    LIB_DIRECTORY
    ${CMAKE_SOURCE_DIR}/lib
)


###############################################################################
# Local CMake modules
###############################################################################

SET (
    CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMakeModules
    ${CMAKE_MODULE_PATH}
)

###############################################################################
# Dependencies
###############################################################################

FIND_PACKAGE (SFML 2 REQUIRED system window graphics audio network)
FIND_PACKAGE (PythonLibs)
FIND_PACKAGE (Boost COMPONENTS system python thread)

FIND_LIBRARY (
    ASSIMP_LIBRARIES
    assimp
    PATHS
    /usr/local/lib
)

###############################################################################
# Compiler options
###############################################################################

SET (CMAKE_CXX_FLAGS "-std=c++0x -Wall")

INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}
    ${SOURCES_DIR}    
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
)

###############################################################################
# SimplePathTracer library
###############################################################################

FILE (
    GLOB_RECURSE
    SOURCES
    ${SOURCES_DIR}/*
)

ADD_LIBRARY (
    SimplePathTracer
    SHARED
    ${SOURCES}
)

TARGET_LINK_LIBRARIES (
    SimplePathTracer
    ${ASSIMP_LIBRARIES}
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
)

SET_PROPERTY (
    TARGET SimplePathTracer
    PROPERTY LIBRARY_OUTPUT_DIRECTORY ${LIB_DIRECTORY}
)

SET_PROPERTY (
    TARGET SimplePathTracer
    PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_DIRECTORY}
)

SET_PROPERTY (
    TARGET SimplePathTracer
    PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_DIRECTORY}
)

###############################################################################
# SimplePathTracer python module
###############################################################################

FILE (
    GLOB_RECURSE
    PYTHON_MODULE_SOURCES
    PythonModule/*
)

ADD_LIBRARY (
    PySpt
    SHARED
    ${PYTHON_MODULE_SOURCES}
)

TARGET_LINK_LIBRARIES (
    PySpt
    SimplePathTracer
    ${PYTHON_LIBRARIES}
)

SET_PROPERTY (
    TARGET PySpt
    PROPERTY LIBRARY_OUTPUT_DIRECTORY ${LIB_DIRECTORY}
)

SET_PROPERTY (
    TARGET PySpt
    PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_DIRECTORY}
)

SET_PROPERTY (
    TARGET PySpt
    PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_DIRECTORY}
)

# Create a symbolic link of the module with a name proper to import in python
ADD_CUSTOM_COMMAND(TARGET PySpt POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${LIB_DIRECTORY}/libPySpt.dylib ${LIB_DIRECTORY}/PySpt.so)

###############################################################################
# Examples
###############################################################################

ADD_EXECUTABLE (
    SfmlDemo
    examples/SfmlDemo.cpp
)

TARGET_LINK_LIBRARIES (
    SfmlDemo
    SimplePathTracer
    ${SFML_LIBRARIES}
)

SET_PROPERTY (
    TARGET SfmlDemo
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

SET_PROPERTY (
    TARGET SfmlDemo
    PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIRECTORY}
)

SET_PROPERTY (
    TARGET SfmlDemo
    PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIRECTORY}
)