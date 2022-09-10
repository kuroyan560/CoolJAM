#pragma once
#include "PlayerBullet.h"
#include "Importer.h"
#include "Model.h"

std::shared_ptr<Model> PlayerBullet::s_model;

PlayerBullet::PlayerBullet() {

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_scale = SCALE;
	m_isActive = false;

	if (!s_model)
	{
		s_model = Importer::Instance()->LoadModel("resource/user/", "playerBullet.glb");
	}
	m_transform.SetScale(SCALE);

}

void PlayerBullet::Init() {

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_scale = SCALE;
	m_isActive = false;

}

void PlayerBullet::Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec) {

	/*===== ���������� =====*/

	m_pos = Pos;
	m_forwardVec = ForwardVec;
	m_scale = SCALE;
	m_isActive = true;

}

void PlayerBullet::Update(const float& MapSize) {

	/*===== �X�V���� ======*/

	m_pos += m_forwardVec * SPEED;

	// �}�b�v�O�ֈړ�������B
	if (MapSize + CHECK_HIT_SCALE < m_pos.Length()) {

		Init();

	}

}

#include"DrawFunc_Append.h"
void PlayerBullet::Draw() {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	DrawFunc_Append::DrawModel(s_model, m_transform);
}