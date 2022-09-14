#include"EnvironmentMgr.h"
#include"Importer.h"
#include"UsersInput.h"
#include"Object.h"
#include"LightManager.h"
#include"ColorPalette.h"

EnvironmentMgr::EnvironmentMgr() :PILLAR_POS_ANGLE_OFFSET(Angle::ROUND() / PILLAR_NUM)
{
	static const auto DIR = "resource/user/";

	m_skyDomeModelArray[STATUS::DEFAULT] = Importer::Instance()->LoadModel(DIR, "skyDome_mono.glb");
	m_skyDomeModelArray[STATUS::FEVER] = Importer::Instance()->LoadModel(DIR, "skyDome_fever.glb");

	m_pillarModelArray[STATUS::DEFAULT] = Importer::Instance()->LoadModel(DIR, "pillar_mono.glb");
	m_pillarModelArray[STATUS::FEVER] = Importer::Instance()->LoadModel(DIR, "pillar_fever.glb");

	m_ligMgr = std::make_shared<LightManager>();

	//�f�B���N�V�������C�g
	m_dirLigDef_0.SetDir({ 0,-1,0 });
	m_dirLigDef_1.SetDir({ 0,-0.5f,1 });
	m_dirLigDef_2.SetDir({ 0,0,1 });
	m_ligMgr->RegisterDirLight(&m_dirLigDef_0);
	m_ligMgr->RegisterDirLight(&m_dirLigDef_1);
	m_ligMgr->RegisterDirLight(&m_dirLigDef_2);

	m_ptLig.SetColor(ColorPalette::S_PINK_COLOR);
	m_ptLig.SetInfluenceRange(15.0f);
	m_ligMgr->RegisterPointLight(&m_ptLig);

	//�V�����C�g
	m_hemiLig.SetSkyColor(Color());
	m_hemiLig.SetGroundColor(Color(0.2f, 0.2f, 0.2f, 1.0f));
	m_ligMgr->RegisterHemiSphereLight(&m_hemiLig);

	std::vector<Vec3<float> *>posArray;
	for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
	{
		Angle posAngle = PILLAR_POS_ANGLE_OFFSET * pillarIdx;
		posArray.push_back(&m_pillarPosArray[pillarIdx]);
	}
	m_lineLight = std::make_unique<LineLight>(posArray);

	pillarColor = { 1.0f,1.0f,1.0f };
}

void EnvironmentMgr::Init()
{
	m_nextStatus = STATUS::NONE;
	m_nowStatus = STATUS::DEFAULT;
	m_statusChangeRate = 1.0f;
}

void EnvironmentMgr::Update(const Vec3<float> &PlayerPos)
{
	m_ptLig.SetPos(PlayerPos);

	//�X�e�[�^�X�؂�ւ��
	if (m_nextStatus != STATUS::NONE)
	{
		m_statusChangeRate += 1.0f / m_statusChangeTime;

		//�t�B�[�o�[����ʏ�X�e�[�^�X�ɐ؂�ւ��ۂɌ�����ǉ��ŕ`�悵�Ȃ�
		if (m_nowStatus == STATUS::FEVER && m_nextStatus != STATUS::FEVER)
		{
			m_lineLight->Stop();
		}
		//�؂�ւ�芮��
		if (1.0f <= m_statusChangeRate)
		{
			//�����t�B�[�o�[�Ȃ�����̕`����J�n����
			if (m_nextStatus == STATUS::FEVER)
			{
				m_lineLight->Init();
			}


			m_nowStatus = m_nextStatus;
			m_nextStatus = STATUS::NONE;
			m_statusChangeRate = 1.0f;
		}
	}

	m_lineLight->Update();

}

#include"DrawFunc3D.h"
#include"DrawFunc_Append.h"
void EnvironmentMgr::Draw(Camera &Cam)
{

	if (m_nextStatus == STATUS::NONE || m_statusChangeRate < 0.3f)
	{
		//�X�J�C�h�[��
		DrawFunc3D::DrawNonShadingModel(m_skyDomeModelArray[m_nowStatus], Transform(), Cam);


		Vec3<float>mainColor = ColorPalette::RGBtoHSV(pillarColor);
		Vec3<float>nextColor = ColorPalette::RGBtoHSV(larpPillarColor);
		pillarColor = KuroMath::Lerp(mainColor, nextColor, 0.1f);
		pillarColor = ColorPalette::HSVtoRGB(pillarColor);


		//��
		for (int pillarIdx = 0; pillarIdx < PILLAR_NUM; ++pillarIdx)
		{
			Transform transform;
			Angle posAngle = PILLAR_POS_ANGLE_OFFSET * pillarIdx;
			m_pillarPosArray[pillarIdx] = { cos(posAngle) * m_pillarPosRadius,m_pillarPosY,sin(posAngle) * m_pillarPosRadius };
			transform.SetPos(m_pillarPosArray[pillarIdx]);

			m_pillarModelArray[m_nowStatus]->m_meshes[0].material->constData.lambert.emissive =
			{
				pillarColor.x,
				pillarColor.y,
				pillarColor.z
			};
			m_pillarModelArray[m_nowStatus]->m_meshes[0].material->Mapping();

			DrawFunc_Append::DrawModel(m_pillarModelArray[m_nowStatus], transform);
			//DrawFunc3D::DrawNonShadingModel(m_pillarModelArray[m_nowStatus], transform, Cam);
		}
	}

	if (m_nextStatus != STATUS::NONE)
	{
		//�X�J�C�h�[���g�k
		Transform skyDomeTransform;
		float scale = KuroMath::Ease(InOut, Circ, m_statusChangeRate, 0.0f, 1.0f);
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
			m_pillarPosArray[pillarIdx] = { cos(posAngle) * posRadius,m_pillarPosY,sin(posAngle) * posRadius };
			transform.SetPos(m_pillarPosArray[pillarIdx]);


			m_pillarModelArray[m_nowStatus]->m_meshes[0].material->constData.pbr.baseColor =
			{
				pillarColor.x,
				pillarColor.y,
				pillarColor.z
			};
			m_pillarModelArray[m_nowStatus]->m_meshes[0].material->Mapping();

			//DrawFunc3D::DrawNonShadingModel(m_pillarModelArray[m_nextStatus], transform, Cam);
			DrawFunc_Append::DrawModel(m_pillarModelArray[m_nextStatus], transform);

		}
	}


	m_lineLight->Draw(Cam);


}

#include"imguiApp.h"
void EnvironmentMgr::ImguiDebug()
{
	ImGui::Begin("EnvironmentMgr");

	/*--- �X�e�[�^�X ---*/
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

	/*--- �~���� ---*/
	ImGui::Separator();

	//���W���a
	if (ImGui::DragFloat("Pillar_PosRadius", &m_pillarPosRadius, 0.01f) && m_pillarPosRadius < 1.0f)
	{
		m_pillarPosRadius = 1.0f;
	}
	//Y���W
	ImGui::DragFloat("Pillar_PosY", &m_pillarPosY, 0.5f);

	ImGui::End();
}

void EnvironmentMgr::ChangeColor(const Color &COLOR)
{
	larpPillarColor =
	{
		COLOR.m_r,
		COLOR.m_g,
		COLOR.m_b
	};
}