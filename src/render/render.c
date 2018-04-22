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
 *	Function definitions for render state.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/render.h>

#include "render.h"

#include "../wima.h"

#include "../windows/window.h"

#include <yc/assert.h>

#include <nanovg.h>

void wima_render_save(WimaRenderContext* ctx)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	wassert(ctx->stackCount < WIMA_WIN_RENDER_STACK_MAX, WIMA_ASSERT_WIN_RENDER_STACK_MAX);

	++(ctx->stackCount);
	nvgSave(ctx->nvg);
}

void wima_render_restore(WimaRenderContext* ctx)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	wassert(ctx->stackCount > 0, WIMA_ASSERT_WIN_RENDER_STACK);

	--(ctx->stackCount);
	nvgRestore(ctx->nvg);
}

void wima_render_reset(WimaRenderContext* ctx)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgReset(ctx->nvg);
}

void wima_render_resetTransform(WimaRenderContext* ctx)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgResetTransform(ctx->nvg);
}

void wima_render_transform(WimaRenderContext* ctx, WimaTransform tx)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgTransform(ctx->nvg, tx.v[0], tx.v[1], tx.v[2], tx.v[3], tx.v[4], tx.v[5]);
}

void wima_render_translate(WimaRenderContext* ctx, WimaVecf vec)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgTranslate(ctx->nvg, vec.x, vec.y);
}

void wima_render_rotate(WimaRenderContext* ctx, float angle)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgRotate(ctx->nvg, angle);
}

void wima_render_skewX(WimaRenderContext* ctx, float angle)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgSkewX(ctx->nvg, angle);
}

void wima_render_skewY(WimaRenderContext* ctx, float angle)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgSkewY(ctx->nvg, angle);
}

void wima_render_scale(WimaRenderContext* ctx, float x, float y)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgScale(ctx->nvg, x, y);
}

WimaTransform wima_render_currentTransform(WimaRenderContext* ctx)
{
	wima_assert_init;

	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaTransform result;

	nvgCurrentTransform(ctx->nvg, result.v);

	return result;
}

void wima_render_scissor(WimaRenderContext* ctx, WimaRectf rect)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgScissor(ctx->nvg, rect.x, rect.y, rect.w, rect.h);
}

void wima_render_intersectScissor(WimaRenderContext* ctx, WimaRectf rect)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgIntersectScissor(ctx->nvg, rect.x, rect.y, rect.w, rect.h);
}

void wima_render_resetScissor(WimaRenderContext* ctx)
{
	wima_assert_init;
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	nvgResetScissor(ctx->nvg);
}
