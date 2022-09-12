#pragma once
#include "Vec.h"
#include "Transform.h"
#include <memory>
#include <array>

class Model;
class Camera;

class PlayerHP {

private:

	/*===== メンバ変数 =====*/

	// モデル
	std::shared_ptr<Model> m_model;

	// 行列
	Transform m_transform;

	// 描画角度
	float m_drawAngle;

	// アルファ値
	float m_alpha;

	// 描画するかどうかのフラグ。
	bool m_isActive;

	const float SIZE = 4.0f;

public:

	// コンストラクタ
	PlayerHP(const float& Angle);
	void Init();
	void Update(const Vec3<float>& EnemyPos, const float& EnemySize);
	void Draw();
	inline void Activate() { m_isActive = true; };
	inline void Invalidate() { m_isActive = false; };

};