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

# We use the nanovg includes, so we need this.
include_directories("../../lib/nanovg/src")

set(WIMA_GLOBAL "${PROJECT_NAME}_global")
set(WIMA_MATH_PIC "${PROJECT_NAME}_math_pic")

set(WIMA_THEME_SRC

	#Put source files here.
	"color.c"
	"theme.c"
)

set(WIMA_UI_PIC "wima_ui_pic")

set(WIMA_THEME wima_theme)

set(WIMA_THEME_SHARED "${WIMA_THEME}_shared")
set(WIMA_THEME_SHARED "${WIMA_THEME_SHARED}" PARENT_SCOPE)
set(WIMA_THEME_STATIC "${WIMA_THEME}_static")
set(WIMA_THEME_STATIC "${WIMA_THEME_STATIC}" PARENT_SCOPE)
set(WIMA_THEME_PIC "${WIMA_THEME}_pic")
set(WIMA_THEME_PIC "${WIMA_THEME_PIC}" PARENT_SCOPE)

create_all_libraries(

	"${WIMA_THEME_SHARED}"
	"${WIMA_THEME_STATIC}"
	"${WIMA_THEME_PIC}"

	"${WIMA_THEME}"
	"${WIMA_THEME_SRC}"
	YES

	# Add libraries here.
	"${WIMA_UI_PIC}"
	"${WIMA_GLOBAL}"
	"${NANOVG_PIC}"
	"${WIMA_MATH_PIC}"
	"${JEMALLOC_PIC}"
)
