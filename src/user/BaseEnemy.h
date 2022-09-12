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


public:

	/*===== �����o�֐� =====*/

	BaseEnemy()
	{
	};
	virtual ~BaseEnemy() {};
	virtual void Init() = 0;
	virtual void Generate(ENEMY_INFO::ID ID, const Vec3<float> &PlayerPos, const Vec3<float> &Pos, const Vec3<float> ForwardVec) = 0;
	virtual void Update(std::weak_ptr<BulletMgr> BulletMgr, const Vec3<float> &PlayerPos, const float &MapSize) = 0;
	virtual void Draw() = 0;

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