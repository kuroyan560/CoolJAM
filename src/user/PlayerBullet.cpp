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

}

void PlayerBullet::Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec) {

	/*===== 初期化処理 =====*/

	m_pos = Pos;
	m_forwardVec = ForwardVec;
	m_scale = SCALE;
	m_isActive = true;

}

void PlayerBullet::Update(const float& MapSize) {

	/*===== 更新処理 ======*/

	m_pos += m_forwardVec * SPEED;

	// マップ外へ移動したら。
	if (MapSize < m_pos.Length()) {

		Init();

	}

}

#include"DrawFunc3D.h"
void PlayerBullet::Draw(Camera& NowCam) {

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	DrawFunc3D::DrawNonShadingModel(s_model, m_transform, NowCam);
}