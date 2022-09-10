#pragma once
#include"ImguiDebugInterFace.h"
#include<memory>
#include<string>
#include<array>
#include"Transform.h"
#include"Light.h"
class Model;
class ModelObject;
class Camera;
class LightManager;

class EnvironmentMgr : public ImguiDebugInterface
{
public:
	enum STATUS { DEFAULT, FEVER, NUM, NONE = -1 };

private:
	//スカイドームモデル
	std::array<std::shared_ptr<Model>, STATUS::NUM>m_skyDomeModelArray;

	//柱モデル
	std::array<std::shared_ptr<Model>, STATUS::NUM>m_pillarModelArray;
	//柱の数
	static const int PILLAR_NUM = 12;
	float m_pillarPosRadius = 160.0f;
	float m_pillarPosY = -50.0f;

	//現在のステータス
	int m_nowStatus = STATUS::DEFAULT;

	//次にセットされるステータス
	int m_nextStatus = STATUS::NONE;

	//ステータスの切り替え進行レート
	float m_statusChangeRate = 1.0f;

	//ステータス切り替わり時間
	int m_statusChangeTime = 250;

	//ライトマネージャ
	std::shared_ptr<LightManager>m_ligMgr;

	//ディレクションライト
	Light::Direction m_dirLigDef;

public:
	EnvironmentMgr();
	~EnvironmentMgr() {}

	void Init();
	void Update();
	void Draw(Camera& Cam);

	//環境ステータス変更
	void ChangeStatus(STATUS NextStatus)
	{
		m_nextStatus = NextStatus;
		m_statusChangeRate = 0.0f;
	}

	//ゲッタ
	std::shared_ptr<LightManager>& GetLigMgr() { return m_ligMgr; }

	void OnImguiDebug()override;
};

static std::array<std::string, EnvironmentMgr::STATUS::NUM> s_skyDomeNameArray =
{
	"DEFAULT","FEVER"
};