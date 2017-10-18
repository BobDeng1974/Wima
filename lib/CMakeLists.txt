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

add_subdirectory(glad)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS ON CACHE BOOL "" FORCE)

add_subdirectory(glfw)

set(WIMA_GLAD "${WIMA_GLAD}" PARENT_SCOPE)

set(NANOVG "nanovg")
set(NANOVG "${NANOVG}" PARENT_SCOPE)

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
