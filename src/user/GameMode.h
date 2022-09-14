#pragma once
#include "Singleton.h"

class GameMode : public Singleton<GameMode> {

public:

	enum class ID {

		NONE,
		TUTORIAL,
		GAME,
		RESULT,

	};

	ID m_id = ID::NONE;

};