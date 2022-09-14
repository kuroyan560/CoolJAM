#include "ShieldEnemy.h"
#include "BulletMgr.h"
#include "EnemyHP.h"
#include "SlowMgr.h"

ShieldEnemy::ShieldEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

	float angleInterval = DirectX::XM_PI / 10.0f;
	for (auto& index : m_hpUI) {

		int indexCount = static_cast<int>(&index - &m_hpUI[0]);

		// ��]��
		float drawAngle = 0;

		// �C���f�b�N�X��10�ȏゾ������B
		if (10 < indexCount) {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * GetDigits(indexCount, 0, 0);

		}
		else {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * static_cast<float>(indexCount);

		}

		index = std::make_shared<EnemyHP>(-drawAngle);

	}

}

void ShieldEnemy::OnInit()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void ShieldEnemy::OnGenerate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = SCALE;
	m_hp = HP;
	m_transform.SetScale(m_scale);

	// �G��HP�̔|����`��
	for (auto& index : m_hpUI) {

		index->Init();

	}

}

void ShieldEnemy::OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	// ���E���������B
	float cross = m_forwardVec.Cross(PlayerPos - m_pos).y;
	if (cross != 0) {

		cross = cross < 0 ? -1.0f : 1.0f;
		float handleRot = SHIELD_ROT * cross;

		// �ړ������x�N�g�����p�x�ɒ����Ēl�����Z����B
		float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		forwardVecAngle += handleRot * SlowMgr::Instance()->m_slow;

		// ���Z�����p�x���x�N�g���ɒ����B
		m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

	}

	// �`�����]������B
	float angle = atan2f(m_forwardVec.x, m_forwardVec.z);
	m_transform.SetRotate(DirectX::XMMatrixRotationY(angle));

	// �����蔻��
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);

	// �}�b�v�O�ɏo����B
	CheckHitMapEdge(MapSize, BulletMgr);

	// HPUI�̍X�V����
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

	// HPUI�̍X�V����
	for (auto& index : m_hpUI) {

		index->Update(m_pos, SCALE);

	}

	m_transform.SetPos(m_pos);

}

#include"DrawFunc_Append.h"
void ShieldEnemy::OnDraw()
{

	/*===== �`�揈�� =====*/

	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_modelHit, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

	// �G��HP�̔|����`��
	for (auto& index : m_hpUI) {

		index->Draw();

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

		if (hitCount)Damage(1, BulletMgr);

	}

}

void ShieldEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== �e�ˏo���� =====*/

	ShotBullet(BulletMgr, PlayerPos);

}
