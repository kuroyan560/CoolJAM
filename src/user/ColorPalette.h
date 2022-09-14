#pragma once
#include"../engine/Common/Color.h"
#include"../engine/Common/Vec.h"

namespace ColorPalette
{
	static const Color S_GREEN_COLOR(31, 245, 147, 255);
	static const Color S_PINK_COLOR(0.88f, 0.012f, 0.658f, 1.0f);
	static const Color S_LIGHT_PURPLE_COLOR(66, 28, 119, 255);
	static const Color S_DARK_PURPLE_COLOR(23, 14, 41, 255);
	static const Color S_RED(0.96f, 0.36f, 0.12f, 1.0f);


	void SearchMaxMinColor(Vec3<float>& Color, double& max, double& min, int& rgb);
	Vec3<float> RGBtoHSV(const Vec3<float>& RGB);
	Vec3<float> HSVtoRGB(const Vec3<float>& HSV);
}
