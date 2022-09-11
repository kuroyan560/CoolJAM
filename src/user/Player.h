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

	/*===== メンバ変数 =====*/

	// トランスフォーム
	Transform m_transform;

	// 基本的な変数
	Vec3<float> m_pos;			// 現在座標
	Vec3<float> m_prevPos;		// 前フレームの座標
	Vec3<float> m_inputVec;		// 入力された方向ベクトル(移動方向ベクトルをこの方向に補完する。)
	Vec3<float> m_prevForwardVec;// 前フレームの移動方向ベクトル。
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	DirectX::XMMATRIX m_rotation;	//クォータニオン
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(0.0f, 0.0f, 1.0f);
	float m_speed;				// 移動速度
	const float SCALE = 1.0f;
	const float BOOST_SCALE = 5.0f;
	bool m_isEdge;				// 縁にいるか
	int m_brakeBoostTimer;
	const int MAX_BRAKE_BOOST_TIMER = 120.0f;

	// フィーバー状態か
	bool m_isFever;
	int m_feverTime;
	const int FEVER_TIME = 600;
	const int FEVER_TIME_GAME_TIMER = 10;
	const float FEVER_ATTACK_SCALE = 10.0f;

	// HP関係
	int m_hp;		// プレイヤーのHP
	const int MAX_HP = 5;
	int m_RedTime;		// HPが減った際の色変化に使用するタイマー。
	const std::array<int, 4> RED_TIME = { 0,0,60,120 };	// [0]は使用しない。
	float m_colorEasingTimer;	// HPが減った際の色変化のイージングに使用するタイマー
	const std::array<float, 4> ADD_COLOR_EASING_TIMER = { 0.2f, 0.1f, 0.08f, 0.01f };	// [0]は使用しない。
	bool m_isRed;			// 今の色の状態。
	bool m_isChangeRed;		// 赤に変更するか
	bool m_isChangeGreen;	// 緑に変更するか。




	// ダメージ時
	int m_damageEffectTimer;	// ダメージエフェクトの点滅に使用するタイマー
	const int DAMAGE_EFFECT_DRAW_CHANGE_SPAN = 4;
	int m_damageEffectCount;	// 点滅の回数
	const int DAMAGE_EFFECT_COUNT = 3;
	bool m_isDamageEffect;		// ダメージエフェクト中か
	bool m_isDamageEffectDrawPlayer;

	// ドリフト
	std::array<std::shared_ptr<DriftParticle>, 128> m_driftParticle;
	int m_driftParticleDelay;
	const int DRIFT_PARTICLE_DELAY = 5;

	// Z軸回転量
	float m_rotX;

	// 弾関係
	int m_shotTimer;
	const float SHOT_TIMER = 5;

	// ブレーキ関係
	int m_brakeTimer;			// ブレーキしている時間。
	const int MAX_BRAKE_TIMER = 60.0f;
	bool m_isBrake;				// ブレーキしているかどうか。

	//モデル
	std::shared_ptr<Model>m_model;

	//アウトライン用のモデル
	PlayerModelOutline m_outlineModel;
	float inputATan2f;
	const Vec3<float> GREEN_HSV = Vec3<float>(152.857f, 223.125f, 0.959f);
	const Vec3<float> RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.879f);
	const Vec3<float> DARK_RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.289f);

	PlayerDashLighting dashLight;
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
	int GetBrakeBoostTimer() { return m_brakeBoostTimer; }
	float GetMaxFeverTime() { return FEVER_TIME; }
	float GetMaxFeverTimeGameTimer() { return FEVER_TIME_GAME_TIMER; }
	bool GetIsFever() { return m_isFever; }
	float GetPlayerFeverRate() { return static_cast<float>(m_feverTime) / static_cast<float>(FEVER_TIME); }

	Vec3<float>* GetPosPtr() { return &m_pos; };
	const float *GetInputRadianPtr() { return &inputATan2f; };
	float GetInputRadian() { return inputATan2f; };

private:

	// 入力処理
	void Input(Camera& Cam, const Vec2<float>& WindowSize);

	// 移動処理
	void Move(std::weak_ptr<BulletMgr> BulletMgr);

	// エフェクト全般の更新処理
	void UpdateEffect();

	// 弾を撃つ。
	void Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr);

	// 当たり判定
	void CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope);

	// HPを1減らす。
	void Damage();

	// 01に納める。
	float Saturate(const float& Value);

	// トランスフォームゲッタ
	const Transform& GetTransform()const { return m_transform; }

	// ドリフトのパーティクルを生成。
	void GenerateDriftParticle(const float& NowAngle, const float& Cross);

	Vec3<float> RGBtoHSV(const Vec3<float>& RGB);
	Vec3<float> HSVtoRGB(const Vec3<float>& HSV);
	void SearchMaxMinColor(Vec3<float>& Color, double& max, double& min, int& rgb);

	// 車体の色を変える。
	bool ChangeBodyColorEasing(const float& AddEasingAmount, EASING_TYPE EasingType, EASE_CHANGE_TYPE EaseChangeType, const Vec3<float>& StartColor, const Vec3<float>& EndColor, const bool& IsEaseX);

};

