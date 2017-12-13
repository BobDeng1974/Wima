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

# We use the nanovg, nanosvg, and yc includes, so we need this.
include_directories("../lib/nanovg/src")
include_directories("../lib/nanosvg/src")
include_directories("../lib/yc/include")
include_directories("../lib/glfw/include")

# Make sure we know what the GLFW CMake target is.
set(GLFW_LIB "glfw")

set(WIMA_GLOBAL_SRC

	# Add files here.
	"global.c"
)

set(WIMA_GLOBAL "${PROJECT_NAME}_global")
create_pic_library("${WIMA_GLOBAL}" "${WIMA_GLOBAL}" "${WIMA_GLOBAL_SRC}" NO YES

	# Add libraries here.
	"${DYNA_LIB}"
	"${YC_LIB}"
	"${JEMALLOC_LIB}"
	"${PTHREAD_LIB}"
	"${DL_LIB}"
)

set(WIMA_SRC

	# Put all source files here.
	"time.c"
	"cursor.c"
	"key.c"
	"monitor.c"
	"prop.c"
	"overlay.c"
	"item.c"
	"widget.c"
	"layout.c"
	"callbacks.c"
	"region.c"
	"editor.c"
	"area.c"
	"tree.c"
	"dialog.c"
	"workspace.c"
	"menu.c"
	"window.c"
	"wima.c"
)

include_directories("${X11_INCLUDE_DIR}" "${OPENGL_INCLUDE_DIR}")
include_directories("../include")

add_subdirectory(math)

add_subdirectory(render)

set(WIMA_GLAD "${PROJECT_NAME}_glad")
set(WIMA_PIC "${PROJECT_NAME}_pic")

create_pic_library(
	"${WIMA_PIC}" "${WIMA_PIC}" "${WIMA_SRC}" NO YES

	# Put the needed libraries here.
	"${WIMA_INPUT}"
	"${WIMA_DRAW}"
	"${WIMA_GLOBAL}"
	"${NANOVG_PIC}"
	"${WIMA_MATH}"
	"${DYNA_LIB}"
	"${YC_LIB}"
	"${GLFW_LIB}"
	"${WIMA_GLAD}"
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

merge_static_libs("${PROJECT_NAME}")
target_link_libraries(
	"${PROJECT_NAME}"

	# Put the needed libraries here.
	"${WIMA_INPUT}"
	"${WIMA_DRAW}"
	"${WIMA_GLOBAL}"
	"${NANOVG_PIC}"
	"${WIMA_MATH}"
	"${DYNA_LIB}"
	"${YC_LIB}"
	"${GLFW_LIB}"
	"${WIMA_GLAD}"
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
	"${PTHREAD_LIB}")

if (BUILD_SHARED_LIBS)
	create_shared_library(
		"${PROJECT_NAME}" "${PROJECT_NAME}" "${WIMA_SRC}" "YES"

		# Put the needed libraries here.
		"${WIMA_INPUT}"
		"${WIMA_DRAW}"
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
endif()
