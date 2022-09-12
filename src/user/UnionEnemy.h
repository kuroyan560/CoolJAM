#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>
#include <array>

class EnemyHP;

class Camera;
class Model;

class UnionEnemy {

private:

	/*===== �����o�ϐ� =====*/

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_hitModel;
	Transform m_transform;

	Vec3<float> m_pos;		// ���W
	float m_scale;			// �傫��
	const float SCALE = 3.0f;
	int m_hp;				// HP
	static const int HP = 3;
	int m_hitEffectTimer;	// �e�ɓ��������ۂɐԐF�ɕ`�悷��^�C�}�[
	const int HIT_EFFECT_TIMER = 10;
	bool m_isActive;		// �����t���O

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


public:

	/*===== �����o�֐� =====*/

	UnionEnemy(std::shared_ptr<Model> NormalModel, std::shared_ptr<Model> HitModel);
	void Init();
	void Generate(const Vec3<float>& Pos);
	void Update();
	void Draw();

	bool GetIsActive() { return m_isActive; }
	Vec3<float> GetPos() { return m_pos; }
	float GetScale() { return m_scale; }

	void Damage(const int& Amount);

	// �w��̌��̐������擾�B
	int GetDigits(int Value, int M, int N) {
		int mod_value;
		int result;

		/* n���ڈȉ��̌����擾 */
		mod_value = Value % (int)pow(10, N + 1);

		/* m���ڈȏ�̌����擾 */
		result = mod_value / pow(10, M);

		return result;

	}

};