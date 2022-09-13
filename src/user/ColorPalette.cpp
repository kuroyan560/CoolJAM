#include"ColorPalette.h"

void ColorPalette::SearchMaxMinColor(Vec3<float> &Color, double &max, double &min, int &rgb)
{
	float r = Color.x;
	float g = Color.y;
	float b = Color.z;
	max = r;
	min = r;
	rgb = 0;

	//すべての色が同じ値の場合
	if (r == g && g == b) {
		max = r;
		min = r;
		rgb = 3;
		return;
	}

	//最大値を求める
	if (max < g) {
		max = g;
		rgb = 1;
	}
	if (max < b) {
		max = b;
		rgb = 2;
	};
	//最小値を求める
	if (min > g) {
		min = g;
	}
	if (min > b) {
		min = b;
	}

	}

Vec3<float> ColorPalette::RGBtoHSV(const Vec3<float> &RGB)
{
	double hue = 0;
	double s = 0;
	double value = 0;
	double max = 0;
	double min = 0;
	int colorIndex = 0;
	Vec3<float> rgb = RGB;
	ColorPalette::SearchMaxMinColor(rgb, max, min, colorIndex);

	/*RGB -> HSV*/
	//色相を求める
	//Rが最大値の場合
	if (colorIndex == 0) {
		hue = 60.0f * ((rgb.y - rgb.z) / (max - min));
	}
	//Gが最大値の場合
	if (colorIndex == 1) {
		hue = 60.0f * (rgb.z - rgb.x) / (max - min) + 120;
	}
	//Bが最大値の場合
	if (colorIndex == 2) {
		hue = 60.0f * (rgb.x - rgb.y) / (max - min) + 240;
	}
	//すべての色が同じだった場合
	if (colorIndex == 3) {
		hue = 0;
	}
	//色相がマイナス値だった場合は360を足す
	if (hue < 0) {
		hue += 360;
	}

	//彩度を求める
	s = (max - min) / max * 255.0f;

	//明度を求める
	value = max;

	return Vec3<float>(hue, s, value);
};

Vec3<float> ColorPalette::HSVtoRGB(const Vec3<float> &HSV)
{
	/*HSV -> RGB*/

	//最大値と最小値を求める
	float max = HSV.z;
	float min = max - ((HSV.y / 255.0f) * max);

	//RGBを求める
	float hue = HSV.x;
	Vec3<float> rgb;
	if (hue >= 0 && hue < 60) {
		rgb.x = max;
		rgb.y = (hue / 60) * (max - min) + min;
		rgb.z = min;
	}
	else if (hue >= 60 && hue < 120) {
		rgb.x = ((120 - hue) / 60) * (max - min) + min;
		rgb.y = max;
		rgb.z = min;
	}
	else if (hue >= 120 && hue < 180) {
		rgb.x = min;
		rgb.y = max;
		rgb.z = ((hue - 120) / 60) * (max - min) + min;
	}
	else if (hue >= 180 && hue < 240) {
		rgb.x = min;
		rgb.y = ((240 - hue) / 60) * (max - min) + min;
		rgb.z = max;
	}
	else if (hue >= 240 && hue < 300) {
		rgb.x = ((hue - 240) / 60) * (max - min) + min;
		rgb.y = min;
		rgb.z = max;
	}
	else if (hue >= 300 && hue <= 360) {
		rgb.x = max;
		rgb.y = min;
		rgb.z = ((360 - hue) / 60) * (max - min) + min;
	}

	rgb = Vec3<float>(rgb.x, rgb.y, rgb.z);

	return rgb;
};