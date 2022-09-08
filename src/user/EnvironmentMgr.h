#pragma once
#include"ImguiDebugInterFace.h"
#include<memory>
#include<string>
#include<array>
#include"Transform.h"
class Model;
class ModelObject;
class Camera;

class EnvironmentMgr : public ImguiDebugInterface
{
public:
	enum STATUS { DEFAULT, FEVER, NUM, NONE = -1 };

private:
	//スカイドームモデル
	std::shared_ptr<Model>m_skyDomeModelArray[STATUS::NUM];

	//柱モデル
	std::shared_ptr<Model>m_pillarModelArray[STATUS::NUM];
	//柱の数
	static const int PILLAR_NUM = 12;
	float m_pillarPosRadius = 160.0f;

	//現在のステータス
	int m_nowStatus = STATUS::DEFAULT;

	//次にセットされるステータス
	int m_nextStatus = STATUS::NONE;

	//ステータスの切り替え進行レート
	float m_statusChangeRate = 1.0f;

	//ステータス切り替わり時間
	int m_statusChangeTime = 250;

public:
	EnvironmentMgr();
	~EnvironmentMgr() {}

	void Init();
	void Update();
	void Draw(Camera& Cam);

	void ChangeStatus(STATUS NextStatus)
	{
		m_nextStatus = NextStatus;
		m_statusChangeRate = 0.0f;
	}
	void OnImguiDebug()override;
};

static std::array<std::string, EnvironmentMgr::STATUS::NUM> s_skyDomeNameArray =
{
	"DEFAULT","FEVER"
};