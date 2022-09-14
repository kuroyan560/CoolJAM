#pragma once
#include "EnemyBullet.h"
#include "Importer.h"
#include "Model.h"
#include "SlowMgr.h"

std::shared_ptr<Model> EnemyBullet::s_model;

EnemyBullet::EnemyBullet() {

	/*===== �R���X�g���N�^ =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_scale = SCALE;
	m_isActive = false;

	if (!s_model)
	{
		s_model = Importer::Instance()->LoadModel("resource/user/", "enemyBullet.glb");
		
	}
	m_transform.SetScale(SCALE);

}

void EnemyBullet::Init() {

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_scale = SCALE;
	m_isActive = false;

}

void EnemyBullet::Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec) {

	/*===== ���������� =====*/

	m_pos = Pos;
	m_forwardVec = ForwardVec;
	m_scale = SCALE;
	m_transform.SetScale(14);
	m_isActive = true;

}

void EnemyBullet::Update(const float& MapSize) {

	/*===== �X�V���� ======*/

	m_pos += m_forwardVec * SPEED * SlowMgr::Instance()->m_slow;

	// �}�b�v�O�ֈړ�������B
	if (MapSize < m_pos.Length()) {

		Init();

	}

}

#include"DrawFunc_Append.h"
void EnemyBullet::Draw() {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	DrawFunc_Append::DrawModel(s_model, m_transform);

}