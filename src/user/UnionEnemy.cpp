#include "UnionEnemy.h"
#include "Importer.h"
#include "Model.h"

UnionEnemy::UnionEnemy(std::shared_ptr<Model> NormalModel, std::shared_ptr<Model> HitModel)
{

	/*===== コンストラクタ =====*/

	m_model = NormalModel;
	m_hitModel = HitModel;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_scale = SCALE;
	m_hp = 0;

}

void UnionEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;
	m_scale = SCALE;
	m_hitEffectTimer = 0;
	m_hp = 0;

}

void UnionEnemy::Generate(const Vec3<float>& Pos)
{

	/*===== 生成処理 =====*/

	m_pos = Pos;
	m_scale = SCALE;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_hp = HP;
	m_transform.SetScale(SCALE);

}

void UnionEnemy::Update()
{

	/*===== 更新処理 =====*/

	if (m_hp <= 0) {

		Init();

	}

}

#include "DrawFunc3D.h"
void UnionEnemy::Draw(Camera& Cam)
{

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		DrawFunc3D::DrawNonShadingModel(m_hitModel, m_transform, Cam);

	}
	else {

		DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

	}

}

void UnionEnemy::Damage(const int& Amount)
{

	/*===== ダメージを与える =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		Init();

	}

}
