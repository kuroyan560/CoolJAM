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
	m_waveUI->Init();
}

void GameUI::Update()
{
	timer->Update();
	m_waveUI->Update();




	// デバッグ用
	if (UsersInput::Instance()->KeyOnTrigger(DIK_I)) {

		m_waveUI->Appear();

	}
	if (UsersInput::Instance()->KeyOnTrigger(DIK_O)) {

		m_waveUI->Exit();

	}

}

void GameUI::Draw()
{
	// レンダーターゲットをセット。
	StageFloor::Instance()->ScreenTargetSet(true);

	timer->Draw();
	m_waveUI->Draw();

}