#include"EnvironmentMgr.h"
#include"Importer.h"
#include"UsersInput.h"
#include"Object.h"

EnvironmentMgr::EnvironmentMgr()
{
	static const auto DIR = "resource/user/";

	m_skyDomeModelArray[STATUS::DEFAULT] = Importer::Instance()->LoadModel(DIR, "skyDome_mono.glb");
	m_skyDomeModelArray[STATUS::FEVER] = Importer::Instance()->LoadModel(DIR, "skyDome_fever.glb");

	m_pillarModelArray[STATUS::DEFAULT] = Importer::Instance()->LoadModel(DIR, "pillar_mono.glb");
	m_pillarModelArray[STATUS::FEVER] = Importer::Instance()->LoadModel(DIR, "pillar_fever.glb");
}

void EnvironmentMgr::Init()
{
	m_nextStatus = STATUS::NONE;
	m_nowStatus = STATUS::DEFAULT;
	m_statusChangeRate = 1.0f;
	initLineLightFlag = false;
}

void EnvironmentMgr::Update()
{
	static const Angle PILLAR_POS_ANGLE_OFFSET = Angle::ROUND() / PILLAR_NUM;

	//ステータス切り替わり
	if (m_nextStatus != STATUS::NONE)
	{
		m_statusChangeRate += 1.0f / m_statusChangeTime;

		//切り替わり完了
		if (1.0f <= m_statusChangeRate)
		{
			m_nowStatus = m_nextStatus;
			m_nextStatus = STATUS::NONE;
			m_statusChangeRate = 1.0f;
			initLineLightFlag = true;
		}
	}

	if (!initLineLightFlag && !lineLight)
	{
		std::vector<Vec3<float>>posArray;
		for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
		{
			Angle posAngle = PILLAR_POS_ANGLE_OFFSET * pillarIdx;
			posArray.push_back(
				{ cos(posAngle) * m_pillarPosRadius, m_pillarPosY, sin(posAngle) * m_pillarPosRadius }
			);
		}
		lineLight = std::make_unique<LineLight>(posArray);
		lineLight->Init();
		initLineLightFlag = true;
	}


	//デバッグ
	if (UsersInput::Instance()->KeyOnTrigger(DIK_S) && m_nextStatus == STATUS::NONE)
	{
		ChangeStatus(STATUS(1 - m_nowStatus));
	}


	if (initLineLightFlag && lineLight)
	{
		lineLight->Update();
	}

}

#include"DrawFunc3D.h"
void EnvironmentMgr::Draw(Camera &Cam)
{
	static const Angle PILLAR_POS_ANGLE_OFFSET = Angle::ROUND() / PILLAR_NUM;

	if (m_nextStatus == STATUS::NONE || m_statusChangeRate < 0.3f)
	{
		//スカイドーム
		DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_nowStatus], Transform(), Cam);

		//柱
		for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
		{
			Transform transform;
			Angle posAngle = PILLAR_POS_ANGLE_OFFSET * pillarIdx;
			transform.SetPos({ cos(posAngle) * m_pillarPosRadius,m_pillarPosY,sin(posAngle) * m_pillarPosRadius });
			DrawFunc3D::DrawNonShadingModel(m_pillarModelArray[m_nowStatus], transform, Cam);
		}
	}

	if (m_nextStatus != STATUS::NONE)
	{
		//スカイドーム拡縮
		Transform skyDomeTransform;
		float scale = KuroMath::Ease(InOut, Circ, m_statusChangeRate, 0.0f, 1.0f);
		skyDomeTransform.SetScale(scale);
		Angle rotate = KuroMath::Ease(Out, Circ, m_statusChangeRate, Angle::ROUND() * 3.0f, 0.0f);
		skyDomeTransform.SetRotate({ 0,1,0 }, rotate);
		DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_nextStatus], skyDomeTransform, Cam);

		//柱回転しながら広がる
		for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
		{
			Transform transform;
			Angle posAngle = rotate + Angle(PILLAR_POS_ANGLE_OFFSET * pillarIdx);
			float posRadius = KuroMath::Ease(Out, Back, m_statusChangeRate, 0.0f, m_pillarPosRadius);
			transform.SetPos({ cos(posAngle) * posRadius,m_pillarPosY,sin(posAngle) * posRadius });
			DrawFunc3D::DrawNonShadingModel(m_pillarModelArray[m_nextStatus], transform, Cam);
		}
	}


	if (initLineLightFlag && lineLight)
	{
		lineLight->Draw(Cam);
	}
}

void EnvironmentMgr::OnImguiDebug()
{
	ImGui::Begin("EnvironmentMgr");

	/*--- ステータス ---*/
		//現在のステータス
	ImGui::Text("NowStatus : { %s }", s_skyDomeNameArray[m_nowStatus].c_str());

	//ステータス切り替え実行
	std::string currentChangeSkyDome = "NONE";
	if (m_nextStatus != STATUS::NONE)currentChangeSkyDome = s_skyDomeNameArray[m_nextStatus];

	if (ImGui::BeginCombo("NextStatus", currentChangeSkyDome.c_str()))
	{
		for (int n = 0; n < STATUS::NUM; ++n)
		{
			bool isSelected = (m_nextStatus == n);
			if (m_nextStatus == STATUS::NONE && ImGui::Selectable(s_skyDomeNameArray[n].c_str(), isSelected))
			{
				ChangeStatus((STATUS)n);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	//ステータス切り替え時間
	if (ImGui::DragInt("StatusChangeTime", &m_statusChangeTime) && m_statusChangeTime < 1)
	{
		m_statusChangeTime = 1;    //０以下防止
	}

	/*--- 円周柱 ---*/
	ImGui::Separator();

	//座標半径
	if (ImGui::DragFloat("Pillar_PosRadius", &m_pillarPosRadius, 0.01f) && m_pillarPosRadius < 1.0f)
	{
		m_pillarPosRadius = 1.0f;
	}
	//Y座標
	ImGui::DragFloat("Pillar_PosY", &m_pillarPosY, 0.5f);

	ImGui::End();
}