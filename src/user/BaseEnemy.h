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

	static const int APPEAR_TIME = 90;
	static const int APPEAR_HEIGHT_OFFSET = 60;
	static const int RETICLE_TEX_CHANGE_SPAN = 5;
	static const int RETICLE_SCALE_MAX = 60.0f;
	static const int RETICLE_SCALE_MIN = 20.0f;


	int m_appearTimer;
	int m_appearReticleTexIdx;
	Angle m_appearReticleAngle;
	float m_appearReticleHeight;
	Vec3<float>m_generatePos;
	Vec3<float>m_generateForwardVec;

public:

	/*===== �����o�֐� =====*/

	// ���f���B
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	std::unique_ptr<Outline>m_outline;

	Vec3<float> m_pos;	// ���W
	int m_hp;			// HP
	float m_scale;		// �傫��
	bool m_isActive;	// �����t���O
	ENEMY_INFO::ID m_id;

	Transform m_transform;


	virtual void OnUpdate(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize) = 0;
	virtual void OnGenerate(ENEMY_INFO::ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos, const Vec3<float> ForwardVec) = 0;
	virtual void OnDraw() = 0;
public:
	static const int &DamageSE() { return s_damageSE; }
	static const int &DeadSE() { return s_deadSE; }

	/*===== �����o�֐� =====*/

	BaseEnemy();
	virtual ~BaseEnemy() {};
	virtual void Init() = 0;
	void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Generate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec);
	void Draw();

	void Damage(const int &Amount, std::weak_ptr<BulletMgr> BulletMgr);

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



	bool AnnnounceHit()
	{
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
		m_transform.SetScale(m_baseScale - m_damageScale);
		return m_hitFlag;
	}

	void CommonInit()
	{
		m_outline = std::make_unique<Outline>(m_model, &m_transform, ColorPalette::S_PINK_COLOR);
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

private:
	int m_prevHp;
	bool m_hitFlag;
	int m_hitTiemr;

	Vec3<float>m_damageScale;
	Vec3<float>m_lDamageScale;
	float m_shrinkScale;
	Vec3<float>m_baseScale;
};