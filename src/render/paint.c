/*
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
 *	Function definitions for Wima's paint.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <nanovg.h>

#include <wima/render.h>

#include "render.h"

WimaPaint wima_paint_linearGradient(WimaRenderContext* ctx, float sx, float sy, float ex, float ey,
                                    WimaColor icol, WimaColor ocol)
{
	WimaCol ic, oc;
	WimaPnt p;

	ic.wima = icol;
	oc.wima = ocol;

	p.nvg = nvgLinearGradient(ctx->nvg, sx, sy, ex, ey, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_boxGradient(WimaRenderContext* ctx, float x, float y, float w, float h,
                                 float r, float f, WimaColor icol, WimaColor ocol)
{
	WimaCol ic, oc;
	WimaPnt p;

	ic.wima = icol;
	oc.wima = ocol;

	p.nvg = nvgBoxGradient(ctx->nvg, x, y, w, h, r, f, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_radialGradient(WimaRenderContext* ctx, float cx, float cy, float inr, float outr,
                                    WimaColor icol, WimaColor ocol)
{
	WimaCol ic, oc;
	WimaPnt p;

	ic.wima = icol;
	oc.wima = ocol;

	p.nvg = nvgRadialGradient(ctx->nvg, cx, cy, inr, outr, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_imagePattern(WimaRenderContext* ctx, float ox, float oy, float ex, float ey,
                                  float angle, int image, float alpha)
{
	// TODO: Write this function.

	WimaPnt p;

	return p.wima;
}