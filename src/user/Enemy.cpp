#include "Enemy.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"

void Enemy::Init()
{

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>(0, 0, 1);
	m_scale = 1.0f;
	m_isActive = false;
	m_shotTimer = 0;

	m_model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
	m_transform.SetScale(1.0f);

}

void Enemy::Generate(ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos)
{

	/*===== ���������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = Vec3<float>(PlayerPos - Pos).GetNormal();
	m_scale = SCALE[static_cast<int>(ID)];
	m_speed = 0;
	m_transform.SetScale(m_scale);
	m_isActive = true;

}

void Enemy::Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	switch (m_id)
	{
	case Enemy::ID::STOPPING:
		break;
	case Enemy::ID::STRAIGHT:

		/*-- �ǔ��G --*/

		// �ړ����x���ԁB
		m_speed += (SPEED - m_speed) / 10.0f;

		// �ړ�������B
		m_pos += m_forwardVec * m_speed;


		break;
	case Enemy::ID::TRACKING:

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

#include"DrawFunc3D.h"
void Enemy::Draw(Camera& Cam)
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

}

void Enemy::CheckHit(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize)
{

	/*===== �����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	// �v���C���[�e�Ƃ̓����蔻��B
	int hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, SCALE[static_cast<int>(m_id)]);
	if (0 < hitCount) {

		Init();

	}

}

void Enemy::Shot(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos) {

	/*===== �e�ˏo���� =====*/

	++m_shotTimer;
	if (SHOT_TIMER < m_shotTimer) {

		BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(PlayerPos - m_pos).GetNormal());

		m_shotTimer = 0;

	}

}