#pragma once
#include"Transform.h"
#include"Vec.h"
#include<memory>
#include<array>

class Model;
class Camera;

class DriftParticle {

private:

	/*===== メンバ変数 =====*/

	// トランスフォーム
	Transform m_transform;

	// モデル
	std::shared_ptr<Model>m_model;

	// 基本的な変数
	Vec3<float> m_pos;			// 座標
	Vec3<float> m_playerPos;
	Vec3<float> m_prevPos;		// 前フレームの座標
	Vec3<float> m_nowFrameMoveVec;
	Vec3<float> m_prevFrameMoveVec;
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	float m_speed;				// 移動速度
	const float MAX_SPEED = 2.0f;
	const float SUB_SPEED = 0.5f;
	float m_scale;
	const float SCALE = 0.5f;	// 大きさ
	const float SUB_SCALE = 0.25f;
	int m_exitTimer;			// 消えるまでのタイマー
	const int EXIT_TIMER = 0;
	float m_gravity;			// 重力
	const float ADD_GRAVITY = 0.01f;
	const float MAX_GRAVITY = 0.1f;
	bool m_isActive;			// 生存フラグ

public:

	DriftParticle();
	void Init();
	void Generate(const Vec3<float>& Pos, const float& Angle, const float& Rate);
	void Update(const Vec3<float>& Pos);
	void Draw();

	bool GetIsActive() { return m_isActive; }

};