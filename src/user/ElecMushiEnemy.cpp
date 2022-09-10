#include "ElecMushiEnemy.h"
#include "BulletMgr.h"

ElecMushiEnemy::ElecMushiEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

}

void ElecMushiEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void ElecMushiEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

	m_id = ID;
	m_pos = Pos;
	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0.00001f;
	m_shotTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = PLAYER_STRAIGHT_SCALE;
	m_hp = PLAYER_STRAIGHT_HP;
	m_transform.SetScale(m_scale);

}

void ElecMushiEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
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

	// �m�b�N�o�b�N�̈ړ��B
	m_pos += m_knockBackVec * m_knockBackSpeed;

	// �m�b�N�o�b�N�̈ړ������炷�B
	m_knockBackSpeed -= m_knockBackSpeed / 20.0f;

	// �����蔻��
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);


}

#include"DrawFunc3D.h"
void ElecMushiEnemy::Draw(Camera& Cam)
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc3D::DrawNonShadingModel(m_modelHit, m_transform, Cam);

	}
	else {

		DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

	}

}

void ElecMushiEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �e�Ƃ̓����蔻�� =====*/

	// �}�b�v�O�ɏo����B
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {


			// �G���L�������񂾁B
			BulletMgr.lock()->KillElecMushi();
			Init();

		}

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

		// �G���L�������񂾁B
		BulletMgr.lock()->KillElecMushi();

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

void ElecMushiEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
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
