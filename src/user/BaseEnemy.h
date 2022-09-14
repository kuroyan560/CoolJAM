#pragma once
#include "Vec.h"
#include "EnemyWave.h"
#include <memory>
#include <array>
#include"Outline.h"
#include"ColorPalette.h"
#include"../engine/UsersInput.h"
#include<array>

class Model;
class Camera;
class BulletMgr;
class Model;
class TextureBuffer;

class BaseEnemy {

	static int s_deadSE;
	static int s_damageSE;
	static std::shared_ptr<Model>s_appearReticleModel;
	static std::array<std::shared_ptr<TextureBuffer>, 5>s_appearReticleTex;

	//登場演出
	static const int APPEAR_TIME = 90;
	static const int APPEAR_HEIGHT_OFFSET = 60;
	static const int RETICLE_TEX_CHANGE_SPAN = 5;
	static const int RETICLE_SCALE_MAX = 60.0f;
	static const int RETICLE_SCALE_MIN = 20.0f;

	//退散演出
	static const int DISAPPEAR_TIME = 60;
	static const int DISAPPEAR_SPIN_DEGREE = 360 * 3;
	static const int DISAPPEAR_HEIGHT_OFFSET = -3;

	const float EDGE_SIZE = 20.0f;

	//登場演出
	int m_appearTimer;
	int m_appearReticleTexIdx;
	Angle m_appearReticleAngle;
	float m_appearReticleHeight;
	Vec3<float>m_generatePos;
	Vec3<float>m_generateForwardVec;

	//退散演出
	bool m_disappear;
	int m_disappearTimer;
	Vec3<float>m_disAppearStartScale;
	Vec3<float> m_disappearStartFront;
	float m_disappearStartHeight;

public:

	/*===== メンバ関数 =====*/

	// モデル。
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	std::unique_ptr<Outline>m_outline;
	Color m_outlineColor;

	Vec3<float> m_pos;	// 座標
	int m_hp;			// HP
	int m_shotTimer;
	int m_maxShotTimer;
	float m_scale;		// 大きさ
	bool m_isActive;	// 生存フラグ
	ENEMY_INFO::ID m_id;

	Transform m_transform;

	const int LOW_SCORE_POINT = 300;
	const int MIDDLE_SCORE_POINT = 600;
	const int HIGHT_SCORE_POOINT = 900;

	virtual void OnInit() = 0;
	virtual void OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize) = 0;
	virtual void OnGenerate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec) = 0;
	virtual void OnDraw() = 0;
public:
	static const int &DamageSE() { return s_damageSE; }
	static const int &DeadSE() { return s_deadSE; }

	/*===== メンバ関数 =====*/

	BaseEnemy();
	virtual ~BaseEnemy() {};
	void Init();
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Generate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec, const int& ShotTimer);
	void Draw();
	void Disappear();

	void ShotBullet(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos);

	void Damage(const int& Amount, std::weak_ptr<BulletMgr> BulletMgr);

	void CheckHitMapEdge(const float& MapSize, std::weak_ptr<BulletMgr> BulletMgr);

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



	bool AnnnounceHit()
	{
		if (m_disappearTimer)return false;

		if (m_hp != m_prevHp || UsersInput::Instance()->KeyOnTrigger(DIK_M))
		{
			m_hitFlag = true;
			m_hitTiemr = 0;
			m_shrinkScale += 0.3f;
		}
		if (m_hitFlag)
		{
			m_lDamageScale = Vec3<float>(m_shrinkScale, m_shrinkScale, m_shrinkScale);
			++m_hitTiemr;
		}
		if (5 <= m_hitTiemr)
		{
			float s = 0.0f;
			m_lDamageScale = Vec3<float>(s, s, s);
			m_hitTiemr = 0;
			m_hitFlag = false;
		}
		m_damageScale = KuroMath::Lerp(m_damageScale, m_lDamageScale, 0.6f);

		m_prevHp = m_hp;

		//ダメージ受けすぎて小さくなりすぎない処理
		Vec3<float>shrinkScale = m_baseScale - m_damageScale;
		const float MIN_SCALE = 1.5f;
		if(shrinkScale.x <= MIN_SCALE)
		{
			shrinkScale = { MIN_SCALE,MIN_SCALE,MIN_SCALE };
		}

		m_transform.SetScale(shrinkScale);
		return m_hitFlag;
	}

	void CommonInit()
	{
		m_outline = std::make_unique<Outline>(m_model, &m_transform, m_outlineColor);
		m_prevHp = m_hp;
		m_hitTiemr = 0;
		m_hitFlag = false;
		m_baseScale = m_transform.GetScale();
		m_damageScale = {};
		m_shrinkScale = 0.0f;
	};
	void CommonUpdate()
	{
		m_outline->Upadte();
	};
	void CommonDraw(Camera &CAMERA)
	{
		m_outline->Draw(CAMERA, m_hitFlag);
	};

	const bool &IsDisappear()const { return m_disappear; }

private:
	int m_prevHp;
	bool m_hitFlag;
	int m_hitTiemr;

	Vec3<float>m_damageScale;
	Vec3<float>m_lDamageScale;
	float m_shrinkScale;
	Vec3<float>m_baseScale;
};