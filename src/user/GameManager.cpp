#include "GameManager.h"
#include"imguiApp.h"
#include"Player.h"
#include"KuroEngine.h"
#include"Fps.h"

GameManager::GameManager()
{
	RegisterCamera(m_debugCamKey, m_debugCam);
	m_debugCam.Init({ 0,0,-5.0 }, { 0,0,0 });
}

void GameManager::Update()
{
	//�f�o�b�O�J�����̍X�V
	if (!m_debugCamKey.compare(m_nowCamKey))
	{
		m_debugCam.Move();
	}
}

void GameManager::ImGuiDebug()
{
	ImGui::Begin("GameManager");

/*--- FPS�\�� ---*/
	ImGui::Text("fps : %.5f", KuroEngine::Instance()->GetFps());
	ImGui::Separator();

/*--- �f�o�b�O�`�� ---*/
	ImGui::Checkbox("DebugDraw", &m_debugDraw);

	ImGui::Separator();

/*--- �J�����I�� ---*/
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	for (auto& cam : m_cameras)
	{
		//�J�����L�[�̎擾
		const auto& camKey = cam.first;	
		if (ImGui::RadioButton(camKey.c_str(), m_nowCamKey == camKey))	//�I�����ꂽ��
		{
			//�J�����ύX
			ChangeCamera(camKey);
		}
	}
	ImGui::EndChild();

	ImGui::End();
}
