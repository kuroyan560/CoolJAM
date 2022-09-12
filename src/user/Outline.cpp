#include "Outline.h"
#include"../engine/DrawFunc3D.h"
#include"../user/KazDrawFunc.h"

Outline::Outline(std::shared_ptr<Model> MODEL, const Vec3<float> *POS, const Vec3<float> &SCALE, const Color &COLOR)
{
	m_model = MODEL;
	m_pos = POS;
	m_color = COLOR;

	m_transform.SetScale(SCALE + Vec3<float>(0.3f, 0.3f, 0.3f));
}

void Outline::Init()
{
}

void Outline::Upadte()
{
	m_transform.SetPos(*m_pos);
}

void Outline::Draw(Camera &CAMERA)
{
	KazDrawFunc::DrawNonShadingModelSignalColor(m_model, m_transform, m_color, CAMERA);
}
