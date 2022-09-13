#include"GameUI.h"
#include"WaveUI.h"
#include"UsersInput.h"

GameUI::GameUI()
{
	timer = std::make_unique<GameTimer>();
	m_waveUI = std::make_shared<WaveUI>("resource/user/wave.png");
}

void GameUI::Init()
{
	timer->Init(120);
	timer->Start();
	m_waveUI->Init();
}

void GameUI::Update()
{
	timer->Update();
	m_waveUI->Update();




	// �f�o�b�O�p
	if (UsersInput::Instance()->KeyOnTrigger(DIK_I)) {

		m_waveUI->Appear();

	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_O)) {

		m_waveUI->Exit();

	}

}

void GameUI::Draw()
{
	timer->Draw();
	m_waveUI->Draw();

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
