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

	/*===== メンバ変数 =====*/

	// 基本的な変数
	Vec3<float> m_pos;			// 現在座標
	Vec3<float> m_prevPos;		// 前フレームの座標
	Vec3<float> m_inputVec;		// 入力された方向ベクトル(移動方向ベクトルをこの方向に補完する。)
	Vec3<float> m_prevForwardVec;// 前フレームの移動方向ベクトル。
	Vec3<float> m_forwardVec;	// 移動方向ベクトル
	DirectX::XMMATRIX m_rotation;	//クォータニオン
	const Vec3<float> DEF_FORWARDVEC = Vec3<float>(1.0f, 0.0f, 0.0f);
	float m_speed;				// 移動速度
	const float SCALE = 1.0f;
	const float BOOST_SCALE = 20.0f;
	bool m_isEdge;				// 縁にいるか
	float m_brakeBoostTimer;
	const int MAX_BRAKE_BOOST_TIMER = 120.0f;

	// マウス座標に設置するモデル。
	std::shared_ptr<ModelObject>m_mousePointer;
	float m_mousePointerScale;
	const float MOUSE_POINTER_SCALE = 5.0f;

	// 動いた総量
	float m_movedLength;

	// フィーバー状態か
	bool m_isPrevFever;
	bool m_isFever;
	float m_feverTime;
	const int FEVER_TIME = 600;
	const int FEVER_TIME_GAME_TIMER = 10;

	// HP関係
	int m_hp;		// プレイヤーのHP
	static const int MAX_HP = 10;
	int m_RedTime;		// HPが減った際の色変化に使用するタイマー。
	const std::array<int, 4> RED_TIME = { 0,0,60,120 };	// [0]は使用しない。
	float m_colorEasingTimer;	// HPが減った際の色変化のイージングに使用するタイマー
	const std::array<float, 4> ADD_COLOR_EASING_TIMER = { 0.2f, 0.1f, 0.08f, 0.01f };	// [0]は使用しない。
	bool m_isRed;			// 今の色の状態。
	bool m_isChangeRed;		// 赤に変更するか
	bool m_isChangeGreen;	// 緑に変更するか。

	std::array<std::shared_ptr<PlayerHP>, MAX_HP> m_hpUI;


	// ダメージ時
	float m_damageEffectTimer;	// ダメージエフェクトの点滅に使用するタイマー
	const int DAMAGE_EFFECT_DRAW_CHANGE_SPAN = 4;
	int m_damageEffectCount;	// 点滅の回数
	const int DAMAGE_EFFECT_COUNT = 3;
	bool m_isDamageEffect;		// ダメージエフェクト中か
	bool m_isDamageEffectDrawPlayer;

	// チュートリアル用。
	float m_dashCounter;

	// ドリフト
	std::array<std::shared_ptr<DriftParticle>, 128> m_driftParticle;
	int m_driftParticleDelay;
	const int DRIFT_PARTICLE_DELAY = 5;

	// Z軸回転量
	float m_rotX;

	// 弾関係
	float m_shotTimer;
	const float SHOT_TIMER = 5;

	// ブレーキ関係
	int m_brakeTimer;			// ブレーキしている時間。
	const int MAX_BRAKE_TIMER = 60.0f;
	bool m_isBrake;				// ブレーキしているかどうか。

	//モデル
	std::shared_ptr<ModelObject>m_modelObj;

	//アウトライン用のモデル
	float inputATan2f;
	const Vec3<float> GREEN_HSV = Vec3<float>(152.857f, 223.125f, 0.959f);
	const Vec3<float> RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.879f);
	const Vec3<float> DARK_RED_HSV = Vec3<float>(352.394f - 360.0f, 205.738f, 0.289f);

	PlayerModelOutline m_outlineModel;
	std::unique_ptr<PlayerDashLighting> m_dashLight;

public:

	float MIN_SPEED = 0.7f;	// 最小の移動速度
	float MAX_SPEED = 2.5f;	// 最大の移動速度
	float BRAKE_SPEED = 0.7f;

	/*===== メンバ関数 =====*/

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

	// 入力処理
	void Input(Camera& Cam, const Vec2<float>& WindowSize);

	// 移動処理
	void Move(std::weak_ptr<BulletMgr> BulletMgr, bool IsStopFeverTimer);

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
	const Transform& GetTransform()const { return m_modelObj->m_transform; }

	// ドリフトのパーティクルを生成。
	void GenerateDriftParticle(const float& NowAngle, const float& Cross);


	// 車体の色を変える。
	bool ChangeBodyColorEasing(const float& AddEasingAmount, EASING_TYPE EasingType, EASE_CHANGE_TYPE EaseChangeType, const Vec3<float>& StartColor, const Vec3<float>& EndColor, const bool& IsEaseX);

	// 指定の桁の数字を取得。
	inline int GetDigits(int Value, int M, int N) {
		int mod_value;
		int result;

		/* n桁目以下の桁を取得 */
		mod_value = Value % (int)pow(10, N + 1);

		/* m桁目以上の桁を取得 */
		result = mod_value / pow(10, M);

		return result;

	}

	// マウスのわーるど座標を求める。
	bool Player::CheckHitRayToStagePolygon(const Vec3<float>& RayStartPos, const Vec3<float>& RayDir, Vec3<float>& HitPos)
	{

		// 平面の頂点
		Vec3<float> p1 = Vec3<float>(10000, 0, 10000);
		Vec3<float> p2 = Vec3<float>(-10000, 0, -10000);
		Vec3<float> p3 = Vec3<float>(10000, 0, -10000);

		// レイの開始地点から平面におろした垂線の長さを求める
		Vec3<float> planeNorm = Vec3<float>(0, 1, 0);
		float rayToOriginLength = RayStartPos.Dot(planeNorm);
		float planeToOriginLength = p1.Dot(planeNorm);
		// 視点から平面におろした垂線の長さ
		float perpendicularLine = rayToOriginLength - planeToOriginLength;

		// 三角関数を利用して視点から衝突店までの距離を求める
		float dist = planeNorm.Dot(RayDir);
		float impDistance = perpendicularLine / -dist;

		// 衝突地点
		HitPos = RayStartPos + RayDir * impDistance;

		return true;
	}


};

