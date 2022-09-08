#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>

class Model;
class Camera;

class PlayerBullet {

private:

	/*===== ƒƒ“ƒo•Ï” =====*/

	Vec3<float> m_pos;
	Vec3<float> m_forwardVec;
	float m_scale;
	const float SCALE = 0.5f;
	const float CHECK_HIT_SCALE = 3.0f;
	const float SPEED = 2.5f;
	bool m_isActive;

	std::shared_ptr<Model> m_model;
	Transform m_transform;


public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	PlayerBullet();
	void Init();
	void Generate(const Vec3<float>& Pos, const Vec3<float>& ForwardVec);
	void Update(const float& MapSize);
	void Draw(Camera& NowCam);

	inline Vec3<float> GetPos() { return m_pos; }
	inline float GetCheckHitScale() { return m_scale; }

	bool GetIsActive() {
		return m_isActive;
	}

};