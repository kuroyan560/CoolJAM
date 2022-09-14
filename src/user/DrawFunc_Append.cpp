#include"DrawFunc_Append.h"
#include"KuroEngine.h"
#include"Model.h"
#include"ModelAnimator.h"
#include"LightManager.h"


int DrawFunc_Append::s_drawLineCount = 0;
std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>DrawFunc_Append::s_drawLinePipeline;

int DrawFunc_Append::s_drawModelCount = 0;
std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>DrawFunc_Append::s_drawModelPipeline;

int DrawFunc_Append::s_drawPlaneCount = 0;
std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>DrawFunc_Append::s_drawPlanePipeline;

std::weak_ptr<Camera>DrawFunc_Append::s_nowCam;
std::weak_ptr<LightManager>DrawFunc_Append::s_nowLigMgr;

std::weak_ptr<RenderTarget>DrawFunc_Append::s_main;
std::weak_ptr<RenderTarget>DrawFunc_Append::s_emissiveMap;
std::weak_ptr<RenderTarget>DrawFunc_Append::s_depthMap;
std::weak_ptr<DepthStencil>DrawFunc_Append::s_depthStencil;

void DrawFunc_Append::SetRegisteredTargets()
{
	KuroEngine::Instance()->Graphics().SetRenderTargets(
		{ s_main.lock(),s_emissiveMap.lock(),s_depthMap.lock() }, s_depthStencil.lock());
}

void DrawFunc_Append::RegisterRenderTargets(DXGI_FORMAT MainFormat, std::shared_ptr<RenderTarget> EmissiveMap, std::shared_ptr<RenderTarget> DepthMap, std::shared_ptr<DepthStencil> DepthStencil)
{
	s_emissiveMap = EmissiveMap;
	s_depthMap = DepthMap;
	s_depthStencil = DepthStencil;

	//メインターゲットのフォーマット取得
	const auto mainFormat = MainFormat;

	//既に生成済か検索
	auto searchPipeline = s_drawLinePipeline.find(mainFormat);
	if (searchPipeline != s_drawLinePipeline.end())return;	//生成済

	//パイプライン未生成
	for (int blendModeIdx = 0; blendModeIdx < AlphaBlendModeNum; ++blendModeIdx)
	{
		AlphaBlendMode blendMode = AlphaBlendMode(blendModeIdx);

		//レンダーターゲット描画先情報
		std::vector<RenderTargetInfo>RENDER_TARGET_INFO =
		{
			RenderTargetInfo(mainFormat, blendMode),
			RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, AlphaBlendMode_Trans),
			RenderTargetInfo(DXGI_FORMAT_R32_FLOAT, AlphaBlendMode_None)
		};

		//DrawLine用
		{
			//パイプライン設定
			static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			PIPELINE_OPTION.m_calling = D3D12_CULL_MODE_NONE;

			//シェーダー情報
			static Shaders SHADERS;
			SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawLine3D_Append.hlsl", "VSmain", "vs_6_4");
			SHADERS.m_gs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawLine3D_Append.hlsl", "GSmain", "gs_6_4");
			SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawLine3D_Append.hlsl", "PSmain", "ps_6_4");

			//インプットレイアウト
			static std::vector<InputLayoutParam>INPUT_LAYOUT =
			{
				InputLayoutParam("FROM_POS",DXGI_FORMAT_R32G32B32_FLOAT),
				InputLayoutParam("TO_POS",DXGI_FORMAT_R32G32B32_FLOAT),
				InputLayoutParam("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT),
				InputLayoutParam("DRAW_RATE",DXGI_FORMAT_R32G32B32_FLOAT),
				InputLayoutParam("THICKNESS",DXGI_FORMAT_R32_FLOAT),
			};

			//ルートパラメータ
			static std::vector<RootParam>ROOT_PARAMETER =
			{
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報バッファ"),
			};

			//パイプライン生成
			s_drawLinePipeline[mainFormat][blendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, true) });
		}

		//DrawModel用
		{
			//パイプライン設定
			static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//シェーダー情報
			static Shaders SHADERS;
			SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawModel_Append.hlsl", "VSmain", "vs_6_4");
			SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawModel_Append.hlsl", "PSmain", "ps_6_4");

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

			//パイプライン生成
			s_drawModelPipeline[mainFormat][blendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, ModelMesh::Vertex::GetInputLayout(), ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, true) });
		}

		//DrawPlane用
		{
			//パイプライン設定
			static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			//シェーダー情報
			static Shaders SHADERS;
			SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawPlane_Append.hlsl", "VSmain", "vs_6_4");
			SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawPlane_Append.hlsl", "PSmain", "ps_6_4");

			//ルートパラメータ
			static std::vector<RootParam>ROOT_PARAMETER =
			{
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報バッファ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"描画データバッファ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"テクスチャ"),
			};

			static std::vector<InputLayoutParam>INPUT_LAYOUT =
			{
				InputLayoutParam("POSITION",DXGI_FORMAT_R32G32B32_FLOAT),
				InputLayoutParam("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
			};

			//パイプライン生成
			s_drawPlanePipeline[mainFormat][blendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, true) });
		}
	}
}

void DrawFunc_Append::FrameInit(
	std::shared_ptr<RenderTarget>Main,
	std::shared_ptr<Camera>NowCamera,
	std::shared_ptr<LightManager>NowLigMgr)
{
	s_drawModelCount = 0;
	s_drawLineCount = 0;
	s_drawPlaneCount = 0;

	s_main = Main;

	//登録されたフォーマットと異なる
	if (s_drawLinePipeline.find(s_main.lock()->GetDesc().Format) == s_drawLinePipeline.end())assert(0);

	KuroEngine::Instance()->Graphics().ClearRenderTarget(s_main.lock());
	KuroEngine::Instance()->Graphics().ClearRenderTarget(s_emissiveMap.lock());
	KuroEngine::Instance()->Graphics().ClearRenderTarget(s_depthMap.lock());
	KuroEngine::Instance()->Graphics().ClearDepthStencil(s_depthStencil.lock());

	s_nowCam = NowCamera;
	s_nowLigMgr = NowLigMgr;

	SetRegisteredTargets();
}

void DrawFunc_Append::DrawLine(const Vec3<float>& From, const Vec3<float>& To, const Color& LineColor, const float& Thickness, const RenderTargetSwitch& Switch, const AlphaBlendMode& BlendMode)
{
	if (!LineColor.m_a)return;

	static std::vector<std::shared_ptr<VertexBuffer>>LINE_VERTEX_BUFF;

	SetRegisteredTargets();

	//DrawLine専用頂点
	class LineVertex
	{
	public:
		Vec3<float>m_fromPos;
		Vec3<float>m_toPos;
		Color m_color;
		Vec3<float>m_drawRate;
		float m_thickness;
		LineVertex(const Vec3<float>& FromPos, const Vec3<float>& ToPos, const Color& Color,
			const float& Thickness, const RenderTargetSwitch& TargetSwitch)
			:m_fromPos(FromPos), m_toPos(ToPos), m_color(Color), 
			m_drawRate({ TargetSwitch.m_main,TargetSwitch.m_emissive,TargetSwitch.m_depth}), m_thickness(Thickness) {}
	};

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_drawLinePipeline[s_main.lock()->GetDesc().Format][BlendMode]);

	if (LINE_VERTEX_BUFF.size() < (s_drawLineCount + 1))
	{
		LINE_VERTEX_BUFF.emplace_back(D3D12App::Instance()->GenerateVertexBuffer(sizeof(LineVertex), 1, nullptr, ("DrawLine3D_Append -" + std::to_string(s_drawLineCount)).c_str()));
	}

	LineVertex vertex(From, To, LineColor, Thickness, Switch);
	LINE_VERTEX_BUFF[s_drawLineCount]->Mapping(&vertex);
	Vec3<float>center = From.GetCenter(To);

	KuroEngine::Instance()->Graphics().ObjectRender(
		LINE_VERTEX_BUFF[s_drawLineCount],
		{
			{s_nowCam.lock()->GetBuff(),CBV}
		},
		center.z,
		true);

	s_drawLineCount++;
}

void DrawFunc_Append::DrawModel(const std::weak_ptr<Model>Model, Transform& Transform,
	const RenderTargetSwitch& Switch,
	const bool& IsBillBoard,
	const bool& IsShading,
	std::shared_ptr<ModelAnimator> Animator, 
	AlphaBlendMode BlendMode)
{
	static std::vector<std::shared_ptr<ConstantBuffer>>DRWA_DATA_BUFF;

	struct DrawData
	{
		Matrix m_transformMat;
		RenderTargetSwitch m_drawSwitch;
		int m_isShading;
	};

	if (DRWA_DATA_BUFF.size() < (s_drawModelCount + 1))
	{
		DRWA_DATA_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(DrawData), 1, nullptr, ("DrawModel_Append_Model_DrawData -" + std::to_string(s_drawModelCount)).c_str()));
	}

	DrawData drawData;
	drawData.m_transformMat = Transform.GetMat(IsBillBoard ? s_nowCam.lock()->GetBillBoardMat() : XMMatrixIdentity());
	drawData.m_drawSwitch = Switch;
	drawData.m_isShading = IsShading ? 1 : 0;
	DRWA_DATA_BUFF[s_drawModelCount]->Mapping(&drawData);

	SetRegisteredTargets();

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_drawModelPipeline[s_main.lock()->GetDesc().Format][BlendMode]);

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
				{DRWA_DATA_BUFF[s_drawModelCount],CBV},
				{boneBuff,CBV},
				{mesh.material->texBuff[COLOR_TEX],SRV},
				{mesh.material->texBuff[EMISSIVE_TEX],SRV},
				{mesh.material->buff,CBV}
			},
			Transform.GetPos().z,
			BlendMode == AlphaBlendMode_Trans || Switch.m_main < 1.0f);
	}

	s_drawModelCount++;
}

void DrawFunc_Append::DrawPlane(Transform& Transform, const std::weak_ptr<TextureBuffer> Tex, const RenderTargetSwitch& Switch, AlphaBlendMode BlendMode)
{
	struct Vertex
	{
		Vec3<float>m_pos;
		Vec2<float>m_uv;
		Vertex(const Vec3<float>& Pos, const Vec2<float>& Uv)
			:m_pos(Pos), m_uv(Uv) {}
	};
	static std::array<Vertex, 4>s_planeVerticies =
	{
		Vertex({ -1,0,-1 },{0.0f,1.0f}),	//LB
		Vertex({ -1,0,1 },{0.0f,0.0f}),	//LT
		Vertex({ 1,0,-1 },{1.0f,1.0f}),	//RB
		Vertex({ 1,0,1 },{1.0f,0.0f}),	//RT
	};

	static std::shared_ptr<VertexBuffer>s_vertBuff =
		D3D12App::Instance()->GenerateVertexBuffer(sizeof(Vertex), 4, s_planeVerticies.data(), "DrawAppend - DrawPlane - VertexBuffer");

	struct DrawData
	{
		Matrix m_transformMat;
		RenderTargetSwitch m_drawSwitch;
	};

	static std::vector<std::shared_ptr<ConstantBuffer>>DRAW_DATA_BUFF;

	if (DRAW_DATA_BUFF.size() < (s_drawPlaneCount + 1))
	{
		DRAW_DATA_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(DrawData), 1, nullptr, ("DrawModel_Append_Plane_DrawData -" + std::to_string(s_drawPlaneCount)).c_str()));
	}

	DrawData drawData;
	drawData.m_transformMat = Transform.GetMat();
	drawData.m_drawSwitch = Switch;
	DRAW_DATA_BUFF[s_drawPlaneCount]->Mapping(&drawData);

	SetRegisteredTargets();

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_drawPlanePipeline[s_main.lock()->GetDesc().Format][BlendMode]);

	KuroEngine::Instance()->Graphics().ObjectRender(
		s_vertBuff,
		{
			{s_nowCam.lock()->GetBuff(),CBV},
			{DRAW_DATA_BUFF[s_drawPlaneCount],CBV},
			{Tex.lock(),SRV},
		},
		Transform.GetPos().z,
		BlendMode == AlphaBlendMode_Trans || Switch.m_main < 1.0f
		);

	s_drawPlaneCount++;
}
