#include "Enemy.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"

std::shared_ptr<Model> Enemy::s_model;
std::shared_ptr<Model> Enemy::s_modelHit;
bool Enemy::m_debugIsShotEnemy = true;

Enemy::Enemy() {

	/*===== �R���X�g���N�^ =====*/

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

	/*===== ���������� =====*/

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

	/*===== ���������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;

	switch (m_id)
	{
	case ENEMY_INFO::ID::STOPPING:
		m_scale = STOPPING_SCALE;
		m_hp = STOPPING_HP;
		break;
	case ENEMY_INFO::ID::STRAIGHT:
		m_scale = STRAIGHT_SCALE;
		m_hp = STRAIGHT_HP;
		break;
	case ENEMY_INFO::ID::PLAYER_STRAIGHT:
		m_scale = PLAYER_STRAIGHT_SCALE;
		m_hp = PLAYER_STRAIGHT_HP;
		break;
	case ENEMY_INFO::ID::TRACKING:
		m_scale = TRACKING_SCALE;
		m_hp = TRACKING_HP;
		break;
	case ENEMY_INFO::ID::SHIELD:
		m_scale = SHIELD_SCALE;
		m_hp = SHIELD_HP;
		break;
	default:
		// �p�����[�^���ݒ肳��Ă��Ȃ��ł��I
		assert(0);
		break;
	}

	m_transform.SetScale(m_scale);

}

void Enemy::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	switch (m_id)
	{
	case ENEMY_INFO::ID::STOPPING:
		break;
	case ENEMY_INFO::ID::STRAIGHT:
	case ENEMY_INFO::ID::PLAYER_STRAIGHT:

		/*-- �ǔ��G --*/

		// �ړ����x���ԁB
		m_speed += (SPEED - m_speed) / 10.0f;

		// �ړ�������B
		m_pos += m_forwardVec * m_speed;


		break;
	case ENEMY_INFO::ID::TRACKING:

		/*-- �ǔ��G --*/

	{

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
		m_speed += (TRACKING_SPEED - m_speed) / 10.0f;

		// �ړ�������B
		m_pos += m_forwardVec * m_speed;

	}

	break;
	case ENEMY_INFO::ID::SHIELD:

		/*-- �������G --*/
	{

		// ���E���������B
		float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
		if (cross != 0) {

			cross = cross < 0 ? -1.0f : 1.0f;
			float handleRot = SHIELD_ROT * cross;

			// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
			float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
			forwardVecAngle += handleRot;

			// ���Z�����p�x���x�N�g���ɒ����B
			m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

		}

		// �`�����]������B
		float angle = atan2f(m_forwardVec.x, m_forwardVec.z);
		m_transform.SetRotate(DirectX::XMMatrixRotationY(angle));

	}

	default:
		break;
	}

	// �����蔻��
	CheckHitBullet(BulletMgr, MapSize);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);

}

#include"DrawFunc3D.h"
void Enemy::Draw(Camera& Cam)
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc3D::DrawNonShadingModel(s_modelHit, m_transform, Cam);

	}
	else {

		DrawFunc3D::DrawNonShadingModel(s_model, m_transform, Cam);

	}

}

void Enemy::CheckHitBullet(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	int hitCount = 0;
	// �������̓G��������E
	if (m_id == ENEMY_INFO::ID::SHIELD) {

		// �v���C���[�e�Ƃ̓����蔻��B
		hitCount = BulletMgr.lock()->CheckHitPlayerBulletAngle(m_pos, m_scale, m_forwardVec, 0.0f);
		m_hp -= hitCount;
		if (m_hp <= 0) {

			Init();

		}

	}
	else {

		// �v���C���[�e�Ƃ̓����蔻��B
		hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale);
		m_hp -= hitCount;
		if (m_hp <= 0) {

			Init();

		}

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

void Enemy::Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos) {

	/*===== �e�ˏo���� =====*/

	if (!m_debugIsShotEnemy) return;

	++m_shotTimer;
	if (SHOT_TIMER < m_shotTimer) {

		BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(PlayerPos - m_pos).GetNormal());

		m_shotTimer = 0;

	}

}

bool Enemy::CheckIsEdge(const Vec3<float>& Pos, const float& Size) {

	/*===== �߂��ɂ��邩�ǂ��� =====*/

	// ��_�Ԃ̋���
	float length = Vec3<float>(Pos - m_pos).Length();

	// �G�b�W�p�̉~�Ƃ̓����蔻��B
	if (length < Size + EDGE) {

		return true;

	}

	return false;

}

bool Enemy::CheckHit(const Vec3<float>& Pos, const float& Size)
{

	/*===== �����蔻�� =====*/

	// ��_�Ԃ̋���
	float length = Vec3<float>(Pos - m_pos).Length();

	if (length < Size + m_scale) {

		return true;

	}

	return false;

}

void Enemy::Damage() {

	/*===== �_���[�W��^���� =====*/

	--m_hp;
	if (m_hp <= 0) {

		Init();

	}

}