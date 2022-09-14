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
	m_timer->Init(10);
	m_waveUI->Init(10);
	m_gameStartTimerUI->Init();
	m_gameEndTimerUI->Init();
	m_isStartFlag = false;
	m_endCountDownFlag = false;

	m_timer->timerPos = { 388.0f,893.0f };
	m_wavePos = { 0.0f,-2.0f };
	m_scorePos = { 45.0f,-25.0f };
}

void GameUI::Update(const int &NowWaveMaxTimer)
{
	if (UsersInput::Instance()->KeyOnTrigger(DIK_P))
	{
		ScoreMgr::Instance()->AddScore(100);
	}

	//�Q�[���J�n
	if (m_gameStartTimerUI->IsStart() && !m_isStartFlag)
	{
		m_timer->Start();
		m_waveUI->Appear();
		ScoreMgr::Instance()->Appear();
		m_isStartFlag = true;
	}
	//�I���J�E���g�_�E��
	if (m_isStartFlag && m_timer->GetFlame() * 60 <= 60 * 4 && !m_endCountDownFlag)
	{
		m_gameEndTimerUI->Start();
		m_endCountDownFlag = true;
	}
	//�Q�[���I��
	if (m_gameEndTimerUI->IsStart())
	{
		ScoreMgr::Instance()->Center();
	}

	m_nowWaveMaxTimer = NowWaveMaxTimer;


	m_timer->Update(30);
	m_waveUI->Update(m_waveNum, m_wavePos, 1.0f / 30.0f);
	ScoreMgr::Instance()->Update(m_scorePos, 1.0f / 30.0f);
	m_gameStartTimerUI->Update(m_gameStartTimerOffsetPos, 1.0f / 30.0f);
	m_gameEndTimerUI->Update(m_gameStartTimerOffsetPos, 1.0f / 30.0f);
}

void GameUI::Draw()
{
	m_timer->Draw();
	m_waveUI->Draw();
	ScoreMgr::Instance()->Draw();
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
	SetVec2("Wave_StringSize", &waveOffsetData.m_stringSize);
	SetVec2("Wave_NumPos", &waveOffsetData.m_numPos);
	SetVec2("Wave_NumSize", &waveOffsetData.m_numSize);
	SetVec2("Wave_ColorPos", &waveOffsetData.m_colonPos);
	SetVec2("Wave_ColorSize", &waveOffsetData.m_colonSize);

	SetVec2("Score_StringPos", &scoreOffsetData.m_stringPos);
	SetVec2("Score_StringSize", &scoreOffsetData.m_stringSize);
	SetVec2("Score_NumPos", &scoreOffsetData.m_numPos);
	SetVec2("Score_NumSize", &scoreOffsetData.m_numSize);
	SetVec2("Score_ColorPos", &scoreOffsetData.m_colonPos);
	SetVec2("Score_ColorSize", &scoreOffsetData.m_colonSize);
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
