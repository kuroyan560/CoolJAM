#include "ElecMushiEnemy.h"
#include "BulletMgr.h"

ElecMushiEnemy::ElecMushiEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== コンストラクタ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

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
	m_scale = PLAYER_STRAIGHT_SCALE;
	m_hp = PLAYER_STRAIGHT_HP;
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


}

#include"DrawFunc3D.h"
void ElecMushiEnemy::Draw(Camera& Cam)
{

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);

	}
	else {

		DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

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

	m_hp -= hitCount;
	if (m_hp <= 0) {

		// エレキ虫が死んだ。
		BulletMgr.lock()->KillElecMushi();

		Init();

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
