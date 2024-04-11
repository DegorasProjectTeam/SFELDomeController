# **********************************************************************************************************************
#   SFELDomeController: Controller application for SFEL station dome.
#
#   Copyright (C) 2023 Degoras Project Team
#                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
#                      < Jesús Relinque Madroñal >
#
#   This file is part of SFELDomeController.
#
#   Licensed under the European Union Public License (EUPL), Version 1.2 or subsequent versions of the EUPL license
#   as soon they will be approved by the European Commission (IDABC).
#
#   This project is free software: you can redistribute it and/or modify it under the terms of the EUPL license as
#   published by the IDABC, either Version 1.2 or, at your option, any later version.
#
#   This project is distributed in the hope that it will be useful. Unless required by applicable law or agreed to in
#   writing, it is distributed on an "AS IS" basis, WITHOUT ANY WARRANTY OR CONDITIONS OF ANY KIND; without even the
#   implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the EUPL license to check specific
#   language governing permissions and limitations and more details.
#
#   You should use this project in compliance with the EUPL license. You should have received a copy of the license
#   along with this project. If not, see the license at < https://eupl.eu/ >.
# **********************************************************************************************************************

# **********************************************************************************************************************
# Updated 27/09/2023
# **********************************************************************************************************************

# Cmake minimum.
cmake_minimum_required(VERSION 3.21)

# Check version.
if (NOT ${CMAKE_VERSION} VERSION_LESS "3.1.0")
    cmake_policy(SET CMP0054 NEW)
endif()

#Configure the build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Library names.
set(FULL_NAME SFELDomeController)
set(APP_NAME ${FULL_NAME})

# Project name.
project(${APP_NAME})

# Check the architecture.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Architecture not supported by default.")
endif()

# Set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
include(CMakeMacros)

# Set the version.
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

# Output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Other variables setting.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(SHOW_EXTERNALS TRUE)

# Define the install directory for each compiler and operating system.
macro_prepare_install_dir(INSTALL_BIN INSTALL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../product)

# Includes.
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${ZMQ_DIR}/includes)


# Get the headers, sources and template files.
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/*.h)
file(GLOB_RECURSE TEMPLTS ${CMAKE_SOURCE_DIR}/*.tpp)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)


# Get the include aliases files.
macro_get_files_without_extension(ALIAS ${CMAKE_SOURCE_DIR}/includes/${FULL_NAME}/*)

# Create a custom group for the templates, aliases and externals.
source_group("Alias Files" FILES ${ALIAS})
source_group("Header Files" FILES ${HEADERS})
source_group("External Libs" FILES ${EXTERN})

# Add definitions and the library.
string(TOUPPER ${FULL_NAME} FULL_NAME_UPPER)

add_executable(${APP_NAME} ${SOURCES})


# Configure the compilers.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -O3 -fopenmp")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${FULL_NAME} PRIVATE /std:c++17 /openmp)
else()
    message(FATAL_ERROR "Compiler not supported by default.")
endif()

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${FULL_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# Link libraries. For win32 only.

# Instalation common
# Install the include files of LibZMQUtils.
install(DIRECTORY ${CMAKE_SOURCE_DIR}/includes/${FULL_NAME}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/includes
        PATTERN "*.txt" EXCLUDE)

# Install the include files of ZMQ.
install(DIRECTORY ${ZMQ_DIR}/includes/zmq
        DESTINATION ${CMAKE_INSTALL_PREFIX}/includes)

# Install the binaries  of LibZMQUtils to the specified install directory.
install(TARGETS ${FULL_NAME}
        LIBRARY DESTINATION ${INSTALL_LIB}
        RUNTIME DESTINATION ${INSTALL_LIB})


# Runtime artifacts.
install(IMPORTED_RUNTIME_ARTIFACTS ${FULL_NAME}
        RUNTIME_DEPENDENCY_SET ${LIB_DEPS_SET}
        DESTINATION ${INSTALL_BIN})


# Installation process for windows.
if(WIN32)

    # Install the binaries of ZMQ to the specified install directory.
    install(DIRECTORY ${ZMQ_LIBS_DIR}/
            DESTINATION ${INSTALL_LIB}
            FILES_MATCHING PATTERN "*.dll")

    # Install the lib files for msvc.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        # ZMQ libs
        install(DIRECTORY ${ZMQ_LIBS_DIR}/
                DESTINATION ${INSTALL_LIB}
                FILES_MATCHING PATTERN "*.lib")

        # Install the LibZMQUtils.lib file to the specified library directory.
        install(FILES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${FULL_NAME}.lib
                DESTINATION ${INSTALL_LIB})

    endif()

    # Install the external dependencies.
    install(RUNTIME_DEPENDENCY_SET ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            DIRECTORIES ${ZMQ_LIBS_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            DESTINATION ${INSTALL_BIN})

elseif(UNIX)
    # Install the libraries of ZMQ to the specified install directory.
    install(DIRECTORY ${ZMQ_LIBS_DIR}/
            DESTINATION ${INSTALL_LIB}
            FILES_MATCHING PATTERN "*.so")


    install(DIRECTORY ${ZMQ_LIBS_DIR}/
            DESTINATION ${INSTALL_LIB}
            FILES_MATCHING PATTERN "*.a")

endif()

# Enable testing.
enable_testing()

# **********************************************************************************************************************
