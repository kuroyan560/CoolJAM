#pragma once
#include "PlayerBullet.h"
#include "Importer.h"
#include "Model.h"
#include "SlowMgr.h"

std::shared_ptr<Model> PlayerBullet::s_model;

PlayerBullet::PlayerBullet() {

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_scale = SCALE;
	m_isActive = false;
	m_speed = 0.0f;

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
	m_speed = 0.0f;

}

void PlayerBullet::Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec) {

	/*===== ���������� =====*/

	m_pos = Pos;
	m_forwardVec = ForwardVec;
	m_scale = SCALE;
	m_isActive = true;
	m_speed = 0.0f;

}

void PlayerBullet::Update(const float& MapSize) {

	/*===== �X�V���� ======*/

	m_pos += m_forwardVec * m_speed * SlowMgr::Instance()->m_slow;

	m_speed += ADD_SPEED * SlowMgr::Instance()->m_slow;
	if (MAX_SPEED <= ADD_SPEED) {

		m_speed = MAX_SPEED;

	}

	// �}�b�v�O�ֈړ�������B
	if (MapSize + CHECK_HIT_SCALE < m_pos.Length()) {

		Init();

	}

}

#include"DrawFunc_Append.h"
void PlayerBullet::Draw() {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	m_transform.SetRotate(DirectX::XMMatrixRotationY(atan2f(m_forwardVec.x, m_forwardVec.z)));
	DrawFunc_Append::DrawModel(s_model, m_transform);
}