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

set(WIMA_GLAD "${WIMA_GLAD}" PARENT_SCOPE)
set(WIMA_GLAD_SHARED "${WIMA_GLAD_SHARED}" PARENT_SCOPE)
set(WIMA_GLAD_STATIC "${WIMA_GLAD_STATIC}" PARENT_SCOPE)
set(WIMA_GLAD_PIC "${WIMA_GLAD_PIC}" PARENT_SCOPE)

set(NANOVG "nanovg")

set(NANOVG_SHARED "${NANOVG}_shared")
set(NANOVG_SHARED "${NANOVG_SHARED}" PARENT_SCOPE)
set(NANOVG_STATIC "${NANOVG}_static")
set(NANOVG_STATIC "${NANOVG_STATIC}" PARENT_SCOPE)
set(NANOVG_PIC "${NANOVG}_pic")
set(NANOVG_PIC "${NANOVG_PIC}" PARENT_SCOPE)

add_definitions(-DNANOVG_GL3_IMPLEMENTATION -DNANOVG_GL_IMPLEMENTATION)

set(NANOVG_SRC_DIR "nanovg/src")

include_directories("${NANOVG_SRC_DIR}")

set(NANOVG_SRC

	# Add source files here.
	"${NANOVG_SRC_DIR}/nanovg.c"
	"${NANOVG_SRC_DIR}/nanovg_pick.c"
)

create_all_libraries(
	"${NANOVG_SHARED}"
	"${NANOVG_STATIC}"
	"${NANOVG_PIC}"
	"${NANOVG}"
	"${NANOVG_SRC}"
	YES

	# Add necessary libraries here.
	"${WIMA_GLAD_PIC}"
	"${MATH_LIB}"
)