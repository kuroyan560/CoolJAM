#pragma once
#include "Vec.h"
#include "EnemyWave.h"
#include <memory>
#include <array>
#include"Outline.h"
#include"ColorPalette.h"

class Model;
class Camera;
class BulletMgr;

class BaseEnemy {

public:

	/*===== メンバ関数 =====*/

	// モデル。
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_modelHit;
	std::unique_ptr<Outline>m_outline;

	Vec3<float> m_pos;	// 座標
	int m_hp;			// HP
	float m_scale;		// 大きさ
	bool m_isActive;	// 生存フラグ
	ENEMY_INFO::ID m_id;

	Transform m_transform;


public:

	/*===== メンバ関数 =====*/

	BaseEnemy()
	{
	};
	virtual ~BaseEnemy() {};
	virtual void Init() = 0;
	virtual void Generate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec) = 0;
	virtual void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize) = 0;
	virtual void Draw() = 0;

	void Damage(const int &Amount, std::weak_ptr<BulletMgr> BulletMgr);

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





	void CommonUpdate()
	{
		if (m_outline)
		{
			m_outline->Upadte();
		}
	};
	void CommonDraw(Camera &CAMERA)
	{
		if (m_outline)
		{
			m_outline->Draw(CAMERA);
		}
	};

	void CommonInit()
	{
		m_outline = std::make_unique<Outline>(m_model, &m_transform, ColorPalette::S_PINK_COLOR);
	};

protected:

};