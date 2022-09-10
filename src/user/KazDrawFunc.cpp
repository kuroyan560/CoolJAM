#include "KazDrawFunc.h"

int KazDrawFunc::s_drawNonShadingModelSignalColor = 0;
int KazDrawFunc::s_drawBillBoardGraph = 0;

void KazDrawFunc::CountReset()
{
	s_drawNonShadingModelSignalColor = 0;
	s_drawBillBoardGraph = 0;
};

void KazDrawFunc::DrawNonShadingModelSignalColor(const std::weak_ptr<Model> Model, Transform &Transform, Color &color, Camera &Cam, std::shared_ptr<ModelAnimator> Animator, const AlphaBlendMode &BlendMode)
{
	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>PIPELINE;
	static std::vector<std::shared_ptr<ConstantBuffer>>TRANSFORM_BUFF;
	static std::vector<std::shared_ptr<ConstantBuffer>>COLOR_BUFF;

	const auto targetFormat = KuroEngine::Instance()->Graphics().GetRecentRenderTargetFormat(0);

	//パイプライン未生成
	if (!PIPELINE[targetFormat][BlendMode])
	{
		//パイプライン設定
		static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダー情報
		static Shaders SHADERS;
		SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/engine/DrawNonShadingModel.hlsl", "VSmain", "vs_6_4");
		SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawNonShadingModelSingleColor.hlsl", "PSmain", "ps_6_4");

		//ルートパラメータ
		static std::vector<RootParam>ROOT_PARAMETER =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"トランスフォームバッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"ボーン行列バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"カラーテクスチャ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"マテリアル基本情報バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"色"),
		};

		//レンダーターゲット描画先情報;
		std::vector<RenderTargetInfo>RENDER_TARGET_INFO = { RenderTargetInfo(targetFormat, BlendMode) };
		//パイプライン生成
		PIPELINE[targetFormat][BlendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, ModelMesh::Vertex::GetInputLayout(), ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, false) });
	}

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(PIPELINE[targetFormat][BlendMode]);

	if (TRANSFORM_BUFF.size() < (s_drawNonShadingModelSignalColor + 1))
	{
		TRANSFORM_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(Matrix), 1, nullptr, ("DrawNonShadingModel_Transform -" + std::to_string(s_drawNonShadingModelSignalColor)).c_str()));
	}
	if (COLOR_BUFF.size() < (s_drawNonShadingModelSignalColor + 1))
	{
		COLOR_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(DirectX::XMFLOAT4), 1, nullptr, ("Color" + std::to_string(s_drawNonShadingModelSignalColor)).c_str()));
	}

	TRANSFORM_BUFF[s_drawNonShadingModelSignalColor]->Mapping(&Transform.GetMat());
	COLOR_BUFF[s_drawNonShadingModelSignalColor]->Mapping(&color);

	auto model = Model.lock();
	std::shared_ptr<ConstantBuffer>boneBuff;
	if (Animator)boneBuff = Animator->GetBoneMatBuff();

	for (int meshIdx = 0; meshIdx < model->m_meshes.size(); ++meshIdx)
	{
		const auto &mesh = model->m_meshes[meshIdx];
		KuroEngine::Instance()->Graphics().ObjectRender(
			mesh.mesh->vertBuff,
			mesh.mesh->idxBuff,
			{
				{Cam.GetBuff(),CBV},
				{TRANSFORM_BUFF[s_drawNonShadingModelSignalColor],CBV},
				{boneBuff,CBV},
				{mesh.material->texBuff[COLOR_TEX],SRV},
				{mesh.material->buff,CBV},
				{COLOR_BUFF[s_drawNonShadingModelSignalColor],CBV}
			},
			Transform.GetPos().z,
			true);
	}

	s_drawNonShadingModelSignalColor++;
}

void KazDrawFunc::DrawBillBoardGraph(Camera &Cam, const Vec3<float> &Pos, const Vec2<float> &Size, std::shared_ptr<TextureBuffer> Tex, const AlphaBlendMode &BlendMode)
{
	static std::vector<std::shared_ptr<VertexBuffer>>s_graphVertBuff;
	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>PIPELINE;

	const auto targetFormat = KuroEngine::Instance()->Graphics().GetRecentRenderTargetFormat(0);


	if (!PIPELINE[targetFormat][BlendMode])
	{
		//パイプライン設定
		static PipelineInitializeOption s_pipelineOption(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		s_pipelineOption.m_calling = false;

		//シェーダー情報
		static Shaders s_shaders;
		s_shaders.m_vs = D3D12App::Instance()->CompileShader("resource/engine/DrawBillBoard.hlsl", "VSmain", "vs_6_4");
		s_shaders.m_gs = D3D12App::Instance()->CompileShader("resource/engine/DrawBillBoard.hlsl", "GSmain", "gs_6_4");
		s_shaders.m_ps = D3D12App::Instance()->CompileShader("resource/engine/DrawBillBoard.hlsl", "PSmain", "ps_6_4");

		//インプットレイアウト
		static std::vector<InputLayoutParam>s_inputLayOut =
		{
			InputLayoutParam("POS",DXGI_FORMAT_R32G32B32_FLOAT),
			InputLayoutParam("SIZE",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT),
		};

		//ルートパラメータ
		static std::vector<RootParam>s_rootParams =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"テクスチャ"),
		};

		//レンダーターゲット描画先情報
		std::vector<RenderTargetInfo>s_renderTargetInfo = { RenderTargetInfo(targetFormat, BlendMode) };
		//パイプライン生成
		PIPELINE[targetFormat][BlendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(s_pipelineOption, s_shaders, s_inputLayOut, s_rootParams, s_renderTargetInfo, { WrappedSampler(false, false) });
	}

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(PIPELINE[targetFormat][BlendMode]);

	if (s_graphVertBuff.size() < (s_drawBillBoardGraph + 1))
	{
		s_graphVertBuff.emplace_back(D3D12App::Instance()->GenerateVertexBuffer(sizeof(Vertex), 1, nullptr, ("DrawGraphBillBoard -" + std::to_string(s_drawBillBoardGraph)).c_str()));
	}

	Vertex vertex(Pos, Size, Color());
	s_graphVertBuff[s_drawBillBoardGraph]->Mapping(&vertex);

	KuroEngine::Instance()->Graphics().ObjectRender(
		s_graphVertBuff[s_drawBillBoardGraph],
		{
			{Cam.GetBuff(),CBV},
			{Tex,SRV}
		},
		Pos.z,
		true);

	s_drawBillBoardGraph++;
}
