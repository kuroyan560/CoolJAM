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
	m_hp = HP[static_cast<int>(ID)];
	m_scale = SCALE[static_cast<int>(ID)];
	m_speed = 0;
	m_transform.SetScale(m_scale);
	m_isActive = true;
	m_hitEffectTimer = 0;

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
		m_speed += (SPEED - m_speed) / 10.0f;

		// �ړ�������B
		m_pos += m_forwardVec * m_speed;

	}

	break;
	default:
		break;
	}

	// �����蔻��
	CheckHit(BulletMgr, MapSize);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);

}

//#include"DrawFunc3D.h"
#include"DrawFunc_Append.h"
void Enemy::Draw()
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc_Append::DrawModel(s_modelHit, m_transform);

	}
	else {

		DrawFunc_Append::DrawModel(s_model, m_transform);

	}

}

void Enemy::CheckHit(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	// �v���C���[�e�Ƃ̓����蔻��B
	int hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, SCALE[static_cast<int>(m_id)]);
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
	if (length < Size + EDGE_SCALE[static_cast<int>(m_id)]) {

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