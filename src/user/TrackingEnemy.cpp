#include "TrackingEnemy.h"
#include "BulletMgr.h"

TrackingEnemy::TrackingEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== コンストラクタ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

}

void TrackingEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;

}

void TrackingEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== 生成処理 =====*/

	m_id = ID;
	m_pos = Pos;
	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0.00001f;
	m_shotTimer = 0;
	m_forwardVec = Vec3<float>(PlayerPos - m_pos).GetNormal();
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = TRACKING_SCALE;
	m_hp = TRACKING_HP;
	m_transform.SetScale(m_scale);

}

void TrackingEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== 更新処理 =====*/

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

	// ノックバックの移動。
	m_pos += m_knockBackVec * m_knockBackSpeed;

	// ノックバックの移動を減らす。
	m_knockBackSpeed -= m_knockBackSpeed / 20.0f;

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

#include "DrawFunc3D.h"
void TrackingEnemy::Draw(Camera& Cam)
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

void TrackingEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== 当たり判定 =====*/

	// マップ外判定。
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	int hitCount = 0;
	// プレイヤー弾との当たり判定。
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	// ノックバックさせる。
	if (0 < hitCount) {

		m_knockBackVec = Vec3<float>(m_pos - PlayerPos).GetNormal();
		m_knockBackSpeed = KNOCK_BACK_SPEED;

	}

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

void TrackingEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== 弾射出処理 =====*/

	if (!(m_id == ENEMY_INFO::ID::UNION)) return;

}
