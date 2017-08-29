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

set("WIMA_SRC"

	# Put all source files here.
	"callbacks.c"
	"region.c"
	"area.c"
	"workspace.c"
	"window.c"
	"wima.c"
)

find_package(X11 REQUIRED)

find_library(RT_LIB rt PATHS "/usr/lib" "/usr/local/lib")
find_library(MATH_LIB m PATHS "/usr/lib" "/usr/local/lib")
find_library(DL_LIB dl "/usr/lib" "/usr/local/lib")
find_library(PTHREAD_LIB pthread PATHS "/usr/lib" "/usr/local/lib")
find_library(JEMALLOC_LIB NAMES libjemalloc_pic.a jemalloc PATHS "/usr/lib" "/usr/local/lib")
find_library(GLFW_LIB glfw PATHS "/usr/lib" "/usr/local/lib")
find_library(DYNA_LIB NAMES libdyna_pic.a dyna PATHS "/usr/lib" "/usr/local/lib")

include_directories("${X11_INCLUDE_DIR}")

add_subdirectory(oui)
add_subdirectory(bnd)

set(WIMA_SHARED "${PROJECT_NAME}_shared")
set(WIMA_STATIC "${PROJECT_NAME}_static")
set(WIMA_PIC "${PROJECT_NAME}_pic")

create_all_libraries(
	"${WIMA_SHARED}" "${WIMA_STATIC}" "${WIMA_PIC}"
	"${PROJECT_NAME}" "${WIMA_SRC}" "YES"

	# Put the needed libraries here.
	"${DYNA_LIB}"
	"${GLFW_LIB}"
	"${WIMA_GLAD_PIC}"
	"${JEMALLOC_LIB}"
	"${DL_LIB}"
	"${X11_X11_LIB}"
	"${X11_Xi_LIB}"
	"${X11_Xrandr_LIB}"
	"${X11_Xxf86vm_LIB}"
	"${X11_Xinerama_LIB}"
	"${X11_Xcursor_LIB}"
	"${RT_LIB}"
	"${MATH_LIB}"
	"${PTHREAD_LIB}"
)

add_subdirectory(tests)
