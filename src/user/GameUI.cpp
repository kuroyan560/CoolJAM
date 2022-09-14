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
	m_timer->Init(10);
	m_timer->Start();
	m_waveUI->Init(10);
	m_gameStartTimerUI->Init();
}

void GameUI::Update(const int& NowWaveMaxTimer)
{

	// デバッグ用
	if (UsersInput::Instance()->KeyOnTrigger(DIK_I)) {

		m_waveUI->Appear();

		ScoreMgr::Instance()->Appear();
	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_O)) {

		m_waveUI->Exit();

		ScoreMgr::Instance()->Exit();

	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_L)) {

		ScoreMgr::Instance()->Center();

	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_P)) {

		ScoreMgr::Instance()->AddScore(100);

	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_K)) {

		m_gameStartTimerUI->Start();

	}

	m_nowWaveMaxTimer = NowWaveMaxTimer;
	m_timer->Update(60);
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
