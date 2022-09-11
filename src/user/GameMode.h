#pragma once
#include "Singleton.h"

class GameMode : public Singleton<GameMode> {

public:

	bool m_isTutorial;
	bool m_isGame;

};