#pragma once
#include"Transform.h"
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>
#include"PlayerDashLighting.h"
#include"PlayerModelOutline.h"

class Model;
class Camera;
class BulletMgr;
class EnemyMgr;
class DriftParticle;

class Player
{

private:

	/*===== �����o�ϐ� =====*/

	// �g�����X�t�H�[��
	Transform m_transform;

	// ��{�I�ȕϐ�
	Vec3<float> m_pos;			// ���ݍ��W
	Vec3<float> m_prevPos;		// �O�t���[���̍��W
	Vec3<float> m_inputVec;		// ���͂��ꂽ�����x�N�g��(�ړ������x�N�g�������̕����ɕ⊮����B)
	Vec3<float> m_prevForwardVec;// �O�t���[���̈ړ������x�N�g���B
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	DirectX::XMMATRIX m_rotation;	//�N�H�[�^�j�I��
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(0.0f, 0.0f, 1.0f);
	float m_speed;				// �ړ����x
	const float SCALE = 1.0f;
	const float BOOST_SCALE = 5.0f;
	bool m_isEdge;				// ���ɂ��邩
	int m_brakeBoostTimer;
	const int MAX_BRAKE_BOOST_TIMER = 120.0f;

	// �t�B�[�o�[��Ԃ�
	bool m_isFever;
	int m_feverTime;
	const int FEVER_TIME = 600;
	const float FEVER_ATTACK_SCALE = 10.0f;

	// HP�֌W
	int m_hp;		// �v���C���[��HP
	const int MAX_HP = 5;
	int m_RedTime;		// HP���������ۂ̐F�ω��Ɏg�p����^�C�}�[�B
	const std::array<int, 4> RED_TIME = { 0,0,60,120 };	// [0]�͎g�p���Ȃ��B
	float m_colorEasingTimer;	// HP���������ۂ̐F�ω��̃C�[�W���O�Ɏg�p����^�C�}�[
	const std::array<float, 4> ADD_COLOR_EASING_TIMER = { 0.2f, 0.1f, 0.08f, 0.01f };	// [0]�͎g�p���Ȃ��B
	bool m_isRed;			// ���̐F�̏�ԁB
	bool m_isChangeRed;		// �ԂɕύX���邩
	bool m_isChangeGreen;	// �΂ɕύX���邩�B




	// �_���[�W��
	int m_damageEffectTimer;	// �_���[�W�G�t�F�N�g�̓_�łɎg�p����^�C�}�[
	const int DAMAGE_EFFECT_DRAW_CHANGE_SPAN = 4;
	int m_damageEffectCount;	// �_�ł̉�
	const int DAMAGE_EFFECT_COUNT = 3;
	bool m_isDamageEffect;		// �_���[�W�G�t�F�N�g����
	bool m_isDamageEffectDrawPlayer;

	// �h���t�g
	std::array<std::shared_ptr<DriftParticle>, 128> m_driftParticle;
	int m_driftParticleDelay;
	const int DRIFT_PARTICLE_DELAY = 5;

	// Z����]��
	float m_rotX;

	// �e�֌W
	int m_shotTimer;
	const float SHOT_TIMER = 5;

	// �u���[�L�֌W
	int m_brakeTimer;			// �u���[�L���Ă��鎞�ԁB
	const int MAX_BRAKE_TIMER = 60.0f;
	bool m_isBrake;				// �u���[�L���Ă��邩�ǂ����B

	//���f��
	std::shared_ptr<Model>m_model;

	//�A�E�g���C���p�̃��f��
	PlayerModelOutline m_outlineModel;
	const Vec3<float> GREEN_HSV = Vec3<float>(152.857f, 223.125f, 0.959f);
	const Vec3<float> RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.879f);
	const Vec3<float> DARK_RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.289f);

	PlayerDashLighting dashLight;
public:

	float MIN_SPEED = 0.7f;	// �ŏ��̈ړ����x
	float MAX_SPEED = 2.5f;	// �ő�̈ړ����x
	float BRAKE_SPEED = 0.7f;

	/*===== �����o�֐� =====*/

	Player();
	void Init();
	void Finalize();
	void Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope);
	void Draw(Camera& Cam);
	void DrawDebugInfo(Camera& Cam);

	Vec3<float> GetPos() { return m_pos; }
	Vec3<float> GetForwardVec() { return m_forwardVec; }
	Vec3<float> GetMovedVec() { return Vec3<float>(m_pos - m_prevPos).GetNormal(); }
	int GetBrakeBoostTimer() { return m_brakeBoostTimer; }


private:

	// ���͏���
	void Input(Camera& Cam, const Vec2<float>& WindowSize);

	// �ړ�����
	void Move();

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
	const Transform& GetTransform()const { return m_transform; }

	// �}�b�v���Ɏ��܂�悤�ɂ���B
	Vec3<float> KeepInMap(Vec3<float>& Pos, const float& MapSize);

	/// <summary>
	/// ���C�ƃ��f���̐����ł̓����蔻��
	/// </summary>
	/// <param name="Model"> �����蔻����s�����f�� </param>
	/// <param name="ModelTransform"> �����蔻����s�����f����Transform </param>
	/// <param name="RayDir"> ���C���΂����� </param>
	/// <param name="LineLength"> �L�΂����C�̒��� </param>
	/// <param name="ShortesetPos"> �ŒZ�̏Փ˓_ </param>
	/// <returns> �Փ˔���̗L�� </returns>
	bool CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos);

	// �����Ɛ����̓����蔻��
	bool IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2);

	// �����̌�_�����߂�B
	Vec2<float> CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2);

	// �x�N�g���ɍs����|����B
	inline Vec3<float> MulMat(Vec3<float> Vec, const DirectX::XMMATRIX& Mat) {
		DirectX::XMVECTOR vector = DirectX::XMVector3Transform({ Vec.x, Vec.y, Vec.z }, Mat);
		return Vec3<float>(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]);
	}

	// �h���t�g�̃p�[�e�B�N���𐶐��B
	void GenerateDriftParticle(const float& NowAngle, const float& Cross);

	Vec3<float> RGBtoHSV(const Vec3<float>& RGB);
	Vec3<float> HSVtoRGB(const Vec3<float>& HSV);
	void SearchMaxMinColor(Vec3<float>& Color, double& max, double& min, int& rgb);

	// �ԑ̂̐F��ς���B
	bool ChangeBodyColorEasing(const float& AddEasingAmount, EASING_TYPE EasingType, EASE_CHANGE_TYPE EaseChangeType, const Vec3<float>& StartColor, const Vec3<float>& EndColor, const bool& IsEaseX);

};

