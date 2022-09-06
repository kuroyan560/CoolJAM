#pragma once
#include"Transform.h"
#include<memory>
class Model;
class Camera;
#include "Vec.h"

class Player
{
	//トランスフォーム
	Transform m_transform;

private:

	/*===== メンバ変数 =====*/

	Vec3<float> m_pos;			// 現在座標
	Vec3<float> m_inputVec;		// 入力された方向ベクトル(移動方向ベクトルをこの方向に補完する。)
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(0.0f, 0.0f, 1.0f);
	float m_speed;				// 移動速度
	const float DEF_SPEED = 10.0f;			// デフォルトの移動速度
	const float BRAKE_SPEED = 3.0f;			// ブレーキ時の移動速度
	const float HANDLE_ROT = 0.03f;			// 移動方向ベクトルの保管量ラジアン
	const float BRAKE_HANDLE_ROT = 0.06f;	// ブレーキ時の移動方向ベクトルの保管量ラジアン
	bool m_isEdge;				// 縁にいるか
	const float EDGE_SIZE = 100.0f;

	// ブレーキ関係
	int m_brakeTimer;			// ブレーキしている時間。
	float m_brakeBoostSpeed;	// ブレーキによって発生したブーストの速度。
	const int MAX_BRAKE_TIMER = 120.0f;
	const float MAX_BRAKE_SPEED = 60.0f;
	const float SUB_BRAKE_SPEED = 1.0f;
	bool m_isBrake;				// ブレーキしているかどうか。
	//モデル
	std::shared_ptr<Model>m_model;


public:

	/*===== メンバ関数 =====*/

	Player();
	void Init();
	void Update();
	void Draw();

	Vec3<float> GetPos() { return m_pos; }


private:

	// 入力処理
	void Input();

	// 移動処理
	void Move();

	// 当たり判定
	void CheckHit();

	// 01に納める。
	float Saturate(const float& Value);

	//トランスフォームゲッタ
	const Transform& GetTransform()const { return m_transform; }
};

