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

# We use the nanovg and yc includes, so we need this.
include_directories("../lib/nanovg/src")
include_directories("../lib/yc/include")

set(WIMA_GLOBAL_SRC

	# Add files here.
	"global.c"
)

set(WIMA_GLOBAL "${PROJECT_NAME}_global")
create_pic_library("${WIMA_GLOBAL}" "${WIMA_GLOBAL}" "${WIMA_GLOBAL_SRC}" YES

	# Add libraries here.
	"${DYNA_LIB}"
	"${YC_LIB}"
	"${JEMALLOC_LIB}"
	"${PTHREAD_LIB}"
	"${DL_LIB}"
)

set(WIMA_SRC

	# Put all source files here.
	"prop.c"
	"widget.c"
	"layout.c"
	"callbacks.c"
	"region.c"
	"area.c"
	"workspace.c"
	"window.c"
	"wima.c"
)

include_directories("${X11_INCLUDE_DIR}" "${OPENGL_INCLUDE_DIR}")

add_subdirectory(math)

add_subdirectory(render)
add_subdirectory(draw)

set(WIMA_GLAD "${PROJECT_NAME}_glad")

create_pic_library(
	"${PROJECT_NAME}" "${PROJECT_NAME}" "${WIMA_SRC}" "YES"

	# Put the needed libraries here.
	"${WIMA_RENDER}"
	"${WIMA_GLOBAL}"
	"${NANOVG_PIC}"
	"${WIMA_MATH}"
	"${DYNA_LIB}"
	"${YC_LIB}"
	"${GLFW_LIB}"
	"${WIMA_GLAD}"
	"${JEMALLOC_LIB}"
	"${DL_LIB}"
	"${OPENGL_LIBRARIES}"
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
