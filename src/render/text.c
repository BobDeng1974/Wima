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
 *	Function definitions for text.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <nanovg.h>

#include <wima/render.h>

#include "render.h"

// TODO: Improve the API.

void wima_text_blur(WimaRenderContext* ctx, float blur) {
	nvgFontBlur(ctx->nvg, blur);
}

void wima_text_letterSpacing(WimaRenderContext* ctx, float spacing) {
	nvgTextLetterSpacing(ctx->nvg, spacing);
}

void wima_text_lineHeight(WimaRenderContext* ctx, float lineHeight) {
	nvgTextLineHeight(ctx->nvg, lineHeight);
}

void wima_text_align(WimaRenderContext* ctx, WimaTextAlign align) {
	nvgTextAlign(ctx->nvg, align);
}

float wima_text(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end) {
	return nvgText(ctx->nvg, pt.x, pt.y, string, end);
}

void wima_text_box(WimaRenderContext* ctx, WimaVecf pt, float breakRowWidth,
                   const char* string, const char* end)
{
	nvgTextBox(ctx->nvg, pt.x, pt.y, breakRowWidth, string, end);
}

float wima_text_bounds(WimaRenderContext* ctx, WimaVecf pt, const char* string,
                       const char* end, WimaRectf* bounds)
{
	return nvgTextBounds(ctx->nvg, pt.x, pt.y, string, end, bounds->v);
}

WimaRectf wima_text_box_bounds(WimaRenderContext* ctx, WimaVecf pt, float breakRowWidth,
                               const char* string, const char* end)
{
	WimaRectf result;

	nvgTextBoxBounds(ctx->nvg, pt.x, pt.y, breakRowWidth, string, end, result.v);

	return result;
}

int wima_text_glyphPositions(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end,
                             WimaGlyphPosition* positions, int maxPositions)
{
	return nvgTextGlyphPositions(ctx->nvg, pt.x, pt.y, string, end, (NVGglyphPosition*) positions, maxPositions);
}

WimaTextMetrics wima_text_metrics(WimaRenderContext* ctx) {

	WimaTextMetrics result;

	nvgTextMetrics(ctx->nvg, &result.ascender, &result.descender, &result.lineHeight);

	return result;
}

int wima_text_breakLines(WimaRenderContext* ctx, const char* string, const char* end,
                         float breakRowWidth, WimaTextRow* rows, int maxRows)
{
	return nvgTextBreakLines(ctx->nvg, string, end, breakRowWidth, (NVGtextRow*) rows, maxRows);
}
