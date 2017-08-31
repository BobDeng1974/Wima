/**
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Project LFyre
 *
 *	Licensed under the Apache License, Version 2.0 (the "Apache License")
 *	with the following modification; you may not use this file except in
 *	compliance with the Apache License and the following modification to it:
 *	Section 6. Trademarks. is deleted and replaced with:
 *
 *	6. Trademarks. This License does not grant permission to use the trade
 *		names, trademarks, service marks, or product names of the Licensor
 *		and its affiliates, except as required to comply with Section 4(c) of
 *		the License and to reproduce the content of the NOTICE file.
 *
 *	You may obtain a copy of the Apache License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the Apache License with the above modification is
 *	distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *	KIND, either express or implied. See the Apache License for the specific
 *	language governing permissions and limitations under the Apache License.
 *
 *	****** END LICENSE BLOCK ******
 *
 *	*****************************************************************
 *
 *	******* BEGIN FILE DESCRIPTION *******
 *
 *	Defines useful math functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifdef _MSC_VER

#pragma warning (disable: 4996) // Switch off security warnings
#pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#include <float.h>

static float wima_fminf ( float a, float b )
{
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a < b) ? a : b));
}

static float wima_fmaxf ( float a, float b )
{
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a > b) ? a : b));
}

static double wima_fmin ( double a, double b )
{
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a < b) ? a : b));
}

static double wima_fmax ( double a, double b )
{
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a > b) ? a : b));
}

#else

#define wima_fminf(a, b) fminf(a, b)
#define wima_fmaxf(a, b) fmaxf(a, b)
#define wima_fmin(a, b) fmin(a, b)
#define wima_fmax(a, b) fmax(a, b)

#endif

float wima_clamp(float v, float mn, float mx) {
	return (v > mx) ? mx : ((v < mn) ? mn : v);
}
