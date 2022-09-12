#include "ElecMushiEnemy.h"
#include "BulletMgr.h"
#include "../engine/Importer.h"
#include "EnemyHP.h"

ElecMushiEnemy::ElecMushiEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
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

void ElecMushiEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;

}

void ElecMushiEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== 生成処理 =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_sineWaveTimer = 0;
	m_scale = SCALE;
	m_hp = HP;
	m_transform.SetScale(m_scale);
	m_defLength = m_pos.Length();
	m_sineWaveLength = 0;

}

void ElecMushiEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	// SIN波で動いた量を打ち消す。
	m_pos -= m_pos.GetNormal() * m_sineWaveLength;

	// 移動する前の座標
	Vec3<float> prevPos = m_pos;

	// 移動させる。
	m_speed = SPEED;
	m_pos += m_forwardVec * m_speed;

	// 押し戻す。
	if (m_defLength <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * m_defLength;

	}

	// 正面ベクトルを回転。
	m_forwardVec = Vec3<float>(m_pos - prevPos).GetNormal();


	// Sine波を更新。
	m_sineWaveTimer += ADD_SINE_WAVE_TIMER;
	float sineWave = sinf(m_sineWaveTimer);
	m_sineWaveLength = sineWave * SINE_WAVE_LENGTH;

	// Sine波の動きも考慮した位置に配置。
	m_pos = m_pos.GetNormal() * (m_sineWaveLength + m_defLength);

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

}

#include"DrawFunc3D.h"
#include"DrawFunc_Append.h"
void ElecMushiEnemy::Draw()
{

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_modelHit, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transfsorm, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Draw();

	}

}

void ElecMushiEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== 弾との当たり判定 =====*/

	// マップ外に出たら。
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {


			// エレキ虫が死んだ。
			BulletMgr.lock()->KillElecMushi();
			Init();

		}

	}

	int hitCount = 0;
	// プレイヤー弾との当たり判定。
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	Damage(1, BulletMgr);

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

void ElecMushiEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
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
