#pragma once
#include"Color.h"
#include"D3D12Data.h"
#include<vector>
#include"Transform.h"
#include<memory>
#include"Camera.h"
#include"Object.h"
#include<map>

class Model;
class LightManager;
class ModelAnimator;

//レンダーターゲットへの描画スイッチ
struct RenderTargetSwitch
{
	float m_main = 1.0f;
	float m_emissive = 1.0f;
	float m_depth = 1.0f;

	RenderTargetSwitch() {}
	RenderTargetSwitch(const float& Main, const float& Emissive, const float& Depth)
		:m_main(Main), m_emissive(Emissive), m_depth(Depth) {}
};

class DrawFunc_Append
{
	static int s_drawLineCount;
	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>s_drawLinePipeline;

	static int s_drawModelCount;
	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>s_drawModelPipeline;

	static int s_drawPlaneCount;
	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>s_drawPlanePipeline;


	static std::weak_ptr<Camera>s_nowCam;
	static std::weak_ptr<LightManager>s_nowLigMgr;

	static std::weak_ptr<RenderTarget>s_main;
	static std::weak_ptr<RenderTarget>s_emissiveMap;
	static std::weak_ptr<RenderTarget>s_depthMap;
	static std::weak_ptr<DepthStencil>s_depthStencil;

	//登録されたレンダーターゲットをセット
	static void SetRegisteredTargets();

public:
	//各レンダーターゲットの登録
	static void RegisterRenderTargets(
		DXGI_FORMAT MainFormat,
		std::shared_ptr<RenderTarget>EmissiveMap,
		std::shared_ptr<RenderTarget>DepthMap,
		std::shared_ptr<DepthStencil>DepthStencil);
	//各レンダーターゲット、カメラ、ライトマネージャの登録
	static void FrameInit(
		std::shared_ptr<RenderTarget>Main,
		std::shared_ptr<Camera>NowCamera,
		std::shared_ptr<LightManager>NowLigMgr
	);

	/// <summary>
	/// 線描画
	/// </summary>
	/// <param name="From">始点</param>
	/// <param name="To">終点</param>
	/// <param name="LineColor">色</param>
	/// <param name="Thickness">太さ</param>
	/// <param name="Switch">レンダーターゲットへの描画レート（アルファ）※Depthは 0 or 1</param>
	/// <param name="BlendMode">ブレンドモード</param>
	static void DrawLine(const Vec3<float>& From, const Vec3<float>& To, const Color& LineColor, const float& Thickness,
		const RenderTargetSwitch& Switch = RenderTargetSwitch(), const AlphaBlendMode& BlendMode = AlphaBlendMode_Trans);

	/// <summary>
	/// モデル描画
	/// </summary>
	/// <param name="Model">モデル</param>
	/// <param name="Transform">モデルのトランスフォーム</param>
	/// <param name="Switch">レンダーターゲットへの描画レート（アルファ）※Depthは 0 or 1</param>
	/// <param name="Animator">モデルのアニメーター</param>
	/// <param name="BlendMode">ブレンドモード</param>
	static void DrawModel(const std::weak_ptr<Model>Model, Transform& Transform, 
		const RenderTargetSwitch& Switch = RenderTargetSwitch(),
		const bool& IsBillBoard = false,
		const bool& IsShading = true,
		std::shared_ptr<ModelAnimator> Animator = nullptr,
		AlphaBlendMode BlendMode = AlphaBlendMode_None);

	static void DrawModel(const std::weak_ptr<ModelObject>ModelObject,
		const RenderTargetSwitch& Switch = RenderTargetSwitch(), 
		const bool& IsBillBoard = false,
		const bool& IsShading = true,
		AlphaBlendMode BlendMode = AlphaBlendMode_None)
	{
		auto obj = ModelObject.lock();
		DrawModel(obj->m_model, obj->m_transform, Switch, IsBillBoard, IsShading, obj->m_animator, BlendMode);
	}

	static void DrawPlane(Transform& Transform, const std::weak_ptr<TextureBuffer>Tex, 
		const RenderTargetSwitch& Switch = RenderTargetSwitch(), AlphaBlendMode BlendMode = AlphaBlendMode_None);
};