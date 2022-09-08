#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>

class Model;
class Camera;

class EnemyBullet {

	static std::shared_ptr<Model> s_model;

private:

	/*===== ƒƒ“ƒo•Ï” =====*/

	Vec3<float> m_pos;
	Vec3<float> m_forwardVec;
	float m_scale;
	const float SCALE = 4.0f;
	const float CHECK_HIT_SCALE = 4.0f;
	const float SPEED = 0.3f;
	bool m_isActive;

	Transform m_transform;


public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	EnemyBullet();
	void Init();
	void Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec);
	void Update(const float& MapSize);
	void Draw();

	inline Vec3<float> GetPos() { return m_pos; }
	inline float GetCheckHitScale() { return m_scale; }

	bool GetIsActive() {
		return m_isActive;
	}

};