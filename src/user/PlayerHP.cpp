#include "PlayerHP.h"
#include "../engine/Importer.h"
#include <DrawFunc_Append.h>

PlayerHP::PlayerHP(const float& Angle)
{

	/*===== コンストラクタ =====*/

	m_model = Importer::Instance()->LoadModel("resource/user/", "player_hp.glb");
	m_alpha = 0.0f;
	m_isActive = false;
	m_drawAngle = 0.0f;
	m_drawAngle = Angle;

}

void PlayerHP::Init()
{

	/*===== 初期化処理 =====*/

	m_alpha = 0.0f;
	m_isActive = false;

}

void PlayerHP::Update(const Vec3<float>& EnemyPos, const float& EnemySize)
{

	/*===== 更新処理 =======*/

	// 座標を設定。
	m_transform.SetPos(EnemyPos);

	// 中心からずらす量。
	float moveScale = EnemySize + (SIZE * 2.0f);

	// ずらす角度。
	Vec3<float> moveDir = Vec3<float>(cosf(m_drawAngle), 0.0f, sinf(m_drawAngle));

	// 座標を描画角度分ずらす。
	m_transform.SetPos(m_transform.GetPos() + moveDir * moveScale);

	// 生存フラグが立っていたら。
	if (m_isActive) {

		m_alpha += (1.0f - m_alpha) / 10.0f;

	}
	else {

		// アルファが0より大きかったら。
		if (0 < m_alpha) {

			m_alpha += (0.0f - m_alpha) / 10.0f;

		}

	}

}

void PlayerHP::Draw()
{

	/*===== 描画処理 =====*/

	// アルファ値が0.05f以下だったら処理を行わない。
	if (m_alpha <= 0.05f) return;

	m_transform.SetPos(m_transform.GetPos() + Vec3<float>(0.0f, 0.1f, 0.0f));
	m_transform.SetRotate(DirectX::XMMatrixRotationY(-m_drawAngle));
	m_transform.SetScale(SIZE);

	RenderTargetSwitch renderTargetSwitch = RenderTargetSwitch(m_alpha, 1.0f, 1.0f);
	DrawFunc_Append::DrawModel(m_model, m_transform, renderTargetSwitch, false, false, nullptr, AlphaBlendMode_Trans);

}
