#include "TorusMoveEnemy.h"
#include "EnemyHP.h"
#include "BulletMgr.h"

TorusMoveEnemy::TorusMoveEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
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

void TorusMoveEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void TorusMoveEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
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

void TorusMoveEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	// ���S����̋���
	float centerDistance = m_pos.Length();

	// �ړ�����O�̍��W
	Vec3<float> prevPos = m_pos;

	// �ړ�������B
	m_speed = SPEED;
	m_pos += m_forwardVec * m_speed;

	// �����߂��B
	if (centerDistance <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * centerDistance;

	}

	// ���ʃx�N�g������]�B
	m_forwardVec = Vec3<float>(m_pos - prevPos).GetNormal();

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

}

#include"DrawFunc_Append.h"
void TorusMoveEnemy::Draw()
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

	// �G��HP�̔|����`��
	for (auto& index : m_hpUI) {

		index->Draw();

	}

}

void TorusMoveEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �e�Ƃ̓����蔻�� =====*/

	// �}�b�v�O����B
	if (MapSize + MapSize / 2.0f <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		Init();

	}

	int hitCount = 0;
	// �v���C���[�e�Ƃ̓����蔻��B
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	// �e�ɓ����������t���O
	if (0 < hitCount) {

		Damage(1, BulletMgr);

		m_hitEffectTimer = HIT_EFFECT_TIMER;

	}
	else {

		--m_hitEffectTimer;
		if (m_hitEffectTimer <= 0) {

			m_hitEffectTimer = 0;

		}

	}

}

void TorusMoveEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== �e�������� =====*/

	//if (!(m_id == ENEMY_INFO::ID::UNION)) return;

	//++m_shotTimer;
	//if (SHOT_TIMER < m_shotTimer) {

	//	// �W���̂̎��肩��e���o���B
	//	for (auto& index : m_unionEnemy) {

	//		if (!index->GetIsActive()) continue;

	//		BulletMgr.lock()->GenerateEnemyBullet(m_pos, Vec3(index->GetPos() - m_pos).GetNormal());

	//	}

	//	m_shotTimer = 0;

	//}

}
