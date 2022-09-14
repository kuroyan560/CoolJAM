#include "UnionBaseEnemy.h"
#include "BulletMgr.h"
#include "UnionEnemy.h"
#include "EnemyHP.h"
#include "SlowMgr.h"

UnionBaseEnemy::UnionBaseEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== コンストラクタ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

	// UNIONの敵の周りの敵を生成。
	for (auto& index : m_unionEnemy) {

		index = std::make_shared<UnionEnemy>(m_model, m_modelHit);

	}

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

void UnionBaseEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;

	for (auto& index : m_unionEnemy) {

		index->Init();

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

void UnionBaseEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== 生成処理 =====*/

	m_id = ID;
	m_pos = Pos;
	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0.00001f;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = SCALE;
	m_hp = HP;
	m_transform.SetScale(m_scale);

	// 周りの敵を生成。
	const int UNION_COUNT = static_cast<int>(m_unionEnemy.size());
	const float UNION_ROTATE = DirectX::XM_2PI / static_cast<float>(UNION_COUNT);
	float nowRotate = UNION_ROTATE;
	float moveScale = m_scale + m_unionEnemy[0]->GetScale();
	for (int index = 0; index < UNION_COUNT; ++index) {

		m_unionEnemy[index]->Generate(m_pos + Vec3<float>(cosf(nowRotate) * moveScale, 0, sinf(nowRotate) * moveScale));
		nowRotate += UNION_ROTATE;

	}

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Init();

	}

}

void UnionBaseEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	for (auto& index : m_unionEnemy) {

		if (!index->GetIsActive()) continue;

		index->Update();

	}

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
}

#include"DrawFunc_Append.h"
void UnionBaseEnemy::Draw()
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

	for (auto& index : m_unionEnemy) {

		if (!index->GetIsActive()) continue;

		index->Draw();

	}

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Draw();

	}

}

void UnionBaseEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== 当たり判定 =====*/

	// マップ外判定。
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	// 集合体との当たり判定を行う。
	int hitCount = 0;
	int activeCount = 0;
	for (auto& index : m_unionEnemy) {

		if (!index->GetIsActive()) continue;

		// 当たり判定を行う。
		Vec3<float> hitBulletPos;
		int hitCount = BulletMgr.lock()->CheckHitPlayerBullet(index->GetPos(), index->GetScale(), hitBulletPos);
		index->Damage(hitCount);

		++activeCount;

	}

	// 本体との当たり判定
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);
	if (activeCount <= 0) {

		Damage(1, BulletMgr);

	}

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

void UnionBaseEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== 弾射出処理 =====*/

	++m_shotTimer;
	if (SHOT_TIMER < m_shotTimer) {

		// 集合体の周りから弾を出す。
		for (auto& index : m_unionEnemy) {

			if (!index->GetIsActive()) continue;

			BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(index->GetPos() - m_pos).GetNormal());

		}

		m_shotTimer = 0;

	}

}
