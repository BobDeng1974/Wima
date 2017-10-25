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
 *	Function definitions for render style
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <yc/assert.h>

#include <nanovg.h>

#include <wima/render.h>

#include "render.h"

#include "../global.h"

global_decl;
assert_msgs_decl;

void wima_style_antialias(WimaRenderContext* ctx, bool enabled) {
	assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgShapeAntiAlias(ctx->nvg, enabled);
}

void wima_style_stroke_color(WimaRenderContext* ctx, WimaColor color) {

	assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Translate between Wima and NanoVG.
	WimaCol c;
	c.wima = color;

	nvgStrokeColor(ctx->nvg, c.nvg);
}

void wima_style_stroke_paint(WimaRenderContext* ctx, WimaPaint paint) {

	assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Translate between Wima and NanoVG.
	WimaPnt p;
	p.wima = paint;

	nvgStrokePaint(ctx->nvg, p.nvg);
}

void wima_style_fill_color(WimaRenderContext* ctx, WimaColor color) {

	assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Translate between Wima and NanoVG.
	WimaCol c;
	c.wima = color;

	nvgFillColor(ctx->nvg, c.nvg);
}

void wima_style_fill_paint(WimaRenderContext* ctx, WimaPaint paint) {

	assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Translate between Wima and NanoVG.
	WimaPnt p;
	p.wima = paint;

	nvgFillPaint(ctx->nvg, p.nvg);
}

void wima_style_miter_limit(WimaRenderContext* ctx, float limit) {
	assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgMiterLimit(ctx->nvg, limit);
}

void wima_style_stroke_width(WimaRenderContext* ctx, float size) {
	assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgStrokeWidth(ctx->nvg, size);
}

void wima_style_line_cap(WimaRenderContext* ctx, WimaLineCap cap) {
	assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgLineCap(ctx->nvg, cap);
}

void wima_style_line_join(WimaRenderContext* ctx, WimaLineJoin join) {
	assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgLineJoin(ctx->nvg, join);
}

void wima_style_setGlobalAlpha(WimaRenderContext* ctx, float alpha) {
	assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgGlobalAlpha(ctx->nvg, alpha);
}
