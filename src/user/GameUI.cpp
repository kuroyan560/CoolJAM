#include"GameUI.h"
#include"WaveUI.h"
#include"UsersInput.h"
#include"ScoreMgr.h"
#include "SlowMgr.h"
#include"GameStartTimerUI.h"

GameUI::GameUI()
{
	m_timer = std::make_unique<GameTimer>();
	m_waveUI = std::make_shared<WaveUI>("resource/user/wave.png");
	m_gameStartTimerUI = std::make_shared<GameStartTimerUI>();
	m_waveNum = 0;
}

void GameUI::Init()
{
	m_timer->Init(120);
	m_waveUI->Init(10);
	m_gameStartTimerUI->Init();
	m_isStartFlag = false;

	m_timer->timerPos = { 388.0f,893.0f };
	m_wavePos = { 0.0f,-2.0f };
	m_scorePos = { 45.0f,-25.0f };
}

void GameUI::Update(const int& NowWaveMaxTimer)
{
	if (UsersInput::Instance()->KeyOnTrigger(DIK_P))
	{
		ScoreMgr::Instance()->AddScore(100);
	}

	if (m_gameStartTimerUI->IsStart() && !m_isStartFlag)
	{
		m_timer->Start();
		m_waveUI->Appear();
		ScoreMgr::Instance()->Appear();
		m_isStartFlag = true;
	}

	m_nowWaveMaxTimer = NowWaveMaxTimer;
	m_timer->Update(30);
	m_waveUI->Update(m_waveNum, m_wavePos, 1.0f / 30.0f);
	ScoreMgr::Instance()->Update(m_scorePos, 1.0f / 30.0f);
	m_gameStartTimerUI->Update(m_gameStartTimerOffsetPos, 1.0f / 30.0f);
}

void GameUI::Draw()
{
	m_timer->Draw();
	m_waveUI->Draw();
	ScoreMgr::Instance()->Draw();
	m_gameStartTimerUI->Draw();
}

void GameUI::DrawImGui()
{
	ImGui::Begin("UI");
	SetVec2("Timer", &m_timer->timerPos);
	SetVec2("Wave", &m_wavePos);
	SetVec2("Score", &m_scorePos);
	ImGui::End();
}

void GameUI::Start()
{
	m_gameStartTimerUI->Start();
}

void GameUI::AddWaveNum()
{
}

void GameUI::Score(int SCORE)
{
}

void GameUI::Result()
{
	ScoreMgr::Instance()->Center();
	m_waveUI->Exit();
}

void GameUI::Finish()
{
	ScoreMgr::Instance()->Exit();
}

void GameUI::SetVec2(std::string TAG, Vec2<float>* VEC3)
{
	std::string name = TAG + "X";
	ImGui::DragFloat(name.c_str(), &VEC3->x);
	name = TAG + "Y";
	ImGui::DragFloat(name.c_str(), &VEC3->y);
}

void GameUI::SetVec3(std::string TAG, Vec3<Angle>* VEC3)
{
	std::string name = TAG + "X";
	ImGui::DragFloat(name.c_str(), &VEC3->x);
	name = TAG + "Y";
	ImGui::DragFloat(name.c_str(), &VEC3->y);
	name = TAG + "Z";
	ImGui::DragFloat(name.c_str(), &VEC3->z);
}
