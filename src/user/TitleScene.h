#pragma once
#include"Vec.h"
#include"KuroEngine.h"
#include<memory>
#include"LightBloomDevice.h"

class Player;
class Camera;
class DepthStencil;
class EnvironmentMgr;
class RenderTarget;

class TitleScene : public BaseScene {

private:

	std::unique_ptr<Player> m_player;	// プレイヤー

	// タイトルカメラ
	const std::string m_gameCamKey = "TitleCamera";
	std::shared_ptr<Camera>m_gameCam;

	//カメラ関係。
	Vec3<float> m_baseEye;
	Vec3<float> m_nowEye;
	Vec3<float> m_baseTarget;
	Vec3<float> m_nowTarget;

	//デプスステンシル
	std::shared_ptr<DepthStencil>m_depthStencil;

	//エミッシブマップ
	std::shared_ptr<RenderTarget>m_emissiveMap;

	//デプスマップ
	std::shared_ptr<RenderTarget>m_depthMap;

	//環境統括
	std::unique_ptr<EnvironmentMgr>m_environmentMgr;

	//シーン遷移
	std::unique_ptr<SceneTransition> m_sceneTransition;

	//ライトブルーム
	LightBloomDevice m_ligBloomDev;

	bool m_emissive = true;

	bool m_isTransition;

	// イージングが終わってからゲームシーンに遷移するまでのタイマー
	int m_endEasingTransitionTimer;
	const int END_EASING_TIMER = 90;

	// 遷移時のイージングタイマー
	float m_transitionEasingTimer;
	const float ADD_TRANSITION_EASING_TIMER = 0.01f;

	// カメラの初期位置
	Vec3<float> DEF_TARGET_POS = Vec3<float>(3.19660044f, 0.100951008f, -4.14727974f);
	Vec3<float> DEF_EYE_POS = Vec3<float>(9.01069641f, 2.39467144f, 6.33004093f);
	const float DEF_ANGLE = 0.506614208f;
	const float DEF_LENGTH = 10.9236498f;

	// カメラの補間後の位置
	Vec3<float> END_TARGET_POS = Vec3<float>(0.0f, 0.0f, 0.0f);
	Vec3<float> END_EYE_POS = Vec3<float>(80.0f, 80.0f, 0.0f);
	const float END_ANGLE = 0;
	const float CHANGE_ANGLE = DEF_ANGLE - DEF_ANGLE + END_ANGLE;	// 角度の保管量
	const float END_LENGTH = 80.0f;


public:

	TitleScene();
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

};