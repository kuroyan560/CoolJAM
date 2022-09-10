#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>

class Camera;
class Model;

class UnionEnemy {

private:

	/*===== メンバ変数 =====*/

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_hitModel;
	Transform m_transform;

	Vec3<float> m_pos;		// 座標
	float m_scale;			// 大きさ
	const float SCALE = 3.0f;
	int m_hp;				// HP
	const int HP = 3;
	int m_hitEffectTimer;	// 弾に当たった際に赤色に描画するタイマー
	const int HIT_EFFECT_TIMER = 10;
	bool m_isActive;		// 生存フラグ


public:

	/*===== メンバ関数 =====*/

	UnionEnemy(std::shared_ptr<Model> NormalModel, std::shared_ptr<Model> HitModel);
	void Init();
	void Generate(const Vec3<float>& Pos);
	void Update();
	void Draw();

	bool GetIsActive() { return m_isActive; }
	Vec3<float> GetPos() { return m_pos; }
	float GetScale() { return m_scale; }

	void Damage(const int& Amount);

};