#include "Outline.h"
#include"../engine/DrawFunc3D.h"
#include"../user/KazDrawFunc.h"
#include"../user/ColorPalette.h"

Outline::Outline(std::shared_ptr<Model> MODEL, const Transform *TRANSFORM, const Color &COLOR)
{
	m_model = MODEL;
	m_color = COLOR;
	m_transform = TRANSFORM;
	//m_hitColor = Color(245, 93, 31, 255);
	m_hitColor = ColorPalette::S_PINK_COLOR;
}

void Outline::Init()
{
}

void Outline::Upadte()
{
}

void Outline::Draw(Camera &CAMERA, bool HIT_FLAG)
{
	if (m_transform == nullptr)
	{
		return;
	}

	if (UsersInput::Instance()->KeyInput(DIK_Z))
	{
		m_hitColor = ColorPalette::S_GREEN_COLOR;
	}
	if (UsersInput::Instance()->KeyInput(DIK_X))
	{
		m_hitColor = ColorPalette::S_PINK_COLOR;
	}
	if (UsersInput::Instance()->KeyInput(DIK_C))
	{
		m_hitColor = ColorPalette::S_LIGHT_PURPLE_COLOR;
	}
	if (UsersInput::Instance()->KeyInput(DIK_V))
	{
		m_hitColor = ColorPalette::S_DARK_PURPLE_COLOR;
	}
	if (UsersInput::Instance()->KeyInput(DIK_B))
	{
		m_hitColor = Color(245, 93, 31, 255);
	}




	Color nowColor = m_color;
	if (HIT_FLAG)
	{
		nowColor = m_hitColor;
	}


	Transform transform = *m_transform;
	transform.SetScale(transform.GetScale() + Vec3<float>(0.3f, 0.3f, 0.3f));
	KazDrawFunc::DrawNonShadingModelSignalColor(m_model, transform, nowColor, CAMERA);
}
