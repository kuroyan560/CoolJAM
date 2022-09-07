#pragma once
#include"Transform.h"
#include"Vec.h"
#include<memory>
#include<array>
#include<DirectXMath.h>

class Model;
class Camera;
class BulletMgr;
class EnemyMgr;

class Player
{

private:

	/*===== メンバ変数 =====*/

	// トランスフォーム
	Transform m_transform;

	// 基本的な変数
	Vec3<float> m_pos;			// 現在座標
	Vec3<float> m_prevPos;		// 前フレームの座標
	Vec3<float> m_inputVec;		// 入力された方向ベクトル(移動方向ベクトルをこの方向に補完する。)
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(0.0f, 0.0f, 1.0f);
	float m_speed;				// 移動速度
	const float SCALE = 1.0f;
	const float BOOST_SCALE = 2.0f;
	bool m_isEdge;				// 縁にいるか
	int m_brakeBoostTimer;
	const int MAX_BRAKE_BOOST_TIMER = 120.0f;

	// 弾関係
	int m_shotTimer;
	const float SHOT_TIMER = 15;

	// ブレーキ関係
	int m_brakeTimer;			// ブレーキしている時間。
	const int MAX_BRAKE_TIMER = 60.0f;
	bool m_isBrake;				// ブレーキしているかどうか。

	//モデル
	std::shared_ptr<Model>m_model;


public:

	float MIN_SPEED = 0.7f;	// 最小の移動速度
	float MAX_SPEED = 2.5f;	// 最大の移動速度
	float BRAKE_SPEED = 0.7f;

	/*===== メンバ関数 =====*/

	Player();
	void Init();
	void Finalize();
	void Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope);
	void Draw(Camera& Cam);
	void DrawDebugInfo(Camera& Cam);

	Vec3<float> GetPos() { return m_pos; }
	Vec3<float> GetForwardVec() { return m_forwardVec; }
	Vec3<float> GetMovedVec() { return Vec3<float>(m_pos - m_prevPos).GetNormal(); }


private:

	// 入力処理
	void Input(Camera& Cam, const Vec2<float>& WindowSize);

	// 移動処理
	void Move();

	// 弾を撃つ。
	void Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr);

	// 当たり判定
	void CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope);

	// 01に納める。
	float Saturate(const float& Value);

	// トランスフォームゲッタ
	const Transform& GetTransform()const { return m_transform; }

	// マップ内に収まるようにする。
	Vec3<float> KeepInMap(Vec3<float>& Pos, const float& MapSize);

	/// <summary>
	/// レイとモデルの線分での当たり判定
	/// </summary>
	/// <param name="Model"> 当たり判定を行うモデル </param>
	/// <param name="ModelTransform"> 当たり判定を行うモデルのTransform </param>
	/// <param name="RayDir"> レイを飛ばす方向 </param>
	/// <param name="LineLength"> 伸ばすレイの長さ </param>
	/// <param name="ShortesetPos"> 最短の衝突点 </param>
	/// <returns> 衝突判定の有無 </returns>
	bool CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos);

	// 線分と線分の当たり判定
	bool IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2);

	// 線分の交点を求める。
	Vec2<float> CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2);

	// ベクトルに行列を掛ける。
	inline Vec3<float> MulMat(Vec3<float> Vec, const DirectX::XMMATRIX& Mat) {
		DirectX::XMVECTOR vector = DirectX::XMVector3Transform({ Vec.x, Vec.y, Vec.z }, Mat);
		return Vec3<float>(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]);
	}

};

