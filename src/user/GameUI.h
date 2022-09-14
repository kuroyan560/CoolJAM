#pragma once
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <memory>
#include <array>
#include"StageFloor.h"
#include"GameTimer.h"
#include"../engine/ImguiApp.h"

class WaveUI;
class GameStartTimerUI;

class GameUI
{
public:
	GameUI();
	void Init();
	void Update(const int& NowWaveMaxTimer);
	void Draw();

	void DrawImGui();

	void Start();
	void AddWaveNum();
	void Score(int SCORE);
private:
	std::unique_ptr<GameTimer>m_timer;
	std::shared_ptr<WaveUI> m_waveUI;
	std::shared_ptr<GameStartTimerUI> m_gameStartTimerUI;

	Vec2<float>m_wavePos;
	Vec2<float>m_scorePos;
	Vec2<float>m_gameStartTimerOffsetPos;

	void SetVec2(std::string TAG, Vec2<float>* VEC3);
	void SetVec3(std::string TAG, Vec3<Angle>* VEC3);

	int m_waveNum;
	int m_nowWaveMaxTimer;

};