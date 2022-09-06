#include "Enemy.h"
#include"Importer.h"
#include "Model.h"

void Enemy::Init()
{

	/*===== ���������� =====*/

	m_pos = Vec3<float>();
	m_forwardVec = Vec3<float>(0, 0, 1);
	m_scale = 1.0f;
	m_isActive = false;

	m_model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
	m_transform.SetScale(1.0f);

}

void Enemy::Generate(ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos)
{

	/*===== ���������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_forwardVec = Vec3<float>(PlayerPos - Pos).GetNormal();
	m_scale = SCALE[static_cast<int>(ID)];
	m_speed = 0;
	m_transform.SetScale(m_scale);
	m_isActive = true;

}

void Enemy::Update(const Vec3<float>& PlayerPos, const float& MapSize)
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
	CheckHit(MapSize);

}

#include"DrawFunc3D.h"
void Enemy::Draw(Camera& Cam)
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

}

void Enemy::CheckHit(const float& MapSize)
{

	/*===== �����蔻�� =====*/

	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

}
