#include "Enemy.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"

std::shared_ptr<Model> Enemy::s_model;
std::shared_ptr<Model> Enemy::s_modelHit;
bool Enemy::m_debugIsShotEnemy = true;

Enemy::Enemy() {

	/*===== コンストラクタ =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>(0, 0, 1);
	m_scale = 1.0f;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_shotTimer = 0;
	m_hp = 0;

	if (!s_model)
	{
		s_model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
		s_modelHit = Importer::Instance()->LoadModel("resource/user/", "enemy_hit.glb");
	}
	m_transform.SetScale(1.0f);

}

void Enemy::Init()
{

	/*===== 初期化処理 =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>(0, 0, 1);
	m_scale = 1.0f;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_shotTimer = 0;
	m_hp = 0;

	m_transform.SetScale(1.0f);

}

void Enemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== 初期化処理 =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_hp = HP[static_cast<int>(ID)];
	m_scale = SCALE[static_cast<int>(ID)];
	m_speed = 0;
	m_transform.SetScale(m_scale);
	m_isActive = true;
	m_hitEffectTimer = 0;

}

void Enemy::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

	switch (m_id)
	{
	case ENEMY_INFO::ID::STOPPING:
		break;
	case ENEMY_INFO::ID::STRAIGHT:
	case ENEMY_INFO::ID::PLAYER_STRAIGHT:

		/*-- 追尾敵 --*/

		// 移動速度を補間。
		m_speed += (SPEED - m_speed) / 10.0f;

		// 移動させる。
		m_pos += m_forwardVec * m_speed;


		break;
	case ENEMY_INFO::ID::TRACKING:

		/*-- 追尾敵 --*/

	{

		// 左右判定をする。
		float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
		if (cross != 0) {

			cross = cross < 0 ? -1.0f : 1.0f;
			float handleRot = TRACKING_ROT * cross;

			// 移動方向ベクトルを角度に直して値を加算する。
			float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
			forwardVecAngle += handleRot;

			// 加算した角度をベクトルに直す。
			m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

		}

		// 移動速度を補間。
		m_speed += (SPEED - m_speed) / 10.0f;

		// 移動させる。
		m_pos += m_forwardVec * m_speed;

	}

	break;
	default:
		break;
	}

	// 当たり判定
	CheckHit(BulletMgr, MapSize);

	// 射出処理
	Shot(BulletMgr, PlayerPos);

}

#include"DrawFunc3D.h"
void Enemy::Draw(Camera& Cam)
{

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc3D::DrawNonShadingModel(s_modelHit, m_transform, Cam);

	}
	else {

		DrawFunc3D::DrawNonShadingModel(s_model, m_transform, Cam);

	}

}

void Enemy::CheckHit(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize)
{

	/*===== 当たり判定 =====*/

	// マップ外判定。
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	// プレイヤー弾との当たり判定。
	int hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, SCALE[static_cast<int>(m_id)]);
	m_hp -= hitCount;
	if (m_hp <= 0) {

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

void Enemy::Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos) {

	/*===== 弾射出処理 =====*/

	if (!m_debugIsShotEnemy) return;

	++m_shotTimer;
	if (SHOT_TIMER < m_shotTimer) {

		BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(PlayerPos - m_pos).GetNormal());

		m_shotTimer = 0;

	}

}

bool Enemy::CheckIsEdge(const Vec3<float>& Pos, const float& Size) {

	/*===== 近くにいるかどうか =====*/

	// 二点間の距離
	float length = Vec3<float>(Pos - m_pos).Length();

	// エッジ用の円との当たり判定。
	if (length < Size + EDGE_SCALE[static_cast<int>(m_id)]) {

		return true;

	}

	return false;

}

void Enemy::Damage() {

	/*===== ダメージを与える =====*/

	--m_hp;
	if (m_hp <= 0) {

		Init();

	}

}