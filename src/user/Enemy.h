#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>
#include <array>

class Model;
class Camera;
class BulletMgr;

class Enemy {

private:

	/*===== �����o�ϐ� =====*/

	Vec3<float> m_pos;			// ���W
	Vec3<float> m_forwardVec;	// �ړ������x�N�g��
	float m_scale;				// �傫��
	float m_speed;				// �ړ����x
	const float SPEED = 0.6f;	// �ړ����x�̃f�t�H���g�l�B
	bool m_isActive;			// �����t���O

	//���f��
	std::shared_ptr<Model> m_model;
	Transform m_transform;

	const std::array<float, 3> SCALE = { 6,3,1 };
	const float TRACKING_ROT = 0.05f;


public:

	enum class ID {

		STOPPING,
		STRAIGHT,
		TRACKING,

	};

private:

	ID m_id;


public:

	/*===== �����o�֐� =====*/

	void Init();
	void Generate(ID ID, const Vec3<float>& PlayerPos, const Vec3<float>& Pos);
	void Update(std::weak_ptr< BulletMgr> BulletMgr, const Vec3<float>& PlayerPos, const float& MapSize);
	void Draw(Camera& Cam);

	bool GetIsActive() { return m_isActive; }
	Vec3<float> GetPos() { return m_pos; }


private:

	void CheckHit(std::weak_ptr< BulletMgr> BulletMgr, const float& MapSize);

};