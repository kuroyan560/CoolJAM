#include "TrackingEnemy.h"
#include "BulletMgr.h"

TrackingEnemy::TrackingEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

}

void TrackingEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void TrackingEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

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

	/*===== �X�V���� =====*/

	// ���E���������B
	float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
	if (cross != 0) {

		cross = cross < 0 ? -1.0f : 1.0f;
		float handleRot = TRACKING_ROT * cross;

		// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
		float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		forwardVecAngle += handleRot;

		// ���Z�����p�x���x�N�g���ɒ����B
		m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

	}

	// �ړ����x���ԁB
	m_speed += (SPEED - m_speed) / 10.0f;

	// �ړ�������B
	m_pos += m_forwardVec * m_speed;

	// �m�b�N�o�b�N�̈ړ��B
	m_pos += m_knockBackVec * m_knockBackSpeed;

	// �m�b�N�o�b�N�̈ړ������炷�B
	m_knockBackSpeed -= m_knockBackSpeed / 20.0f;

	// �����蔻��
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);

	// �}�b�v�O�ɏo����B
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {

			Init();

		}

	}

}

#include"DrawFunc_Append.h"
void TrackingEnemy::Draw()
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_modelHit, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

}

void TrackingEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	int hitCount = 0;
	// �v���C���[�e�Ƃ̓����蔻��B
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	// �m�b�N�o�b�N������B
	if (0 < hitCount) {

		m_knockBackVec = Vec3<float>(m_pos - PlayerPos).GetNormal();
		m_knockBackSpeed = KNOCK_BACK_SPEED;

	}

	m_hp -= hitCount;
	if (m_hp <= 0) {

		Init();

	}

	// �e�ɓ����������t���O
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

	/*===== �e�ˏo���� =====*/

	if (!(m_id == ENEMY_INFO::ID::UNION)) return;

}