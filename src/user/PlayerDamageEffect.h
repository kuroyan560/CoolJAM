#pragma once
#include "D3D12App.h"
#include "KuroEngine.h"
#include <memory>
#include <array>

class PlayerDamageEffect {

private:

	/*===== メンバ変数 =====*/

	std::array<std::shared_ptr<TextureBuffer>, 4> m_frameTexture;	// ダメージを受けた時のフレーム
	int m_animNumber;				// 連番の番号
	float m_animSpeed;				// 連番を移し替える速度
	const float ANIM_SPEED = 5.0f;
	bool m_isActive;
	bool m_isExit;
	float m_alpha;


	Vec2<float> m_pos;				// 座標

public:

	/*===== メンバ関数 =====*/

	PlayerDamageEffect();
	void Init();
	void Generate(const Vec2<float>& Pos);
	void Update();
	void Draw();

	bool GetIsActive() { return m_isActive; };

};

class PlayerDamageEffectMgr {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<TextureBuffer> m_frameTexture;	// ダメージを受けた時のフレーム
	std::array<PlayerDamageEffect, 16> m_effect;
	float m_frameAlpha;								// ダメージを受けた時のフレームのアルファ

	float m_damageEffectTimer;	// 有効化されている間のタイマー
	const int DAMAGE_EFFECT_TIMER = 40;

	int m_generateCount;
	const int GENERATE_COUNT = 5;
	float m_generateSpan;
	const int GENERATE_SPAN = 3;

	const float GENERATE_RANGE = 50.0f;


public:

	/*===== メンバ関数 =====*/

	PlayerDamageEffectMgr();
	void Init();
	void Update();
	void Draw();

	void Start();


};