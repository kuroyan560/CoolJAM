#pragma once
#include "D3D12App.h"
#include "KuroEngine.h"
#include <memory>
#include <array>

class PlayerDamageEffect {

private:

	/*===== �����o�ϐ� =====*/

	std::array<std::shared_ptr<TextureBuffer>, 4> m_frameTexture;	// �_���[�W���󂯂����̃t���[��
	int m_animNumber;				// �A�Ԃ̔ԍ�
	float m_animSpeed;				// �A�Ԃ��ڂ��ւ��鑬�x
	const float ANIM_SPEED = 5.0f;
	bool m_isActive;
	bool m_isExit;
	float m_alpha;


	Vec2<float> m_pos;				// ���W

public:

	/*===== �����o�֐� =====*/

	PlayerDamageEffect();
	void Init();
	void Generate(const Vec2<float>& Pos);
	void Update();
	void Draw();

	bool GetIsActive() { return m_isActive; };

};

class PlayerDamageEffectMgr {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<TextureBuffer> m_frameTexture;	// �_���[�W���󂯂����̃t���[��
	std::array<PlayerDamageEffect, 16> m_effect;
	float m_frameAlpha;								// �_���[�W���󂯂����̃t���[���̃A���t�@

	float m_damageEffectTimer;	// �L��������Ă���Ԃ̃^�C�}�[
	const int DAMAGE_EFFECT_TIMER = 40;

	int m_generateCount;
	const int GENERATE_COUNT = 5;
	float m_generateSpan;
	const int GENERATE_SPAN = 3;

	const float GENERATE_RANGE = 50.0f;


public:

	/*===== �����o�֐� =====*/

	PlayerDamageEffectMgr();
	void Init();
	void Update();
	void Draw();

	void Start();


};