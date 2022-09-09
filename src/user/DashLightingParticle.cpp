#include "DashLightingParticle.h"
#include"../engine/DrawFunc3D.h"

DashLightingParticle::DashLightingParticle()
{
}

void DashLightingParticle::Init(const Vec3<float> &POS)
{
}

void DashLightingParticle::Update()
{
	m_pos;

}

void DashLightingParticle::Draw(Camera &CAMERA)
{
	DrawFunc3D::DrawLine(CAMERA, m_startPos, m_endPos, Color(255, 255, 0, 255), 1.3f);
}
