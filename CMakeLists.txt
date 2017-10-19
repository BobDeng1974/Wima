#	***** BEGIN LICENSE BLOCK *****
#
#	Copyright 2017 Project LFyre
#
#	Licensed under the Apache License, Version 2.0 (the "Apache License")
#	with the following modification; you may not use this file except in
#	compliance with the Apache License and the following modification to it:
#	Section 6. Trademarks. is deleted and replaced with:
#
#	6. Trademarks. This License does not grant permission to use the trade
#		names, trademarks, service marks, or product names of the Licensor
#		and its affiliates, except as required to comply with Section 4(c) of
#		the License and to reproduce the content of the NOTICE file.
#
#	You may obtain a copy of the Apache License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the Apache License with the above modification is
#	distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#	KIND, either express or implied. See the Apache License for the specific
#	language governing permissions and limitations under the Apache License.
#
#	****** END LICENSE BLOCK ******

cmake_minimum_required(VERSION 3.9)

project(wima C)

# Necessary to suppress a useless warning with QtCreator.
set(ignoreMe "${QT_QMAKE_EXECUTABLE}")
set(ignoreMe "${CMAKE_CXX_COMPILER}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT "${CMAKE_BUILD_TYPE}" MATCHES "Debug")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
endif()

enable_testing()

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

find_library(RT_LIB rt PATHS "/usr/lib" "/usr/local/lib")
find_library(MATH_LIB m PATHS "/usr/lib" "/usr/local/lib")
find_library(DL_LIB dl "/usr/lib" "/usr/local/lib")
find_library(PTHREAD_LIB pthread PATHS "/usr/lib" "/usr/local/lib")
find_library(JEMALLOC_LIB NAMES libjemalloc_pic.a jemalloc PATHS "/usr/lib" "/usr/local/lib")
find_library(GLFW_LIB glfw PATHS "/usr/lib" "/usr/local/lib")
find_library(YC_LIB NAMES libyc_pic.a yc PATHS "/usr/lib" "/usr/local/lib")
find_library(DYNA_LIB NAMES libdyna_pic.a dyna PATHS "/usr/lib" "/usr/local/lib")

set("WIMA_DOCS_DIR" "docs")
set("WIMA_INCLUDE_DIR" "include")
set("WIMA_LIB_DIR" "lib")
set("WIMA_SRC_DIR" "src")
set("WIMA_RES_DIR" "res")

set(WIMA_CMAKE_DIR "cmake")

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${WIMA_CMAKE_DIR}/.git")
	string(
		CONCAT WIMA_CMAKE_SUBMODULE_ERR
		"\nThe cmake git submodule is not available. "
		"Please run\ngit submodule update --init --recursive")
	message(FATAL_ERROR "${WIMA_CMAKE_SUBMODULE_ERR}")
endif()

add_subdirectory("cmake")

add_subdirectory("${WIMA_DOCS_DIR}")

include_directories("${WIMA_INCLUDE_DIR}")
add_subdirectory("${WIMA_INCLUDE_DIR}")

add_subdirectory("${WIMA_LIB_DIR}")

add_subdirectory("${WIMA_SRC_DIR}")

add_subdirectory("${WIMA_RES_DIR}")

add_subdirectory(tests)
