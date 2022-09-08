#include "DriftParticle.h"
#include "Importer.h"
#include "Model.h"

DriftParticle::DriftParticle() {

	/*===== コンストラクタ =====*/

	// モデル
	m_model = Importer::Instance()->LoadModel("resource/user/", "playerDriftParticle.glb");;

	// 基本的な変数
	m_pos = Vec3<float>();			// 座標
	m_prevPos = Vec3<float>();		// 前フレームの座標
	m_forwardVec = Vec3<float>();	// 移動方向ベクトル
	m_nowFrameMoveVec = Vec3<float>();	// 前フレームの座標
	m_prevFrameMoveVec = Vec3<float>();	// 移動方向ベクトル
	m_speed = 0;					// 移動速度
	m_exitTimer = 0;				// 消えるまでのタイマー
	m_scale = SCALE;				// 大きさ
	m_gravity = 0;					// 重力
	m_isActive = false;				// 生存フラグ

}

void DriftParticle::Init() {

	/*===== 初期化処理 =====*/

	// 基本的な変数
	m_pos = Vec3<float>();			// 座標
	m_prevPos = Vec3<float>();		// 前フレームの座標
	m_forwardVec = Vec3<float>();	// 移動方向ベクトル
	m_nowFrameMoveVec = Vec3<float>();	// 前フレームの座標
	m_prevFrameMoveVec = Vec3<float>();	// 移動方向ベクトル
	m_speed = 0;					// 移動速度
	m_exitTimer = 0;				// 消えるまでのタイマー
	m_scale = SCALE;				// 大きさ
	m_gravity = 0;					// 重力
	m_isActive = false;				// 生存フラグ

}

void DriftParticle::Generate(const Vec3<float>& Pos, const float& Angle, const float& Rate) {

	/*===== 生成処理 =====*/

	m_playerPos = Pos;
	m_pos = Pos;
	m_prevPos = Pos;
	m_speed = KuroFunc::GetRand(MAX_SPEED / 2.0f, MAX_SPEED);
	m_scale = SCALE;
	m_gravity = 0;
	m_exitTimer = 0;
	m_isActive = true;
	m_nowFrameMoveVec = Vec3<float>();	// 前フレームの座標
	m_prevFrameMoveVec = Vec3<float>();	// 移動方向ベクトル

	m_transform.SetRotate(DirectX::XMMatrixRotationY(Angle + DirectX::XM_PIDIV2));	// 角度調節。

	// 移動方向ベクトルを求める。
	float randomAmount = 0.9f;
	m_forwardVec = Vec3<float>(sinf(Angle + KuroFunc::GetRand(-randomAmount, randomAmount)), 0.0f, cosf(Angle + KuroFunc::GetRand(-0.5f, randomAmount)));
	//m_forwardVec.y = KuroFunc::GetRand(-500, 500) / 1000.0f;
	//m_forwardVec.y += (Rate * 0.5f);
	m_forwardVec.y += 0.1f;
	m_forwardVec.Normalize();

}

void DriftParticle::Update(const Vec3<float>& Pos) {

	/*===== 更新処理 =====*/

	// 座標を保存。
	m_prevPos = m_pos;

	// 重力を計算。
	m_gravity += ADD_GRAVITY;
	if (MAX_GRAVITY < m_gravity) m_gravity = MAX_GRAVITY;

	// 移動させる。
	m_pos += m_forwardVec * m_speed;

	// 移動速度を減らす。
	m_speed -= SUB_SPEED;
	if (m_speed < SUB_SPEED) {
		m_speed = SUB_SPEED;
	}

	// 重力を加算。
	//m_pos -= m_gravity;

	// 消えるまでのタイマーを更新。
	++m_exitTimer;
	if (EXIT_TIMER < m_exitTimer) {

		// スケールを小さくする。
		m_scale -= 0.05f;
		if (m_scale < 0) {

			Init();

		}

	}

	// 前フレームの移動ベクトルを保存。
	m_prevFrameMoveVec = m_nowFrameMoveVec;
	m_nowFrameMoveVec = Vec3<float>(m_pos - m_prevPos).GetNormal();

}

#include "DrawFunc_Append.h"
void DriftParticle::Draw() {

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	m_transform.SetScale(m_scale);
	DrawFunc_Append::DrawModel(m_model, m_transform, 1.0f, { false,true,false });

}