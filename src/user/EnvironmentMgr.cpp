#include"EnvironmentMgr.h"
#include"Importer.h"
#include"UsersInput.h"

EnvironmentMgr::EnvironmentMgr()
{
    m_skyDomeModelArray[SKY_DOME::DEFAULT] = Importer::Instance()->LoadModel("resource/user/", "skyDome_monochrome.glb");
    m_skyDomeModelArray[SKY_DOME::FEVER] = Importer::Instance()->LoadModel("resource/user/", "skyDome_fever.glb");
}

void EnvironmentMgr::Init()
{
	m_nextSkyDomeIdx = SKY_DOME::NONE;
	m_skyDomeIdx = SKY_DOME::DEFAULT;
    m_skyDomeChangeRate = 0.0f;
}

void EnvironmentMgr::Update()
{
	//スカイドーム切り替わり
	if (m_nextSkyDomeIdx != SKY_DOME::NONE)
	{
        m_skyDomeChangeRate += 1.0f / m_changeSkyDomeTime;

        //切り替わり完了
        if (1.0f <= m_skyDomeChangeRate)
        {
            m_skyDomeIdx = m_nextSkyDomeIdx;
            m_nextSkyDomeIdx = SKY_DOME::NONE;
            m_skyDomeChangeRate = 0.0f;
        }
	}

    //デバッグ
    if(UsersInput::Instance()->KeyOnTrigger(DIK_S))
    {
        m_nextSkyDomeIdx = 1 - m_skyDomeIdx;
    }
}

#include"DrawFunc3D.h"
void EnvironmentMgr::Draw(Camera& Cam)
{
    static Transform transform;

    if (m_skyDomeChangeRate < 0.02f)
    {
        DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_skyDomeIdx], transform, Cam);
    }

    if (m_nextSkyDomeIdx != SKY_DOME::NONE)
    {
        float scale = KuroMath::Ease(Out, Back, m_skyDomeChangeRate, 0.0f, 1.0f);
        transform.SetScale(scale);
        Angle rotate = KuroMath::Ease(Out, Circ, m_skyDomeChangeRate, Angle::ROUND() * 3.0f, 0.0f);
        transform.SetRotate({ 0,1,0 }, rotate);
        DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_nextSkyDomeIdx], transform, Cam);
    }

}

void EnvironmentMgr::OnImguiDebug()
{
	ImGui::Begin("EnvironmentMgr");

    //スカイドーム切り替え時間
    if (ImGui::DragInt("ChangeSkyDomeTime", &m_changeSkyDomeTime) && m_changeSkyDomeTime < 1)
    {
        m_changeSkyDomeTime = 1;    //０以下防止
    }

    //スカイドーム切り替え実行
    std::string currentChangeSkyDome = "NONE";
    if (m_nextSkyDomeIdx != SKY_DOME::NONE)currentChangeSkyDome = s_skyDomeNameArray[m_nextSkyDomeIdx];

    if (ImGui::BeginCombo("ChangeSkyDome", currentChangeSkyDome.c_str()))
    {
        for (int n = 0; n < SKY_DOME::NUM; ++n)
        {
            bool isSelected = (m_nextSkyDomeIdx == n);
            if (m_nextSkyDomeIdx == SKY_DOME::NONE && ImGui::Selectable(s_skyDomeNameArray[n].c_str(), isSelected))
            {
                ChangeSkyDome((SKY_DOME)n);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

	ImGui::End();
}