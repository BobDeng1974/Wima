/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2018 Yzena Tech
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
 *	Functions for transforming in space.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/math.h>

WimaTransform wima_transform_identity()
{
	WimaTransform tx;

	nvgTransformIdentity(tx.v);

	return tx;
}

WimaTransform wima_transform_translate(WimaTransform src, float tx, float ty)
{
	nvgTransformTranslate(src.v, tx, ty);
	return src;
}

WimaTransform wima_transform_scale(WimaTransform src, float sx, float sy)
{
	nvgTransformScale(src.v, sx, sy);
	return src;
}

WimaTransform wima_transform_rotate(WimaTransform src, float a)
{
	nvgTransformRotate(src.v, a);
	return src;
}

WimaTransform wima_transform_skewX(WimaTransform src, float a)
{
	nvgTransformSkewX(src.v, a);
	return src;
}

WimaTransform wima_transform_skewY(WimaTransform src, float a)
{
	nvgTransformSkewY(src.v, a);
	return src;
}

WimaTransform wima_transform_multiply(WimaTransform src1, WimaTransform src2)
{
	nvgTransformMultiply(src1.v, src2.v);
	return src1;
}

WimaTransform wima_transform_premultiply(WimaTransform src1, WimaTransform src2)
{
	nvgTransformPremultiply(src1.v, src2.v);
	return src1;
}

WimaTransform wima_transform_inverse(WimaTransform src)
{
	WimaTransform dest;

	nvgTransformInverse(dest.v, src.v);

	return dest;
}

WimaVecf wima_transform_point(WimaTransform transform, WimaVecf pos)
{
	WimaVecf result;

	nvgTransformPoint(&result.x, &result.y, transform.v, pos.x, pos.y);

	return result;
}
