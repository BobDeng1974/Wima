#	***** BEGIN LICENSE BLOCK *****
#
#	Copyright 2017 Yzena Tech
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

cmake_minimum_required(VERSION 3.2)

project(wima C)

# Necessary to suppress a useless warning with QtCreator.
set(ignoreMe "${QT_QMAKE_EXECUTABLE}")
set(ignoreMe "${CMAKE_CXX_COMPILER}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# If we are in debug mode...
if(("${CMAKE_BUILD_TYPE}" MATCHES "Debug" OR "${CMAKE_BUILD_TYPE}" MATCHES "RelWithDegInfo")
		AND "${CMAKE_C_COMPILER_ID}" MATCHES "Clang")

	# Create the sanitizer options.
	option(WIMA_SANITIZER_ADDRESS "Build with AddressSanitizer" ON)

	# If the memory sanitizer is in use, set the flag.
	if ("${WIMA_SANITIZER_ADDRESS}")
		string(CONCAT WIMA_SANITIZER_FLAGS " -fsanitize=address")
	endif()

	# Option to run the undefined behavior sanitizer.
	option(WIMA_SANITIZER_UNDEFINED "Build with UndefinedBehaviorSanitizer" ON)

	# If we want to run the undefined behavior sanitizer...
	if(WIMA_SANITIZER_UNDEFINED)
		string(CONCAT WIMA_SANITIZER_FLAGS " -fsanitize=undefined")
	endif()

endif()

# If we are using sanitizers...
if(DEFINED WIMA_SANITIZER_FLAGS)

	# We must use Clang as the linker.
	set(CMAKE_LINKER "${CMAKE_C_COMPILER}")

	# These are all options to make the print outs prettier.
	string(CONCAT WIMA_FLAGS "${WIMA_SANITIZER_FLAGS} -fno-omit-frame-pointer "
		"-fno-optimize-sibling-calls -fno-common")

	# We need to set compiler *and* linker flags.
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WIMA_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${WIMA_FLAGS}")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-sanitize-recover=all")

endif()

# If we are in release mode...
if("${CMAKE_BUILD_TYPE}" MATCHES "Release")

	option(WIMA_ENABLE_LTO "Build with link time optimization" OFF)

	if ("${WIMA_ENABLE_LTO}")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
	endif()

	# Have an option to harden Wima.
	option(WIMA_HARDEN "Build with hardened security" OFF)

	# If we are hardened, add Control Flow Integrity.
	# We would like to add Safe Stack, but it seg faults
	# with jemalloc right now.
	if("${WIMA_HARDEN}")

		# Set a bunch of initial flags.
		#set(WIMA_FLAGS "-fsanitize=cfi -fvisibility=hidden -fsanitize=safe-stack")
		set(WIMA_FLAGS "-fsanitize=cfi -fvisibility=hidden")

		# We need to set compiler flags.
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WIMA_FLAGS}")

	endif()

endif()

enable_testing()

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

find_library(RT_LIB rt PATHS "/usr/lib" "/usr/local/lib")
find_library(MATH_LIB m PATHS "/usr/lib" "/usr/local/lib")
find_library(DL_LIB dl "/usr/lib" "/usr/local/lib")
find_library(PTHREAD_LIB pthread PATHS "/usr/lib" "/usr/local/lib")

# Find the Yc library, if necessary.
if (NOT DEFINED YC_LIB)
	find_library(YC_LIB NAMES libyc.a yc PATHS "/usr/lib" "/usr/local/lib")
endif()

# Find the Dyna library, if necessary.
if (NOT DEFINED DYNA_LIB)
	find_library(DYNA_LIB NAMES libdyna.a dyna PATHS "/usr/lib" "/usr/local/lib")
endif()

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

add_subdirectory("${WIMA_CMAKE_DIR}")

add_subdirectory("${WIMA_DOCS_DIR}")

include_directories("${WIMA_INCLUDE_DIR}")
add_subdirectory("${WIMA_INCLUDE_DIR}")

add_subdirectory("${WIMA_LIB_DIR}")

add_subdirectory("${WIMA_SRC_DIR}")

add_subdirectory("${WIMA_RES_DIR}")

add_subdirectory(tests)
