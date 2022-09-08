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

    m_floorModelObj = std::make_shared<ModelObject>(DIR, "floor.glb");
    m_floorModelObj->m_transform.SetScale(100.0f);
}

void EnvironmentMgr::Init()
{
    m_nextStatus = STATUS::NONE;
    m_nowStatus = STATUS::DEFAULT;
    m_statusChangeRate = 1.0f;
}

void EnvironmentMgr::Update()
{
	//�X�e�[�^�X�؂�ւ��
	if (m_nextStatus != STATUS::NONE)
	{
        m_statusChangeRate += 1.0f / m_statusChangeTime;

        //�؂�ւ�芮��
        if (1.0f <= m_statusChangeRate)
        {
            m_nowStatus = m_nextStatus;
            m_nextStatus = STATUS::NONE;
            m_statusChangeRate = 1.0f;
        }
	}

    //�f�o�b�O
    if(UsersInput::Instance()->KeyOnTrigger(DIK_S) && m_nextStatus == STATUS::NONE)
    {
        ChangeStatus(STATUS(1 - m_nowStatus));
    }
}

#include"DrawFunc3D.h"
void EnvironmentMgr::Draw(Camera& Cam)
{
    static const Angle PILLAR_POS_ANGLE_OFFSET = Angle::ROUND() / PILLAR_NUM;

    //��
    DrawFunc3D::DrawNonShadingModel(m_floorModelObj, Cam);

    if (m_nextStatus == STATUS::NONE ||  m_statusChangeRate < 0.02f)
    {
        //�X�J�C�h�[��
        DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_nowStatus], Transform(), Cam);

        //��
        for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
        {
            Transform transform;
            Angle posAngle = PILLAR_POS_ANGLE_OFFSET * pillarIdx;
            transform.SetPos({ cos(posAngle) * m_pillarPosRadius,0.0f,sin(posAngle) * m_pillarPosRadius });
            DrawFunc3D::DrawNonShadingModel(m_pillarModelArray[m_nowStatus], transform, Cam);
        }
    }

    if (m_nextStatus != STATUS::NONE)
    {
        //�X�J�C�h�[���g�k
        Transform skyDomeTransform;
        float scale = KuroMath::Ease(Out, Back, m_statusChangeRate, 0.0f, 1.0f);
        skyDomeTransform.SetScale(scale);
        Angle rotate = KuroMath::Ease(Out, Circ, m_statusChangeRate, Angle::ROUND() * 3.0f, 0.0f);
        skyDomeTransform.SetRotate({ 0,1,0 }, rotate);
        DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_nextStatus], skyDomeTransform, Cam);

        //����]���Ȃ���L����
        for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
        {
            Transform transform;
            Angle posAngle = rotate + Angle(PILLAR_POS_ANGLE_OFFSET * pillarIdx);
            float posRadius = KuroMath::Ease(Out, Back, m_statusChangeRate, 0.0f, m_pillarPosRadius);
            transform.SetPos({ cos(posAngle) * posRadius,0.0f,sin(posAngle) * posRadius });
            DrawFunc3D::DrawNonShadingModel(m_pillarModelArray[m_nextStatus], transform, Cam);
        }
    }
}

void EnvironmentMgr::OnImguiDebug()
{
	ImGui::Begin("EnvironmentMgr");

    //���݂̃X�e�[�^�X
    ImGui::Text("NowStatus : { %s }", s_skyDomeNameArray[m_nowStatus].c_str());

    //�X�e�[�^�X�؂�ւ����s
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

    //�X�e�[�^�X�؂�ւ�����
    if (ImGui::DragInt("StatusChangeTime", &m_statusChangeTime) && m_statusChangeTime < 1)
    {
        m_statusChangeTime = 1;    //�O�ȉ��h�~
    }

    ImGui::Separator();

    //���̍��W���a
    if (ImGui::DragFloat("Pillar_PosRadius", &m_pillarPosRadius, 0.01f) && m_pillarPosRadius < 1.0f)
    {
        m_pillarPosRadius = 1.0f;
    }

	ImGui::End();
}