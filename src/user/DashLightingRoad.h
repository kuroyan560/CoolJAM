#pragma once
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"DashLightingParticle.h"
#include"../engine/Common/Color.h"

class DashLightingRoad
{
public:
	DashLightingRoad(int ROAD_INDEX);
	void Init(const Vec3<float> &POS);
	void Update(const Vec3<float> &POS, int NOW_INDEX, bool DISAPPEAR_FLAG);
	void Draw(Camera &Cam);

private:
	Vec3<float> m_startPos, m_endPos, m_baseStartPos;

	Color m_color;
	int m_indexNum;
	float m_lenghtRate;
	bool m_initFlag;
	bool m_disappearFlag;
	int m_timer;
	int m_startTimer;
	float m_distance;
};

