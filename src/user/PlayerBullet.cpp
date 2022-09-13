#pragma once
#include "PlayerBullet.h"
#include "Importer.h"
#include "Model.h"

std::shared_ptr<Model> PlayerBullet::s_model;

PlayerBullet::PlayerBullet() {

	/*===== コンストラクタ =====*/

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

	/*===== 初期化処理 =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_scale = SCALE;
	m_isActive = false;
	m_speed = 0.0f;

}

void PlayerBullet::Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec) {

	/*===== 初期化処理 =====*/

	m_pos = Pos;
	m_forwardVec = ForwardVec;
	m_scale = SCALE;
	m_isActive = true;
	m_speed = 0.0f;

}

void PlayerBullet::Update(const float& MapSize) {

	/*===== 更新処理 ======*/

	m_pos += m_forwardVec * m_speed;

	m_speed += ADD_SPEED;
	if (MAX_SPEED <= ADD_SPEED) {

		m_speed = MAX_SPEED;

	}

	// マップ外へ移動したら。
	if (MapSize + CHECK_HIT_SCALE < m_pos.Length()) {

		Init();

	}

}

#include"DrawFunc_Append.h"
void PlayerBullet::Draw() {

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	m_transform.SetRotate(DirectX::XMMatrixRotationY(atan2f(m_forwardVec.x, m_forwardVec.z)));
	DrawFunc_Append::DrawModel(s_model, m_transform);
}