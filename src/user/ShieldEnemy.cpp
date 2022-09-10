#include "ShieldEnemy.h"
#include "BulletMgr.h"

ShieldEnemy::ShieldEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

}

void ShieldEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void ShieldEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = SHIELD_SCALE;
	m_hp = SHIELD_HP;
	m_transform.SetScale(m_scale);

}

void ShieldEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

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

	// �ړ����x���ԁB
	m_speed += (SPEED - m_speed) / 10.0f;

	// �ړ�������B
	m_pos += m_forwardVec * m_speed;

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
void ShieldEnemy::Draw()
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

void ShieldEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �e�Ƃ̓����蔻�� =====*/

	int hitCount = 0;
	// �������̓G��������B
	if (m_id == ENEMY_INFO::ID::SHIELD) {

		// �v���C���[�e�Ƃ̓����蔻��B
		hitCount = BulletMgr.lock()->CheckHitPlayerBulletAngle(m_pos, m_scale, m_forwardVec, 0.0f);
		m_hp -= hitCount;
		if (m_hp <= 0) {

			Init();

		}

	}

}

void ShieldEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== �e�ˏo���� =====*/

	if (!(m_id == ENEMY_INFO::ID::UNION)) return;

}
