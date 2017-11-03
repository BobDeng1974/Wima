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
 *	Functions for drawing paths.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <yc/assert.h>

#include <nanovg.h>

#include <wima/render.h>

#include "render.h"

#include "../global.h"

wima_assert_msgs_decl;

void wima_path_begin(WimaRenderContext* ctx) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgBeginPath(ctx->nvg);
}

void wima_path_moveTo(WimaRenderContext* ctx, WimaVecf pt) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgMoveTo(ctx->nvg, pt.x, pt.y);
}

void wima_path_lineTo(WimaRenderContext* ctx, WimaVecf pt) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgLineTo(ctx->nvg, pt.x, pt.y);
}

void wima_path_bezierTo(WimaRenderContext* ctx, WimaVecf pt, WimaVecf c1, WimaVecf c2) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgBezierTo(ctx->nvg, c1.x, c1.y, c2.x, c2.y, pt.x, pt.y);
}

void wima_path_quadTo(WimaRenderContext* ctx, WimaVecf pt, WimaVecf c) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgQuadTo(ctx->nvg, c.x, c.y, pt.x, pt.y);
}

void wima_path_arcTo(WimaRenderContext* ctx, WimaVecf pt1, WimaVecf pt2, float radius) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgArcTo(ctx->nvg, pt1.x, pt1.y, pt2.x, pt2.y, radius);
}

void wima_path_close(WimaRenderContext* ctx) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgClosePath(ctx->nvg);
}

void wima_path_winding(WimaRenderContext* ctx, WimaWinding dir) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgPathWinding(ctx->nvg, dir);
}

void wima_path_arc(WimaRenderContext* ctx, WimaVecf c, float r, float a0, float a1, WimaWinding dir) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgArc(ctx->nvg, c.x, c.y, r, a0, a1, dir);
}

void wima_path_rect(WimaRenderContext* ctx, WimaRectf rect) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgRect(ctx->nvg, rect.x, rect.y, rect.w, rect.h);
}

void wima_path_roundedRect(WimaRenderContext* ctx, WimaRectf rect, float r) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgRoundedRect(ctx->nvg, rect.x, rect.y, rect.w, rect.h, r);
}

void wima_path_roundedRectVary(WimaRenderContext* ctx, WimaRectf rect, float tl, float tr, float br, float bl) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgRoundedRectVarying(ctx->nvg, rect.x, rect.y, rect.w, rect.h, tl, tr, br, bl);
}

void wima_path_ellipse(WimaRenderContext* ctx, WimaVecf c, float rx, float ry) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgEllipse(ctx->nvg, c.x, c.y, rx, ry);
}

void wima_path_circle(WimaRenderContext* ctx, WimaVecf c, float r) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgCircle(ctx->nvg, c.x, c.y, r);
}

void wima_path_fill(WimaRenderContext* ctx) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgFill(ctx->nvg);
}

void wima_path_stroke(WimaRenderContext* ctx) {
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgStroke(ctx->nvg);
}
