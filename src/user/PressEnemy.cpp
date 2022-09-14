#include "PressEnemy.h"
#include "BulletMgr.h"
#include "EnemyHP.h"

PressEnemy::PressEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== コンストラクタ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

	float angleInterval = DirectX::XM_PI / 10.0f;
	for (auto& index : m_hpUI) {

		int indexCount = static_cast<int>(&index - &m_hpUI[0]);

		// 回転量
		float drawAngle = 0;

		// インデックスが10以上だったら。
		if (10 < indexCount) {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * GetDigits(indexCount, 0, 0);

		}
		else {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * static_cast<float>(indexCount);

		}

		index = std::make_shared<EnemyHP>(-drawAngle);

	}

}

void PressEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;

}

void PressEnemy::OnGenerate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== 生成処理 =====*/

	m_id = ID;
	m_defPos = Pos;
	m_pos = Pos;
	m_knockBackSpeed = 0.0f;
	m_knockBackVec = Vec3<float>();
	m_shotTimer = 0;
	m_returnDefPosSpeed = 0;
	m_returnDefTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = SCALE;
	m_hp = HP;
	m_transform.SetScale(m_scale);

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Init();

	}

}

void PressEnemy::OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	// ノックバックの移動。
	m_pos += m_knockBackVec * m_knockBackSpeed;

	// ノックバックの移動を減らす。
	--m_returnDefTimer;
	if (0 < m_knockBackSpeed) {
		m_knockBackSpeed -= m_knockBackSpeed / 20.0f;
	}

	// ノックバックの移動量が一定以下だったら初期位置に戻す。
	if (m_returnDefTimer < 0) m_returnDefTimer = 0;
	if (m_returnDefTimer <= 0 && RETURN_DEFPOS_SPEED <= Vec3<float>(m_pos - m_defPos).Length()) {

		// 初期位置までの座標。
		Vec3<float> defPosDir = Vec3<float>(m_defPos - m_pos).GetNormal();
		m_pos += defPosDir * m_returnDefPosSpeed;

		// 移動速度を規定値に近づける。
		m_returnDefPosSpeed += (RETURN_DEFPOS_SPEED - m_returnDefPosSpeed) / 20.0f;

	}
	else {

		m_returnDefPosSpeed = 0;

	}

	// 当たり判定
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// 射出処理
	Shot(BulletMgr, PlayerPos);

	// HPUIの更新処理
	for (auto& index : m_hpUI) {

		index->Invalidate();

	}
	if (10 < m_hp) {

		for (int index = 0; index < GetDigits(m_hp, 0, 0); ++index) {

			m_hpUI[index]->Activate();

		}

	}
	else {

		for (int index = 0; index < m_hp; ++index) {

			m_hpUI[index]->Activate();

		}

	}

	// HPUIの更新処理
	for (auto& index : m_hpUI) {

		index->Update(m_pos, SCALE);

	}

	m_transform.SetPos(m_pos);
}

#include"DrawFunc_Append.h"
void PressEnemy::OnDraw()
{

	/*===== 描画処理 =====*/

	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_modelHit, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Draw();

	}

}

void PressEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== 弾との当たり判定 =====*/

	// マップ外に出たら。
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {

			Init();

		}

	}

	int hitCount = 0;
	// プレイヤー弾との当たり判定。
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	// IDがTRACKだったらノックバックさせる。
	if (0 < hitCount && m_id == ENEMY_INFO::ID::PRESS) {

		m_knockBackVec = Vec3<float>(m_pos - PlayerPos).GetNormal();
		m_knockBackSpeed += ADD_KNOCK_BACK_SPEED;
		// ノックバックの移動量が最大値を超えないようにする。
		if (MAX_KNOCK_BACK_SPEED < m_knockBackSpeed) {

			m_knockBackSpeed = MAX_KNOCK_BACK_SPEED;

		}

		m_returnDefTimer = RETURN_DEF_TIMER;

	}

	// 弾に当たったかフラグ
	if (0 < hitCount) {

		m_hitEffectTimer = HIT_EFFECT_TIMER;

	}
	else {

		--m_hitEffectTimer;
		if (m_hitEffectTimer <= 0) {

			m_hitEffectTimer = 0;

		}

	}

}

void PressEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== 弾射出処理 =====*/

	if (!(m_id == ENEMY_INFO::ID::UNION)) return;

}
