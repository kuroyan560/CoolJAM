#pragma once
#include"ImguiDebugInterFace.h"
#include<memory>
#include<string>
#include<array>
#include"Transform.h"
class Model;
class Camera;

class EnvironmentMgr : public ImguiDebugInterface
{
public:
	enum SKY_DOME { DEFAULT, FEVER, NUM, NONE = -1 };

private:
	//スカイドームモデル
	std::shared_ptr<Model>m_skyDomeModelArray[SKY_DOME::NUM];

	//現在のスカイドーム
	int m_skyDomeIdx = SKY_DOME::DEFAULT;

	//次にセットされるスカイドーム
	int m_nextSkyDomeIdx = SKY_DOME::NONE;

	//スカイドームの切り替え進行レート
	float m_skyDomeChangeRate = 0.0f;

	//スカイドーム切り替わり時間
	int m_changeSkyDomeTime = 300;

public:
	EnvironmentMgr();
	~EnvironmentMgr() {}

	void Init();
	void Update();
	void Draw(Camera& Cam);

	void ChangeSkyDome(SKY_DOME NextSkyDome)
	{
		m_nextSkyDomeIdx = NextSkyDome;
	}
	void OnImguiDebug()override;
};

static std::array<std::string, EnvironmentMgr::SKY_DOME::NUM> s_skyDomeNameArray =
{
	"DEFAULT","FEVER"
};