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

	/*===== メンバ変数 =====*/

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Model> m_hitModel;
	Transform m_transform;

	Vec3<float> m_pos;		// 座標
	float m_scale;			// 大きさ
	const float SCALE = 3.0f;
	int m_hp;				// HP
	static const int HP = 3;
	int m_hitEffectTimer;	// 弾に当たった際に赤色に描画するタイマー
	const int HIT_EFFECT_TIMER = 10;
	bool m_isActive;		// 生存フラグ

	std::array<std::shared_ptr<EnemyHP>, HP> m_hpUI;


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

	// 指定の桁の数字を取得。
	int GetDigits(int Value, int M, int N) {
		int mod_value;
		int result;

		/* n桁目以下の桁を取得 */
		mod_value = Value % (int)pow(10, N + 1);

		/* m桁目以上の桁を取得 */
		result = mod_value / pow(10, M);

		return result;

	}

};