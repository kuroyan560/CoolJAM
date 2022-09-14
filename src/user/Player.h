#pragma once
#include"Transform.h"
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"PlayerDashLighting.h"
#include"PlayerModelOutline.h"

class ModelObject;
class Camera;
class BulletMgr;
class EnemyMgr;
class DriftParticle;
class PlayerHP;

class Player
{

private:

	/*===== �����o�ϐ� =====*/

	// ��{�I�ȕϐ�
	Vec3<float> m_pos;			// ���ݍ��W
	Vec3<float> m_prevPos;		// �O�t���[���̍��W
	Vec3<float> m_inputVec;		// ���͂��ꂽ�����x�N�g��(�ړ������x�N�g�������̕����ɕ⊮����B)
	Vec3<float> m_prevForwardVec;// �O�t���[���̈ړ������x�N�g���B
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	DirectX::XMMATRIX m_rotation;	//�N�H�[�^�j�I��
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(1.0f, 0.0f, 0.0f);
	float m_speed;				// �ړ����x
	const float SCALE = 1.0f;
	const float BOOST_SCALE = 20.0f;
	bool m_isEdge;				// ���ɂ��邩
	float m_brakeBoostTimer;
	const int MAX_BRAKE_BOOST_TIMER = 120.0f;

	// �}�E�X���W�ɐݒu���郂�f���B
	std::shared_ptr<ModelObject>m_mousePointer;
	float m_mousePointerScale;
	const float MOUSE_POINTER_SCALE = 5.0f;

	// ����������
	float m_movedLength;

	// �ǍۂŃ_���[�W���󂯂�͈́B
	const float MAP_EDGE = 20.0f;

	// �t�B�[�o�[��Ԃ�
	bool m_isPrevFever;
	bool m_isFever;
	float m_feverTime;
	const int FEVER_TIME = 600;
	const int FEVER_TIME_GAME_TIMER = 10;

	// HP�֌W
	int m_hp;		// �v���C���[��HP
	static const int MAX_HP = 10;
	int m_RedTime;		// HP���������ۂ̐F�ω��Ɏg�p����^�C�}�[�B
	const std::array<int, 4> RED_TIME = { 0,0,60,120 };	// [0]�͎g�p���Ȃ��B
	float m_colorEasingTimer;	// HP���������ۂ̐F�ω��̃C�[�W���O�Ɏg�p����^�C�}�[
	const std::array<float, 4> ADD_COLOR_EASING_TIMER = { 0.2f, 0.1f, 0.08f, 0.01f };	// [0]�͎g�p���Ȃ��B
	bool m_isRed;			// ���̐F�̏�ԁB
	bool m_isChangeRed;		// �ԂɕύX���邩
	bool m_isChangeGreen;	// �΂ɕύX���邩�B

	std::array<std::shared_ptr<PlayerHP>, MAX_HP> m_hpUI;


	// �_���[�W��
	float m_damageEffectTimer;	// �_���[�W�G�t�F�N�g�̓_�łɎg�p����^�C�}�[
	const int DAMAGE_EFFECT_DRAW_CHANGE_SPAN = 4;
	int m_damageEffectCount;	// �_�ł̉�
	const int DAMAGE_EFFECT_COUNT = 3;
	bool m_isDamageEffect;		// �_���[�W�G�t�F�N�g����
	bool m_isDamageEffectDrawPlayer;

	// �`���[�g���A���p�B
	float m_dashCounter;

	// �h���t�g
	std::array<std::shared_ptr<DriftParticle>, 128> m_driftParticle;
	int m_driftParticleDelay;
	const int DRIFT_PARTICLE_DELAY = 5;

	// Z����]��
	float m_rotX;

	// �e�֌W
	float m_shotTimer;
	const float SHOT_TIMER = 5;

	// �u���[�L�֌W
	int m_brakeTimer;			// �u���[�L���Ă��鎞�ԁB
	const int MAX_BRAKE_TIMER = 60.0f;
	bool m_isBrake;				// �u���[�L���Ă��邩�ǂ����B

	//���f��
	std::shared_ptr<ModelObject>m_modelObj;

	//�A�E�g���C���p�̃��f��
	float inputATan2f;
	const Vec3<float> GREEN_HSV = Vec3<float>(152.857f, 223.125f, 0.959f);
	const Vec3<float> RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.879f);
	const Vec3<float> DARK_RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.289f);

	PlayerModelOutline m_outlineModel;
	std::unique_ptr<PlayerDashLighting> m_dashLight;

	int m_damageSE;

public:

	float MIN_SPEED = 0.7f;	// �ŏ��̈ړ����x
	float MAX_SPEED = 2.5f;	// �ő�̈ړ����x
	float BRAKE_SPEED = 0.7f;

	/*===== �����o�֐� =====*/

	Player();
	void Init();
	void Finalize();
	void Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope, bool IsStopFeverTimer = false, bool IsNoInput = false);
	void Draw(Camera& Cam, const bool& IsTitle = false);
	void DrawDebugInfo(Camera& Cam);

	Vec3<float> GetPos() { return m_pos; }
	Vec3<float> GetForwardVec() { return m_forwardVec; }
	Vec3<float> GetMovedVec() { return Vec3<float>(m_pos - m_prevPos).GetNormal(); }
	int GetBrakeBoostTimer() { return m_brakeBoostTimer; }
	float GetMaxFeverTime() { return FEVER_TIME; }
	float GetMaxFeverTimeGameTimer() { return FEVER_TIME_GAME_TIMER; }
	bool GetIsFever() { return m_isFever; }
	float GetPlayerFeverRate() { return static_cast<float>(m_feverTime) / static_cast<float>(FEVER_TIME); }
	float GetMovedLength() { return m_movedLength; }
	void ClearMovedLength() { m_movedLength = 0; }
	int GetDashCount() { return m_dashCounter; }
	void ResetDashCount() { m_dashCounter = 0; }

	Vec3<float>* GetPosPtr() { return &m_pos; };
	const float* GetInputRadianPtr() { return &inputATan2f; };
	float GetInputRadian() { return inputATan2f; };
	bool GetIsFeverTrigger() { return !m_isPrevFever && m_isFever; }
	bool GetIsFeverEndTrigger() { return m_isPrevFever && !m_isFever; }

private:

	// ���͏���
	void Input(Camera& Cam, const Vec2<float>& WindowSize);

	// �ړ�����
	void Move(std::weak_ptr<BulletMgr> BulletMgr, bool IsStopFeverTimer);

	// �G�t�F�N�g�S�ʂ̍X�V����
	void UpdateEffect();

	// �e�����B
	void Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr);

	// �����蔻��
	void CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope);

	// HP��1���炷�B
	void Damage();

	// 01�ɔ[�߂�B
	float Saturate(const float& Value);

	// �g�����X�t�H�[���Q�b�^
	const Transform& GetTransform()const { return m_modelObj->m_transform; }

	// �h���t�g�̃p�[�e�B�N���𐶐��B
	void GenerateDriftParticle(const float& NowAngle, const float& Cross);


	// �ԑ̂̐F��ς���B
	bool ChangeBodyColorEasing(const float& AddEasingAmount, EASING_TYPE EasingType, EASE_CHANGE_TYPE EaseChangeType, const Vec3<float>& StartColor, const Vec3<float>& EndColor, const bool& IsEaseX);

	// �w��̌��̐������擾�B
	inline int GetDigits(int Value, int M, int N) {
		int mod_value;
		int result;

		/* n���ڈȉ��̌����擾 */
		mod_value = Value % (int)pow(10, N + 1);

		/* m���ڈȏ�̌����擾 */
		result = mod_value / pow(10, M);

		return result;

	}

	// �}�E�X�̂�[��Ǎ��W�����߂�B
	bool Player::CheckHitRayToStagePolygon(const Vec3<float>& RayStartPos, const Vec3<float>& RayDir, Vec3<float>& HitPos)
	{

		// ���ʂ̒��_
		Vec3<float> p1 = Vec3<float>(10000, 0, 10000);
		Vec3<float> p2 = Vec3<float>(-10000, 0, -10000);
		Vec3<float> p3 = Vec3<float>(10000, 0, -10000);

		// ���C�̊J�n�n�_���畽�ʂɂ��낵�������̒��������߂�
		Vec3<float> planeNorm = Vec3<float>(0, 1, 0);
		float rayToOriginLength = RayStartPos.Dot(planeNorm);
		float planeToOriginLength = p1.Dot(planeNorm);
		// ���_���畽�ʂɂ��낵�������̒���
		float perpendicularLine = rayToOriginLength - planeToOriginLength;

		// �O�p�֐��𗘗p���Ď��_����Փ˓X�܂ł̋��������߂�
		float dist = planeNorm.Dot(RayDir);
		float impDistance = perpendicularLine / -dist;

		// �Փ˒n�_
		HitPos = RayStartPos + RayDir * impDistance;

		return true;
	}


};

