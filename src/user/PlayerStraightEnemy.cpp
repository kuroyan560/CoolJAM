#include "PlayerStraightEnemy.h"
#include "BulletMgr.h"

PlayerStraightEnemy::PlayerStraightEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

}

void PlayerStraightEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void PlayerStraightEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = PLAYER_STRAIGHT_SCALE;
	m_hp = PLAYER_STRAIGHT_HP;
	m_transform.SetScale(m_scale);

}

void PlayerStraightEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

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
void PlayerStraightEnemy::Draw()
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

void PlayerStraightEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
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

void PlayerStraightEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
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