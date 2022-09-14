
#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>

class Model;
class Camera;

class PlayerBullet {

	static std::shared_ptr<Model> s_model;

private:

	/*===== �����o�ϐ� =====*/

	Vec3<float> m_pos;
	Vec3<float> m_forwardVec;
	float m_scale;
	const float SCALE = 3.0f;
	const float CHECK_HIT_SCALE = 3.0f;
	const float ADD_SPEED = 0.5f;
	const float MAX_SPEED = 10.0f;
	float m_speed;
	bool m_isActive;

	Transform m_transform;


public:

	/*===== �����o�֐� =====*/

	PlayerBullet();
	void Init();
	void Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec);
	void Update(const float& MapSize);
	void Draw();

	inline Vec3<float> GetPos() { return m_pos; }
	inline float GetCheckHitScale() { return CHECK_HIT_SCALE; }

	bool GetIsActive() {
		return m_isActive;
	}

};