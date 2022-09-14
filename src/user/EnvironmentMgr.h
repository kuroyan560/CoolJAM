
#pragma once
#include<memory>
#include<string>
#include<array>
#include"Transform.h"
#include"LineLight.h"

#include"Light.h"
class Model;
class ModelObject;
class Camera;
class LightManager;

class EnvironmentMgr
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
	Light::Direction m_dirLigDef_0;
	Light::Direction m_dirLigDef_1;
	Light::Direction m_dirLigDef_2;
	Light::HemiSphere m_hemiLig;

	//プレイヤーにアタッチするポイントライト
	Light::Point m_ptLig;

	//柱の座標
	std::array<Vec3<float>, PILLAR_NUM> m_pillarPosArray;

	//光線管理
	std::unique_ptr<LineLight> m_lineLight;

	const Angle PILLAR_POS_ANGLE_OFFSET;

	Vec3<float> pillarColor;
	Vec3<float> larpPillarColor;
public:
	EnvironmentMgr();
	~EnvironmentMgr() {}

	void Init();
	void Update(const Vec3<float>&PlayerPos);
	void Draw(Camera& Cam);

	//環境ステータス変更
	void ChangeStatus(STATUS NextStatus)
	{
		m_nextStatus = NextStatus;
		m_statusChangeRate = 0.0f;
	}

	void ChangeColor(const Color &COLOR);

	//ゲッタ
	std::shared_ptr<LightManager>& GetLigMgr() { return m_ligMgr; }
	int GetNowStatus() { return m_nowStatus; }


	void ImguiDebug();
};

static std::array<std::string, EnvironmentMgr::STATUS::NUM> s_skyDomeNameArray =
{
	"DEFAULT","FEVER"
};