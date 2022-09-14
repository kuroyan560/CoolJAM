#include"GameUI.h"
#include"WaveUI.h"
#include"UsersInput.h"
#include"ScoreMgr.h"
#include "SlowMgr.h"

GameUI::GameUI()
{
	m_timer = std::make_unique<GameTimer>();
	m_waveUI = std::make_shared<WaveUI>("resource/user/wave.png");

	std::array<std::shared_ptr<TextureBuffer>, 4> timeTex;
	timeTex[0] = Font::Instance()->m_stripeFont[3];
	timeTex[1] = Font::Instance()->m_stripeFont[2];
	timeTex[2] = Font::Instance()->m_stripeFont[1];
	timeTex[3] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/go.png");
	m_gameStartTimerUI = std::make_unique<GameStartTimerUI>(timeTex);

	timeTex[0] = Font::Instance()->m_stripeFont[3];
	timeTex[1] = Font::Instance()->m_stripeFont[2];
	timeTex[2] = Font::Instance()->m_stripeFont[1];
	timeTex[3] = D3D12App::Instance()->GenerateTextureBuffer("resource/user/time_up.png");
	m_gameEndTimerUI = std::make_unique<GameStartTimerUI>(timeTex);
	m_waveNum = 0;
}

void GameUI::Init()
{
	m_timer->Init(60);
	m_waveUI->Init(10);
	m_gameStartTimerUI->Init();
	m_gameEndTimerUI->Init();
	m_isStartFlag = false;
	m_endCountDownFlag = false;
	m_resultFlag = false;

	m_timer->timerPos = { 388.0f,893.0f };
	m_wavePos = { 0.0f,-2.0f };
	m_scorePos = { 45.0f,-15.0f };

	waveOffsetData.m_stringPos = { 0.0f,-5.0f };
	waveOffsetData.m_stringSize = 0.1f;
	waveOffsetData.m_numPos = { -38.0f,0.0f };

	scoreOffsetData.m_stringPos = { -35.0f,0.0f };
	scoreOffsetData.m_stringSize = -0.1f;
	scoreOffsetData.m_numPos = { 240.0f,0.0f };
	scoreOffsetData.m_numSize = 0.2f;

	m_waveCenterOffsetPos = { 1.0f,-91.0f };
	m_scoreCenterOffsetPos = { 23.0f,239.0f };

	waveFlagData.Init();
	scoreFlagData.Init();
}

void GameUI::Update(const int &NowWaveMaxTimer)
{
	if (UsersInput::Instance()->KeyOnTrigger(DIK_P))
	{
		ScoreMgr::Instance()->AddScore(100);
	}

	//ゲーム開始
	if (m_gameStartTimerUI->IsStart() && !m_isStartFlag)
	{
		m_timer->Start();
		m_waveUI->Appear();
		ScoreMgr::Instance()->Appear();
		m_isStartFlag = true;
	}
	//終了カウントダウン
	if (m_isStartFlag && m_timer->GetFlame() * 60 <= 60 * 4 && !m_endCountDownFlag)
	{
		m_gameEndTimerUI->Start();
		m_endCountDownFlag = true;
	}

	if (m_endCountDownFlag && !m_resultFlag)
	{
		if (m_waveUI->AppearMax() && !waveFlagData.m_centerFlag)
		{
			m_waveUI->Center();
			ScoreMgr::Instance()->Center();
			waveFlagData.m_centerFlag = true;
		}
		if (m_waveUI->CenterMax() && !waveFlagData.m_exitFlag)
		{
			m_waveUI->Exit();
			ScoreMgr::Instance()->Exit();
			waveFlagData.m_exitFlag = true;
		}
	}
	else if (m_resultFlag)
	{
		if (m_waveUI->AppearMax() && !waveFlagData.m_centerFlag)
		{
			m_waveUI->Center();
			ScoreMgr::Instance()->Center();
			waveFlagData.m_centerFlag = true;
		}
	}


	//ゲーム終了
	m_nowWaveMaxTimer = NowWaveMaxTimer;


	if (m_gameEndTimerUI->IsStart() && !m_resultFlag)
	{
		m_waveUI->Init(10);
		ScoreMgr::Instance()->Init();

		ScoreMgr::Instance()->Appear();
		m_waveUI->Appear();
		m_resultFlag = true;

		waveFlagData.Init();
		scoreFlagData.Init();
	}


	m_timer->Update(30);
	m_waveUI->Update(m_waveNum, m_wavePos, m_waveCenterOffsetPos, 1.0f / 30.0f);
	ScoreMgr::Instance()->Update(m_scorePos, m_scoreCenterOffsetPos, 1.0f / 30.0f);
	m_gameStartTimerUI->Update(m_gameStartTimerOffsetPos, 1.0f / 30.0f);
	m_gameEndTimerUI->Update(m_gameStartTimerOffsetPos, 1.0f / 30.0f);
}

void GameUI::Draw()
{
	m_timer->Draw();
	m_waveUI->Draw(waveOffsetData);
	ScoreMgr::Instance()->Draw(scoreOffsetData);
	m_gameStartTimerUI->Draw();
}

void GameUI::Draw2D()
{
	m_gameEndTimerUI->Draw();
}

void GameUI::DrawImGui()
{
	ImGui::Begin("UI");
	SetVec2("Timer", &m_timer->timerPos);
	SetVec2("Wave", &m_wavePos);
	SetVec2("Score", &m_scorePos);

	SetVec2("Wave_StringPos", &waveOffsetData.m_stringPos);
	ImGui::DragFloat("Wave_StringSize", &waveOffsetData.m_stringSize);
	SetVec2("Wave_NumPos", &waveOffsetData.m_numPos);
	ImGui::DragFloat("Wave_NumSize", &waveOffsetData.m_numSize);
	SetVec2("Wave_ColorPos", &waveOffsetData.m_colonPos);
	ImGui::DragFloat("Wave_ColonSize", &waveOffsetData.m_colonSize);

	SetVec2("Score_StringPos", &scoreOffsetData.m_stringPos);
	ImGui::DragFloat("Score_StringSize", &scoreOffsetData.m_stringSize);
	SetVec2("Score_NumPos", &scoreOffsetData.m_numPos);
	ImGui::DragFloat("Score_NumSize", &scoreOffsetData.m_numSize);
	SetVec2("Score_ColorPos", &scoreOffsetData.m_colonPos);
	ImGui::DragFloat("Score_ColonSize", &scoreOffsetData.m_colonSize);

	SetVec2("m_waveCenterOffsetPos", &m_waveCenterOffsetPos);
	SetVec2("m_scoreCenterOffsetPos", &m_scoreCenterOffsetPos);
	ImGui::End();
}

void GameUI::Start()
{
	m_gameStartTimerUI->Start();
}

bool GameUI::IsResult()
{
	return m_gameEndTimerUI->IsStart();
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
