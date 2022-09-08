#pragma once
#include "EnemyBullet.h"
#include "Importer.h"
#include "Model.h"

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
	m_isActive = true;

}

void EnemyBullet::Update(const float& MapSize) {

	/*===== �X�V���� ======*/

	m_pos += m_forwardVec * SPEED;

	// �}�b�v�O�ֈړ�������B
	if (MapSize < m_pos.Length()) {

		Init();

	}

}

#include"DrawFunc3D.h"
void EnemyBullet::Draw(Camera& NowCam) {

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	DrawFunc3D::DrawNonShadingModel(s_model, m_transform, NowCam);

}