#include"GameUI.h"
#include"WaveUI.h"
#include"UsersInput.h"

GameUI::GameUI()
{
	m_timer = std::make_unique<GameTimer>();
	m_waveUI = std::make_shared<WaveUI>("resource/user/wave.png");
	m_waveNum = 0;
}

void GameUI::Init()
{
	m_timer->Init(120);
	m_timer->Start();
	m_waveUI->Init(10);
}

void GameUI::Update()
{
	m_timer->Update();
	m_waveUI->Update(m_waveNum, m_wavePos);

	// デバッグ用
	if (UsersInput::Instance()->KeyOnTrigger(DIK_I)) {

		m_waveUI->Appear();
	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_O)) {

		m_waveUI->Exit();

	}

	m_timer->Update();
	m_waveUI->Update(m_waveNum, m_wavePos);
}

void GameUI::Draw()
{
	m_timer->Draw();
	m_waveUI->Draw();
}

void GameUI::DrawImGui()
{
	ImGui::Begin("UI");
	SetVec2("Timer", &m_timer->timerPos);
	SetVec2("Wave", &m_wavePos);
	ImGui::End();
}

void GameUI::Start()
{
}

void GameUI::AddWaveNum()
{
}

void GameUI::Score(int SCORE)
{
}

void GameUI::SetVec2(std::string TAG, Vec2<float> *VEC3)
{
	std::string name = TAG + "X";
	ImGui::DragFloat(name.c_str(), &VEC3->x);
	name = TAG + "Y";
	ImGui::DragFloat(name.c_str(), &VEC3->y);
}

void GameUI::SetVec3(std::string TAG, Vec3<Angle> *VEC3)
{
	std::string name = TAG + "X";
	ImGui::DragFloat(name.c_str(), &VEC3->x);
	name = TAG + "Y";
	ImGui::DragFloat(name.c_str(), &VEC3->y);
	name = TAG + "Z";
	ImGui::DragFloat(name.c_str(), &VEC3->z);
}
