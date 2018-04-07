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

# We use the nanovg, nanosvg, and GLFW includes, so we need this.
include_directories("../lib/nanovg/src")
include_directories("../lib/nanosvg/src")
include_directories("../lib/glfw/include")

set(WIMA_SRC

	# Put all source files here.
	"wima.c"
)

include_directories("${X11_INCLUDE_DIR}" "${OPENGL_INCLUDE_DIR}")
include_directories("../include")

add_subdirectory(math)
add_subdirectory(time)
add_subdirectory(monitor)
add_subdirectory(prop)
add_subdirectory(input)
add_subdirectory(render)
add_subdirectory(layout)
add_subdirectory(area)
add_subdirectory(window)
add_subdirectory(event)

set(WIMA_GLAD "${PROJECT_NAME}_glad")
set(WIMA_PIC "${PROJECT_NAME}_pic")

create_merge_library(
	"${WIMA_PIC}" "${WIMA_SRC}"

	# Put the needed libraries here.
	"${DYNA_LIB}"
	"${YC_LIB}"
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
target_compile_options("${WIMA_PIC}" BEFORE PUBLIC "-Wall" PUBLIC "-Wextra")

if ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")

	option(WIMA_LOOP_TIMER "Add the loop timer (only in debug mode)" OFF)

	if (WIMA_LOOP_TIMER)
		target_compile_definitions("${WIMA_PIC}" PUBLIC "WIMA_LOOP_TIMER")
	endif()

endif()

merge_libs("${PROJECT_NAME}"

	# Put the needed libraries here.
	"${DYNA_LIB}"
	"${YC_LIB}"
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
