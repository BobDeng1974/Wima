/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

#include <yc/assert.h>

#include <nanovg.h>

#include <wima/render.h>

#include "render.h"
#include "../global.h"

wima_assert_msgs_decl;

WimaPaint wima_paint_linearGradient(WimaRenderContext* ctx, WimaVecf s, WimaVecf e,
                                    WimaColor icol, WimaColor ocol)
{
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol ic, oc;
	WimaPnt p;

	// Translate between Wima and NanoVG.
	ic.wima = icol;
	oc.wima = ocol;

	// Get the paint from NanoVG.
	p.nvg = nvgLinearGradient(ctx->nvg, s.x, s.y, e.x, e.y, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_boxGradient(WimaRenderContext* ctx, WimaRectf rect, float r, float f,
                                 WimaColor icol, WimaColor ocol)
{
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol ic, oc;
	WimaPnt p;

	// Translate between Wima and NanoVG.
	ic.wima = icol;
	oc.wima = ocol;

	// Get the paint from NanoVG.
	p.nvg = nvgBoxGradient(ctx->nvg, rect.x, rect.y, rect.w, rect.h, r, f, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_radialGradient(WimaRenderContext* ctx, WimaVecf c, float inr,
                                    float outr, WimaColor icol, WimaColor ocol)
{
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol ic, oc;
	WimaPnt p;

	// Translate between Wima and NanoVG.
	ic.wima = icol;
	oc.wima = ocol;

	// Get the paint from NanoVG.
	p.nvg = nvgRadialGradient(ctx->nvg, c.x, c.y, inr, outr, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_imagePattern(WimaRenderContext* ctx, WimaVecf o, WimaSizef e,
                                  float angle, int image, float alpha)
{
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// TODO: Write this function.

	WimaPnt p;

	return p.wima;
}
