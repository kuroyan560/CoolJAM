#include "PressEnemy.h"
#include "BulletMgr.h"

PressEnemy::PressEnemy(std::shared_ptr<Model> DefModel, std::shared_ptr<Model> DamageModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = DefModel;
	m_modelHit = DamageModel;
	m_isActive = false;

}

void PressEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;

}

void PressEnemy::Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec)
{

	/*===== �������� =====*/

	m_id = ID;
	m_defPos = Pos;
	m_pos = Pos;
	m_knockBackSpeed = 0.0f;
	m_knockBackVec = Vec3<float>();
	m_shotTimer = 0;
	m_returnDefPosSpeed = 0;
	m_returnDefTimer = 0;
	m_forwardVec = ForwardVec;
	m_speed = 0;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_scale = PRESS_SCALE;
	m_hp = PRESS_HP;
	m_transform.SetScale(m_scale);

}

void PressEnemy::Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize)
{

	/*===== �X�V���� =====*/

	// �m�b�N�o�b�N�̈ړ��B
	m_pos += m_knockBackVec * m_knockBackSpeed;

	// �m�b�N�o�b�N�̈ړ������炷�B
	--m_returnDefTimer;
	if (0 < m_knockBackSpeed) {
		m_knockBackSpeed -= m_knockBackSpeed / 20.0f;
	}

	// �m�b�N�o�b�N�̈ړ��ʂ����ȉ��������珉���ʒu�ɖ߂��B
	if (m_returnDefTimer < 0) m_returnDefTimer = 0;
	if (m_returnDefTimer <= 0 && RETURN_DEFPOS_SPEED <= Vec3<float>(m_pos - m_defPos).Length()) {

		// �����ʒu�܂ł̍��W�B
		Vec3<float> defPosDir = Vec3<float>(m_defPos - m_pos).GetNormal();
		m_pos += defPosDir * m_returnDefPosSpeed;

		// �ړ����x���K��l�ɋ߂Â���B
		m_returnDefPosSpeed += (RETURN_DEFPOS_SPEED - m_returnDefPosSpeed) / 20.0f;

	}
	else {

		m_returnDefPosSpeed = 0;

	}

	// �����蔻��
	CheckHitBullet(BulletMgr, MapSize, PlayerPos);

	// �ˏo����
	Shot(BulletMgr, PlayerPos);

}

#include "DrawFunc3D.h"
void PressEnemy::Draw(Camera& Cam)
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

void PressEnemy::CheckHitBullet(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const Vec3<float>& PlayerPos)
{

	/*===== �e�Ƃ̓����蔻�� =====*/

	// �}�b�v�O�ɏo����B
	if (MapSize <= m_pos.Length()) {

		m_pos = m_pos.GetNormal() * MapSize;

		--m_hp;
		if (m_hp <= 0) {

			Init();

		}

	}

	int hitCount = 0;
	// �v���C���[�e�Ƃ̓����蔻��B
	Vec3<float> hitBulletPos;
	hitCount = BulletMgr.lock()->CheckHitPlayerBullet(m_pos, m_scale, hitBulletPos);

	// ID��TRACK��������m�b�N�o�b�N������B
	if (0 < hitCount && m_id == ENEMY_INFO::ID::PRESS) {

		m_knockBackVec = Vec3<float>(m_pos - PlayerPos).GetNormal();
		m_knockBackSpeed += ADD_KNOCK_BACK_SPEED;
		// �m�b�N�o�b�N�̈ړ��ʂ��ő�l�𒴂��Ȃ��悤�ɂ���B
		if (MAX_KNOCK_BACK_SPEED < m_knockBackSpeed) {

			m_knockBackSpeed = MAX_KNOCK_BACK_SPEED;

		}

		m_returnDefTimer = RETURN_DEF_TIMER;

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

void PressEnemy::Shot(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos)
{

	/*===== �e�ˏo���� =====*/

	if (!(m_id == ENEMY_INFO::ID::UNION)) return;

}
