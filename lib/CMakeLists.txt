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

add_subdirectory(glad)

set(WIMA_GLAD "${WIMA_GLAD}" PARENT_SCOPE)

set(WIMA_GLFW_DIR "glfw")

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${WIMA_GLFW_DIR}/.git")
	string(
		CONCAT WIMA_GLFW_SUBMODULE_ERR
		"\nThe GLFW git submodule is not available. "
		"Please run\ngit submodule update --init --recursive")
	message(FATAL_ERROR "${WIMA_GLFW_SUBMODULE_ERR}")
endif()

# We need to set some GLFW cache variables here.
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

set(GLFW_TARGET "glfw")
set(GLFW_NAME "${GLFW_TARGET}3")
set(GLFW_LIB "lib${GLFW_NAME}.a")
set(GLFW_INC_DIR "glfw/include")
set(GLFW_LIB_PATH "glfw/src")
set(GLFW_LIB_ORIG "${GLFW_LIB_PATH}/${GLFW_LIB}")
set(GLFW_OUT "${CMAKE_CURRENT_BINARY_DIR}/${GLFW_LIB}")
set(GLFW_PIC "${GLFW_TARGET}_pic")
set(GLFW_CUSTOM_TARGET "${GLFW_PIC}_custom_target")

# Copy the object file/
add_custom_command(
	OUTPUT "${GLFW_OUT}"
	COMMAND ${CMAKE_COMMAND} -E copy ${GLFW_LIB_ORIG} ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS "${GLFW_TARGET}"
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(${GLFW_CUSTOM_TARGET} DEPENDS ${GLFW_OUT})
add_library(${GLFW_PIC} STATIC IMPORTED GLOBAL)
add_dependencies(${GLFW_PIC} ${GLFW_CUSTOM_TARGET} ${GLFW_OUT} ${GLFW_TARGET})

# Make sure to set important properties.
set_target_properties("${GLFW_PIC}"
	PROPERTIES
	OUTPUT_NAME "${GLFW_NAME}"
	DEPENDS "${GLFW_OUT}"
	IMPORTED_LOCATION "${GLFW_OUT}"
	INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INC_DIR}"
)

# Put on the merge list.
merge_lib(${GLFW_PIC} ${GLFW_NAME})

set(NANOVG "nanovg")
set(NANOVG "${NANOVG}" PARENT_SCOPE)

set(WIMA_NVG_DIR "${NANOVG}")

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${WIMA_NVG_DIR}/.git")
	string(
		CONCAT WIMA_NVG_SUBMODULE_ERR
		"\nThe NanoVG git submodule is not available. "
		"Please run\ngit submodule update --init --recursive")
	message(FATAL_ERROR "${WIMA_NVG_SUBMODULE_ERR}")
endif()

add_definitions(
	-DNANOVG_GL3_IMPLEMENTATION
	-DNANOVG_GL_IMPLEMENTATION
	-DSTBI_MALLOC=ymalloc
	-DSTBI_REALLOC=yrealloc
	-DSTBI_FREE=yfree
)

set(NANOVG_SRC_DIR "nanovg/src")

include_directories(".")

include_directories("${NANOVG_SRC_DIR}")

set(NANOVG_SRC

	# Add source files here.
	"${NANOVG_SRC_DIR}/nanovg.c"
	#"${NANOVG_SRC_DIR}/nanovg_pick.c"
)

create_pic_library(
	"${NANOVG}" "${NANOVG}" "${NANOVG_SRC}" NO

	# Add necessary libraries here.
	"${WIMA_GLAD}"
	"${MATH_LIB}"
	"${YC_LIB}"
	"${JEMALLOC_LIB}"
	"${PTHREAD_LIB}"
	"${DL_LIB}"
)
