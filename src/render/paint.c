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

#include <wima/render.h>

#include "render.h"

#include "../wima.h"

#include <yc/assert.h>

#include <nanosvg.h>
#include <nanovg.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaPaint wima_paint_linearGradient(WimaRenderContext* ctx, WimaVecf s, WimaVecf e, WimaColor icol, WimaColor ocol)
{
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol ic, oc;
	WimaPnt p;

	// Translate between Wima and NanoVG.
	ic.wima = icol;
	oc.wima = ocol;

	// Get the paint from NanoVG.
	p.nvg = nvgLinearGradient(ctx->nvg, s.x, s.y, e.x, e.y, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_boxGradient(WimaRenderContext* ctx, WimaRectf rect, float r, float f, WimaColor icol,
                                 WimaColor ocol)
{
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol ic, oc;
	WimaPnt p;

	// Translate between Wima and NanoVG.
	ic.wima = icol;
	oc.wima = ocol;

	// Get the paint from NanoVG.
	p.nvg = nvgBoxGradient(ctx->nvg, rect.x, rect.y, rect.w, rect.h, r, f, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_radialGradient(WimaRenderContext* ctx, WimaVecf c, float inr, float outr, WimaColor icol,
                                    WimaColor ocol)
{
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol ic, oc;
	WimaPnt p;

	// Translate between Wima and NanoVG.
	ic.wima = icol;
	oc.wima = ocol;

	// Get the paint from NanoVG.
	p.nvg = nvgRadialGradient(ctx->nvg, c.x, c.y, inr, outr, ic.nvg, oc.nvg);

	return p.wima;
}

WimaPaint wima_paint_imagePattern(WimaRenderContext* ctx, WimaVecf o, WimaSizef e, float angle, int image, float alpha)
{
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPnt p;

	p.nvg = nvgImagePattern(ctx->nvg, o.x, o.y, e.w, e.h, angle, image, alpha);

	return p.wima;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

NVGpaint wima_paint_svgLinearGradient(WimaRenderContext* ctx, NSVGgradient* gradient)
{
	float inverse[6];
	float sx, sy, ex, ey;

	nvgTransformInverse(inverse, gradient->xform);
	nvgTransformPoint(&sx, &sy, inverse, 0, 0);
	nvgTransformPoint(&ex, &ey, inverse, 0, 1);

	return nvgLinearGradient(ctx->nvg, sx, sy, ex, ey, wima_color_int(gradient->stops[0].color),
	                         wima_color_int(gradient->stops[gradient->nstops - 1].color));
}

NVGpaint wima_paint_svgRadialGradient(WimaRenderContext* ctx, NSVGgradient* gradient)
{
	float inverse[6];
	float cx, cy, r1, r2, inr, outr;

	nvgTransformInverse(inverse, gradient->xform);
	nvgTransformPoint(&cx, &cy, inverse, 0, 0);
	nvgTransformPoint(&r1, &r2, inverse, 0, 1);
	outr = r2 - cy;

	if (gradient->nstops == 3)
		inr = gradient->stops[1].offset * outr;
	else
	{
		inr = 0;
	}

	NVGpaint paint = nvgRadialGradient(ctx->nvg, cx, cy, inr, outr, wima_color_int(gradient->stops[0].color),
	                                   wima_color_int(gradient->stops[gradient->nstops - 1].color));

	return paint;
}
