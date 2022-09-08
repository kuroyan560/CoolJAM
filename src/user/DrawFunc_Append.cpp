#include"DrawFunc_Append.h"
#include"KuroEngine.h"
#include"Model.h"
#include"ModelAnimator.h"
#include"LightManager.h"

int DrawFunc_Append::s_drawCount = 0;
std::weak_ptr<Camera>DrawFunc_Append::s_nowCam;
std::weak_ptr<LightManager>DrawFunc_Append::s_nowLigMgr;

void DrawFunc_Append::FrameInit(
	std::shared_ptr<RenderTarget>Main, 
	std::shared_ptr<RenderTarget>EmissiveMap,
	std::shared_ptr<RenderTarget>DepthMap,
	std::shared_ptr<DepthStencil>DepthStencil,
	std::shared_ptr<Camera>NowCamera,
	std::shared_ptr<LightManager>NowLigMgr)
{
	s_drawCount = 0;

	KuroEngine::Instance()->Graphics().ClearRenderTarget(Main);
	KuroEngine::Instance()->Graphics().ClearRenderTarget(EmissiveMap);
	KuroEngine::Instance()->Graphics().ClearRenderTarget(DepthMap);
	KuroEngine::Instance()->Graphics().ClearDepthStencil(DepthStencil);

	KuroEngine::Instance()->Graphics().SetRenderTargets(
		{ Main,EmissiveMap,DepthMap }, DepthStencil);

	s_nowCam = NowCamera;
	s_nowLigMgr = NowLigMgr;
}

void DrawFunc_Append::DrawModel(const std::weak_ptr<Model>Model, Transform& Transform,
	const float& Alpha,
	const RenderTargetSwitch& Switch,
	std::shared_ptr<ModelAnimator> Animator, 
	AlphaBlendMode BlendMode)
{
	if (!Alpha)return;

	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>PIPELINE;
	static std::vector<std::shared_ptr<ConstantBuffer>>DRWA_DATA_BUFF;

	struct DrawData
	{
		Matrix m_transformMat;
		float m_alpha = 1.0f;
		RenderTargetSwitch m_drawSwitch;
	};

	const auto targetFormat = KuroEngine::Instance()->Graphics().GetRecentRenderTargetFormat(0);

	//パイプライン未生成
	if (!PIPELINE[targetFormat][BlendMode])
	{
		//パイプライン設定
		static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダー情報
		static Shaders SHADERS;
		SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/DrawModel_Append.hlsl", "VSmain", "vs_6_4");
		SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/DrawModel_Append.hlsl", "PSmain", "ps_6_4");

		//ルートパラメータ
		static std::vector<RootParam>ROOT_PARAMETER =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, "アクティブ中のライト数バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "ディレクションライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "ポイントライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "スポットライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "天球ライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"描画データバッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"ボーン行列バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"カラーテクスチャ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"エミッシブテクスチャ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"マテリアル基本情報バッファ"),
		};

		//レンダーターゲット描画先情報
		std::vector<RenderTargetInfo>RENDER_TARGET_INFO =
		{
			RenderTargetInfo(targetFormat, BlendMode),
			RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, BlendMode),
			RenderTargetInfo(DXGI_FORMAT_R32_FLOAT, AlphaBlendMode_None)
		};
		//パイプライン生成
		PIPELINE[targetFormat][BlendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, ModelMesh::Vertex::GetInputLayout(), ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, false) });
	}

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(PIPELINE[targetFormat][BlendMode]);

	if (DRWA_DATA_BUFF.size() < (s_drawCount + 1))
	{
		DRWA_DATA_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(DrawData), 1, nullptr, ("DrawModel_Append_DrawData -" + std::to_string(s_drawCount)).c_str()));
	}

	DrawData drawData;
	drawData.m_transformMat = Transform.GetMat();
	drawData.m_alpha = Alpha;
	DRWA_DATA_BUFF[s_drawCount]->Mapping(&drawData);

	auto model = Model.lock();
	std::shared_ptr<ConstantBuffer>boneBuff;
	if (Animator)boneBuff = Animator->GetBoneMatBuff();

	auto ligMgr = s_nowLigMgr.lock();

	for (int meshIdx = 0; meshIdx < model->m_meshes.size(); ++meshIdx)
	{
		const auto& mesh = model->m_meshes[meshIdx];
		KuroEngine::Instance()->Graphics().ObjectRender(
			mesh.mesh->vertBuff,
			mesh.mesh->idxBuff,
			{
				{s_nowCam.lock()->GetBuff(),CBV},
				{ligMgr->GetLigNumInfo(),CBV},
				{ligMgr->GetLigInfo(Light::DIRECTION),SRV},
				{ligMgr->GetLigInfo(Light::POINT),SRV},
				{ligMgr->GetLigInfo(Light::SPOT),SRV},
				{ligMgr->GetLigInfo(Light::HEMISPHERE),SRV},
				{DRWA_DATA_BUFF[s_drawCount],CBV},
				{boneBuff,CBV},
				{mesh.material->texBuff[COLOR_TEX],SRV},
				{mesh.material->texBuff[EMISSIVE_TEX],SRV},
				{mesh.material->buff,CBV}
			},
			Transform.GetPos().z,
			BlendMode == AlphaBlendMode_Trans || Alpha < 1.0f);
	}

	s_drawCount++;
}
