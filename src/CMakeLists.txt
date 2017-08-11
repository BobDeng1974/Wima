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
	"wima.c"
	"callbacks.c"
	"window.c"
	"workspace.c"
	"area.c"
)

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

find_library(RT_LIB rt PATHS "/usr/lib" "/usr/local/lib")
find_library(MATH_LIB m PATHS "/usr/lib" "/usr/local/lib")
find_library(DL_LIB dl "/usr/lib" "/usr/local/lib")
find_library(PTHREAD_LIB pthread PATHS "/usr/lib" "/usr/local/lib")
find_library(JEMALLOC_LIB jemalloc PATHS "/usr/lib" "/usr/local/lib")
find_library(GLFW_LIB glfw3 PATHS "/usr/lib" "/usr/local/lib")
find_library(DYNA_LIB dyna PATHS "/usr/lib" "/usr/local/lib")

include_directories("${OPENGL_INCLUDE_DIR}" "${X11_INCLUDE_DIR}")

add_subdirectory(layout)

create_library(wima "${WIMA_SRC}"

	# Put the needed libraries here.
	"${DYNA_LIB}"
	"${GLFW_LIB}"
	"${JEMALLOC_LIB}"
	"${DL_LIB}"
	"${OPENGL_gl_LIBRARY}"
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
