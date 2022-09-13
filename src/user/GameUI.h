#pragma once
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <memory>
#include <array>
#include"StageFloor.h"
#include"GameTimer.h"

class WaveUI;

class GameUI
{
public:
	GameUI();
	void Init();
	void Update();
	void Draw();

	void Start();
	void AddWaveNum();
	void Score();
private:
	std::unique_ptr<GameTimer>timer;
	std::unique_ptr<WaveUI> m_waveUI;

};