#include "UnionEnemy.h"
#include "Importer.h"
#include "Model.h"
#include "EnemyHP.h"

UnionEnemy::UnionEnemy(std::shared_ptr<Model> NormalModel, std::shared_ptr<Model> HitModel)
{

	/*===== コンストラクタ =====*/

	m_model = NormalModel;
	m_hitModel = HitModel;
	m_isActive = false;
	m_hitEffectTimer = 0;
	m_scale = SCALE;
	m_hp = 0;

	float angleInterval = DirectX::XM_PI / 10.0f;
	for (auto& index : m_hpUI) {

		int indexCount = static_cast<int>(&index - &m_hpUI[0]);

		// 回転量
		float drawAngle = 0;

		// インデックスが10以上だったら。
		if (10 < indexCount) {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * GetDigits(indexCount, 0, 0);

		}
		else {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * static_cast<float>(indexCount);

		}

		index = std::make_shared<EnemyHP>(-drawAngle);

	}

}

void UnionEnemy::Init()
{

	/*===== 初期化処理 =====*/

	m_isActive = false;
	m_scale = SCALE;
	m_hitEffectTimer = 0;
	m_hp = 0;

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Init();

	}

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

	// 敵のHPの板ポリを描画
	for (auto& index : m_hpUI) {

		index->Init();

	}

}

void UnionEnemy::Update()
{

	/*===== 更新処理 =====*/

	if (m_hp <= 0) {

		Init();

	}

	// HPUIの更新処理
	for (auto& index : m_hpUI) {

		index->Invalidate();

	}
	if (10 < m_hp) {

		for (int index = 0; index < GetDigits(m_hp, 0, 0); ++index) {

			m_hpUI[index]->Activate();

		}

	}
	else {

		for (int index = 0; index < m_hp; ++index) {

			m_hpUI[index]->Activate();

		}

	}

	// HPUIの更新処理
	for (auto& index : m_hpUI) {

		index->Update(m_pos, SCALE);

	}

}

#include"DrawFunc_Append.h"
void UnionEnemy::Draw()
{

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);
	if (0 < m_hitEffectTimer) {

		//DrawFunc3D::DrawNonShadingModel(m_hitModel, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_hitModel, m_transform);

	}
	else {

		//DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
		DrawFunc_Append::DrawModel(m_model, m_transform);

	}

	//// 敵のHPの板ポリを描画
	//for (auto& index : m_hpUI) {

	//	index->Draw();

	//}

}

#include"AudioApp.h"
#include"BaseEnemy.h"
void UnionEnemy::Damage(const int& Amount)
{

	/*===== ダメージを与える =====*/

	m_hp -= Amount;
	if (m_hp <= 0) {

		Init();
		AudioApp::Instance()->PlayWaveDelay(BaseEnemy::DeadSE());
	}
	else
	{
		AudioApp::Instance()->PlayWaveDelay(BaseEnemy::DamageSE());
	}

}
