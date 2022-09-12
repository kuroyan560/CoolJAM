#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>
#include <array>

class Model;
class Camera;

class PlayerHP {

private:

	/*===== �����o�ϐ� =====*/

	// ���f��
	std::shared_ptr<Model> m_model;

	// �s��
	Transform m_transform;

	// �`��p�x
	float m_drawAngle;

	// �A���t�@�l
	float m_alpha;

	// �`�悷�邩�ǂ����̃t���O�B
	bool m_isActive;

	const float SIZE = 4.0f;

public:

	// �R���X�g���N�^
	PlayerHP(const float& Angle);
	void Init();
	void Update(const Vec3<float>& EnemyPos, const float& EnemySize);
	void Draw();
	inline void Activate() { m_isActive = true; };
	inline void Invalidate() { m_isActive = false; };

};