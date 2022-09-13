#include"ColorPalette.h"

void ColorPalette::SearchMaxMinColor(Vec3<float> &Color, double &max, double &min, int &rgb)
{
	float r = Color.x;
	float g = Color.y;
	float b = Color.z;
	max = r;
	min = r;
	rgb = 0;

	//���ׂĂ̐F�������l�̏ꍇ
	if (r == g && g == b) {
		max = r;
		min = r;
		rgb = 3;
		return;
	}

	//�ő�l�����߂�
	if (max < g) {
		max = g;
		rgb = 1;
	}
	if (max < b) {
		max = b;
		rgb = 2;
	};
	//�ŏ��l�����߂�
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
	//�F�������߂�
	//R���ő�l�̏ꍇ
	if (colorIndex == 0) {
		hue = 60.0f * ((rgb.y - rgb.z) / (max - min));
	}
	//G���ő�l�̏ꍇ
	if (colorIndex == 1) {
		hue = 60.0f * (rgb.z - rgb.x) / (max - min) + 120;
	}
	//B���ő�l�̏ꍇ
	if (colorIndex == 2) {
		hue = 60.0f * (rgb.x - rgb.y) / (max - min) + 240;
	}
	//���ׂĂ̐F�������������ꍇ
	if (colorIndex == 3) {
		hue = 0;
	}
	//�F�����}�C�i�X�l�������ꍇ��360�𑫂�
	if (hue < 0) {
		hue += 360;
	}

	//�ʓx�����߂�
	s = (max - min) / max * 255.0f;

	//���x�����߂�
	value = max;

	return Vec3<float>(hue, s, value);
};

Vec3<float> ColorPalette::HSVtoRGB(const Vec3<float> &HSV)
{
	/*HSV -> RGB*/

	//�ő�l�ƍŏ��l�����߂�
	float max = HSV.z;
	float min = max - ((HSV.y / 255.0f) * max);

	//RGB�����߂�
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