#pragma once
#include"Color.h"
#include"D3D12Data.h"
#include<vector>
#include"Transform.h"
#include<memory>
#include"Camera.h"
#include"Object.h"

class Model;
class LightManager;
class ModelAnimator;

//レンダーターゲットへの描画スイッチ
struct RenderTargetSwitch
{
	bool m_main = true;
	bool m_emissive = true;
	bool m_depth = true;

	RenderTargetSwitch() {}
	RenderTargetSwitch(const bool& Main, const bool& Emissive, const bool& Depth)
		:m_main(Main), m_emissive(Emissive), m_depth(Depth) {}
};

class DrawFunc_Append
{
	static int s_drawCount;
	static std::weak_ptr<Camera>s_nowCam;
	static std::weak_ptr<LightManager>s_nowLigMgr;

public:
	//呼び出しカウントリセット
	static void FrameInit(
		std::shared_ptr<RenderTarget>Main,
		std::shared_ptr<RenderTarget>EmissiveMap,
		std::shared_ptr<RenderTarget>DepthMap,
		std::shared_ptr<DepthStencil>DepthStencil,
		std::shared_ptr<Camera>NowCamera,
		std::shared_ptr<LightManager>NowLigMgr
	);

	static void DrawModel(const std::weak_ptr<Model>Model, Transform& Transform, 
		const float& Alpha = 1.0f,
		const RenderTargetSwitch& Switch = RenderTargetSwitch(),
		std::shared_ptr<ModelAnimator> Animator = nullptr,
		AlphaBlendMode BlendMode = AlphaBlendMode_None);
	static void DrawModel(const std::weak_ptr<ModelObject>ModelObject,
		const float& Alpha = 1.0f, 
		const RenderTargetSwitch& Switch = RenderTargetSwitch(), 
		AlphaBlendMode BlendMode = AlphaBlendMode_None)
	{
		auto obj = ModelObject.lock();
		DrawModel(obj->m_model, obj->m_transform, Alpha, Switch, obj->m_animator, BlendMode);
	}
};