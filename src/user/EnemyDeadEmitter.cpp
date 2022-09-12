#include "EnemyDeadEmitter.h"
#include"../engine/Common/KuroFunc.h"
#include"KuroEngine.h"
#include"Camera.h"

std::shared_ptr<GraphicsPipeline>EnemyDeadEmitter::s_linePtGraphicsPipeline;
std::shared_ptr<GraphicsPipeline>EnemyDeadEmitter::s_squarePtGraphicsPipeline;
std::shared_ptr<TextureBuffer>EnemyDeadEmitter::s_ptTex;

void EnemyDeadEmitter::GeneratePipeline()
{
	//既に生成済
	if (s_linePtGraphicsPipeline)return;

	PipelineInitializeOption option(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	option.m_calling = D3D12_CULL_MODE_NONE;
	option.m_depthTest = false;

	std::vector<RenderTargetInfo>renderTarget =
	{
		RenderTargetInfo(D3D12App::Instance()->GetBackBuffFormat(), AlphaBlendMode_Trans),
		RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, AlphaBlendMode_Trans),
	};

	//ラインパーティクル
	{
		Shaders shaders;
		shaders.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadLineParticle.hlsl", "VSmain", "vs_6_4");
		shaders.m_gs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadLineParticle.hlsl", "GSmain", "gs_6_4");
		shaders.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadLineParticle.hlsl", "PSmain", "ps_6_4");

		std::vector<RootParam>rootParams =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"エミッター情報"),
		};

		s_linePtGraphicsPipeline = D3D12App::Instance()->GenerateGraphicsPipeline(
			option,
			shaders,
			EnemyDeadLineParticle::GetInputLayout(),
			rootParams,
			renderTarget,
			{ WrappedSampler(false,true) });
		//四角パーティクル
	}
	{
		Shaders shaders;
		shaders.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadSquareParticle.hlsl", "VSmain", "vs_6_4");
		shaders.m_gs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadSquareParticle.hlsl", "GSmain", "gs_6_4");
		shaders.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadSquareParticle.hlsl", "PSmain", "ps_6_4");

		std::vector<RootParam>rootParams =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"エミッター情報"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"パーティクル用テクスチャ"),
		};

		s_squarePtGraphicsPipeline = D3D12App::Instance()->GenerateGraphicsPipeline(
			option,
			shaders,
			EnemyDeadSquareParticle::GetInputLayout(),
			rootParams,
			renderTarget,
			{ WrappedSampler(false,false) });
	}

	//四角パーティクル用テクスチャ読み込み
	s_ptTex = D3D12App::Instance()->GenerateTextureBuffer("resource/user/enemyDeadSquareParticle.png");
}

EnemyDeadEmitter::EnemyDeadEmitter()
{
	GeneratePipeline();

	//ラインパーティクルバッファ
	m_linePtBuff = D3D12App::Instance()->GenerateVertexBuffer(
		sizeof(EnemyDeadLineParticle),
		PARTICLE_NUM,
		nullptr,
		"EnemyDeadLineParticle");

	//四角パーティクルバッファ
	m_squarePtBuff = D3D12App::Instance()->GenerateVertexBuffer(
		sizeof(EnemyDeadSquareParticle),
		PARTICLE_NUM,
		nullptr,
		"EnemyDeadSquareParticle");



	m_emitterInfo.m_linePtThickness = 1.0f;
	m_emitterInfo.m_squarePtSize = 5.0f;


	//エミッターバッファ
	m_emiBuff = D3D12App::Instance()->GenerateConstantBuffer(
		sizeof(EmitterInfo),
		1,
		&m_emitterInfo,
		"EnemyDeadEmitter");



	m_colorPatternArray[0] = ColorPalette::S_LIGHT_PURPLE_COLOR;
	m_colorPatternArray[1] = ColorPalette::S_GREEN_COLOR;
	m_colorPatternArray[2] = ColorPalette::S_DARK_PURPLE_COLOR;
	m_colorPatternArray[3] = ColorPalette::S_LIGHT_PURPLE_COLOR;
}

void EnemyDeadEmitter::Init(const Vec3<float> &POS)
{
	int colorPatternNum = KuroFunc::GetRand(0, 3);
	Color color(m_colorPatternArray[colorPatternNum]);

	//四分割して最低個数を出す
	const int PER_NUM = m_lineParticle.size() / 4;
	int nowPerCount = 0;
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		if ((i + 1) % PER_NUM == 0)
		{
			++nowPerCount;
		}
		int min = 0 + nowPerCount * 90;
		int max = min + 90;
		int random = KuroFunc::GetRand(min, max);
		m_lineParticle[i].Init(POS, KuroFunc::GetRand(1.3f, 2.0f), random, color);
	}

	nowPerCount = 0;
	for (int i = 0; i < m_SquareParticle.size(); ++i)
	{
		if ((i + 1) % PER_NUM == 0)
		{
			++nowPerCount;
		}
		int min = 0 + nowPerCount * 90;
		int max = min + 90;
		int random = KuroFunc::GetRand(min, max);
		m_SquareParticle[i].Init(POS + Vec3<float>(0.0f, 0.7f, 0.0f), KuroFunc::GetRand(0.4f, 1.0f), random, color);
	}
}

void EnemyDeadEmitter::Update()
{
	for (int i = 0; i < m_lineParticle.size(); ++i)
	{
		m_lineParticle[i].Update();
	}
	for (int i = 0; i < m_SquareParticle.size(); ++i)
	{
		m_SquareParticle[i].Update();
	}
}

void EnemyDeadEmitter::Draw(Camera &CAMERA,
	std::weak_ptr<RenderTarget>MAIN,
	std::weak_ptr<RenderTarget>EMMISIVE_MAP,
	std::weak_ptr<DepthStencil>DEPTH_STENCIL)
{
	//パーティクル情報をGPUバッファに転送
	m_linePtBuff->Mapping(m_lineParticle.data());
	m_squarePtBuff->Mapping(m_SquareParticle.data());

	//レンダーターゲットセット
	KuroEngine::Instance()->Graphics().SetRenderTargets({ MAIN.lock(),EMMISIVE_MAP.lock() }, DEPTH_STENCIL.lock());

	//ラインパーティクル描画
	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_linePtGraphicsPipeline);
	KuroEngine::Instance()->Graphics().ObjectRender(
		m_linePtBuff,
		{
			RegisterDescriptorData(CAMERA.GetBuff(),CBV),
			RegisterDescriptorData(m_emiBuff,CBV),
		},
		pos.z,
		true);

	//四角パーティクル描画
	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_squarePtGraphicsPipeline);
	KuroEngine::Instance()->Graphics().ObjectRender(
		m_squarePtBuff,
		{
			RegisterDescriptorData(CAMERA.GetBuff(),CBV),
			RegisterDescriptorData(m_emiBuff,CBV),
			RegisterDescriptorData(s_ptTex,SRV),
		},
		pos.z,
		true);
}
