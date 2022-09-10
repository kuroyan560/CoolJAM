#include "UnionEnemy.h"
#include "Importer.h"
#include "Model.h"

UnionEnemy::UnionEnemy(std::shared_ptr<Model> NormalModel, std::shared_ptr<Model> HitModel)
{

	/*===== �R���X�g���N�^ =====*/

	m_model = NormalModel;
	m_hitModel = HitModel;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_scale = SCALE;
	m_hp = 0;

}

void UnionEnemy::Init()
{

	/*===== ���������� =====*/

	m_isActive = false;
	m_scale = SCALE;
	m_hitEffectTimer = 0;
	m_hp = 0;

}

void UnionEnemy::Generate(const Vec3<float>& Pos)
{

	/*===== �������� =====*/

	m_pos = Pos;
	m_scale = SCALE;
	m_isActive = true;
	m_hitEffectTimer = 0;
	m_hp = HP;
	m_transform.SetScale(SCALE);

}

void UnionEnemy::Update()
{

	/*===== �X�V���� =====*/

	if (m_hp <= 0) {

		Init();

	}

}

#include"DrawFunc_Append.h"
void UnionEnemy::Draw()
{

	/*===== �`�揈�� =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_hitModel, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_hitModel, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

}

void UnionEnemy::Damage(const int& Amount)
{

	/*===== �_���[�W��^���� =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		Init();

	}

}
