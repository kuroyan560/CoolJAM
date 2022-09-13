#include "PlayerStraightEnemy.h"
#include "EnemyHP.h"
#include "BulletMgr.h"

PlayerStraightEnemy::PlayerStraightEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
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

void PlayerStraightEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;

}

void PlayerStraightEnemy::OnGenerate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== 生成処理 =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
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

void PlayerStraightEnemy::OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	// 移動速度を補間。
	m_speed += (SPEED - m_speed) / 10.0f;

	// 移動させる。
	m_pos += m_forwardVec * m_speed;

	// 当たり判定
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// 射出処理
	Shot(BulletMgr, PlayerPos);

	// マップ外に出たら。
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {

			Init();

		}

	}

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

}

#include"DrawFunc_Append.h"
void PlayerStraightEnemy::Draw()
{

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
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

void PlayerStraightEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== 弾との当たり判定 =====*/

	// マップ外判定。
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	int hitCount = 0;
	// プレイヤー弾との当たり判定。
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	// 弾に当たったかフラグ
	if (0 < hitCount) {

		Damage(1, BulletMgr);

		m_hitEffectTimer = HIT_EFFECT_TIMER;

	}
	else {

		--m_hitEffectTimer;
		if (m_hitEffectTimer <= 0) {

			m_hitEffectTimer = 0;

		}

	}

}

void PlayerStraightEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== 弾を撃つ処理 =====*/

	//if (!(m_id == ENEMY_INFO::ID::UNION)) return;

	//++m_shotTimer;
	//if (SHOT_TIMER < m_shotTimer) {

	//	// 集合体の周りから弾を出す。
	//	for (auto& index : m_unionEnemy) {

	//		if (!index->GetIsActive()) continue;

	//		BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(index->GetPos() - m_pos).GetNormal());

	//	}

	//	m_shotTimer = 0;

	//}

}
