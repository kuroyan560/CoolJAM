#include "EnemyDeadEmitter.h"
#include"../engine/Common/KuroFunc.h"
#include"KuroEngine.h"
#include"Camera.h"

std::shared_ptr<GraphicsPipeline>EnemyDeadEmitter::s_linePtGraphicsPipeline;
std::shared_ptr<GraphicsPipeline>EnemyDeadEmitter::s_squarePtGraphicsPipeline;
std::shared_ptr<TextureBuffer>EnemyDeadEmitter::s_ptTex;

void EnemyDeadEmitter::GeneratePipeline()
{
	//���ɐ�����
	if (s_linePtGraphicsPipeline)return;

	PipelineInitializeOption option(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	option.m_calling = D3D12_CULL_MODE_NONE;

	std::vector<RenderTargetInfo>renderTarget =
	{
		RenderTargetInfo(D3D12App::Instance()->GetBackBuffFormat(), AlphaBlendMode_Trans),
		RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, AlphaBlendMode_Trans),
	};

	//���C���p�[�e�B�N��
	{
		Shaders shaders;
		shaders.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadLineParticle.hlsl", "VSmain", "vs_6_4");
		shaders.m_gs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadLineParticle.hlsl", "GSmain", "gs_6_4");
		shaders.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadLineParticle.hlsl", "PSmain", "ps_6_4");

		std::vector<RootParam>rootParams =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�J�������"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�G�~�b�^�[���"),
		};

		s_linePtGraphicsPipeline = D3D12App::Instance()->GenerateGraphicsPipeline(
			option,
			shaders,
			EnemyDeadLineParticle::GetInputLayout(),
			rootParams,
			renderTarget,
			{ WrappedSampler(false,true) });
		//�l�p�p�[�e�B�N��
	}
	{
		Shaders shaders;
		shaders.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadSquareParticle.hlsl", "VSmain", "vs_6_4");
		shaders.m_gs = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadSquareParticle.hlsl", "GSmain", "gs_6_4");
		shaders.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/EnemyDeadSquareParticle.hlsl", "PSmain", "ps_6_4");

		std::vector<RootParam>rootParams =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�J�������"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�G�~�b�^�[���"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�p�[�e�B�N���p�e�N�X�`��"),
		};

		s_squarePtGraphicsPipeline = D3D12App::Instance()->GenerateGraphicsPipeline(
			option,
			shaders,
			EnemyDeadSquareParticle::GetInputLayout(),
			rootParams,
			renderTarget,
			{ WrappedSampler(false,false) });
	}

	//�l�p�p�[�e�B�N���p�e�N�X�`���ǂݍ���
	s_ptTex = D3D12App::Instance()->GenerateTextureBuffer("resource/user/enemyDeadSquareParticle.png");
}

EnemyDeadEmitter::EnemyDeadEmitter()
{
	GeneratePipeline();

	//���C���p�[�e�B�N���o�b�t�@
	m_linePtBuff = D3D12App::Instance()->GenerateVertexBuffer(
		sizeof(EnemyDeadLineParticle),
		PARTICLE_NUM,
		nullptr,
		"EnemyDeadLineParticle");

	//�l�p�p�[�e�B�N���o�b�t�@
	m_squarePtBuff = D3D12App::Instance()->GenerateVertexBuffer(
		sizeof(EnemyDeadSquareParticle),
		PARTICLE_NUM,
		nullptr,
		"EnemyDeadSquareParticle");

	//�G�~�b�^�[�o�b�t�@
	m_emiBuff = D3D12App::Instance()->GenerateConstantBuffer(
		sizeof(EmitterInfo),
		1,
		&m_emitterInfo,
		"EnemyDeadEmitter");
}

void EnemyDeadEmitter::Init(const Vec3<float> &POS)
{
	Color color(KuroFunc::GetRand(0, 255), KuroFunc::GetRand(0, 255), KuroFunc::GetRand(0, 255), 255);

	//�l�������čŒ�����o��
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
		m_SquareParticle[i].Init(POS, KuroFunc::GetRand(0.4f, 1.0f), random, color);
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
	//�p�[�e�B�N������GPU�o�b�t�@�ɓ]��
	m_linePtBuff->Mapping(m_lineParticle.data());
	m_squarePtBuff->Mapping(m_SquareParticle.data());

	//�����_�[�^�[�Q�b�g�Z�b�g
	KuroEngine::Instance()->Graphics().SetRenderTargets({ MAIN.lock(),EMMISIVE_MAP.lock() }, DEPTH_STENCIL.lock());

	//���C���p�[�e�B�N���`��
	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_linePtGraphicsPipeline);
	KuroEngine::Instance()->Graphics().ObjectRender(
		m_linePtBuff,
		{
			RegisterDescriptorData(CAMERA.GetBuff(),CBV),
			RegisterDescriptorData(m_emiBuff,CBV),
		},
		pos.z,
		true);

	//�l�p�p�[�e�B�N���`��
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
