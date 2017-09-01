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

# We need to include NanoVG's headers.
include_directories(../../lib/nanovg/src)

set(WIMA_MATH_PIC "${PROJECT_NAME}_math_pic")

set(WIMA_UI_SRC

	#Put source files here.
	"ui.c"
)

set(WIMA_UI "wima_ui")

set(WIMA_UI_SHARED "${WIMA_UI}_shared")
set(WIMA_UI_SHARED "${WIMA_UI_SHARED}" PARENT_SCOPE)
set(WIMA_UI_STATIC "${WIMA_UI}_static")
set(WIMA_UI_STATIC "${WIMA_UI_STATIC}" PARENT_SCOPE)
set(WIMA_UI_PIC "${WIMA_UI}_pic")
set(WIMA_UI_PIC "${WIMA_UI_PIC}" PARENT_SCOPE)

create_all_libraries(

	"${WIMA_UI_SHARED}"
	"${WIMA_UI_STATIC}"
	"${WIMA_UI_PIC}"

	"${WIMA_UI}"
	"${WIMA_UI_SRC}"
	YES

	# Add libraries here.
	"${WIMA_MATH_PIC}"
	"${NANOVG_PIC}"
	"${JEMALLOC_PIC}"
)
