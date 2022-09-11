#include"DrawFunc_Append.h"
#include"KuroEngine.h"
#include"Model.h"
#include"ModelAnimator.h"
#include"LightManager.h"


int DrawFunc_Append::s_drawLineCount = 0;
std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>DrawFunc_Append::s_drawLinePipeline;

int DrawFunc_Append::s_drawModelCount = 0;
std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>DrawFunc_Append::s_drawModelPipeline;

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

	//���C���^�[�Q�b�g�̃t�H�[�}�b�g�擾
	const auto mainFormat = MainFormat;

	//���ɐ����ς�����
	auto searchPipeline = s_drawLinePipeline.find(mainFormat);
	if (searchPipeline != s_drawLinePipeline.end())return;	//������

	//�p�C�v���C��������
	for (int blendModeIdx = 0; blendModeIdx < AlphaBlendModeNum; ++blendModeIdx)
	{
		AlphaBlendMode blendMode = AlphaBlendMode(blendModeIdx);

		//DrawLine�p
		{
			//�p�C�v���C���ݒ�
			static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			PIPELINE_OPTION.m_calling = D3D12_CULL_MODE_NONE;

			//�V�F�[�_�[���
			static Shaders SHADERS;
			SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawLine3D_Append.hlsl", "VSmain", "vs_6_4");
			SHADERS.m_gs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawLine3D_Append.hlsl", "GSmain", "gs_6_4");
			SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawLine3D_Append.hlsl", "PSmain", "ps_6_4");

			//�C���v�b�g���C�A�E�g
			static std::vector<InputLayoutParam>INPUT_LAYOUT =
			{
				InputLayoutParam("FROM_POS",DXGI_FORMAT_R32G32B32_FLOAT),
				InputLayoutParam("TO_POS",DXGI_FORMAT_R32G32B32_FLOAT),
				InputLayoutParam("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT),
				InputLayoutParam("THICKNESS",DXGI_FORMAT_R32_FLOAT),
				InputLayoutParam("MAIN",DXGI_FORMAT_R32_FLOAT),
				InputLayoutParam("EMISSIVE",DXGI_FORMAT_R32_FLOAT),
				InputLayoutParam("DEPTH",DXGI_FORMAT_R32_FLOAT),
			};

			//���[�g�p�����[�^
			static std::vector<RootParam>ROOT_PARAMETER =
			{
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�J�������o�b�t�@"),
			};

			//�����_�[�^�[�Q�b�g�`�����
			std::vector<RenderTargetInfo>RENDER_TARGET_INFO =
			{
				RenderTargetInfo(mainFormat, blendMode),
				RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, blendMode),
				RenderTargetInfo(DXGI_FORMAT_R32_FLOAT, AlphaBlendMode_None)
			};

			//�p�C�v���C������
			s_drawLinePipeline[mainFormat][blendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, true) });
		}

		//DrawModel�p
		{
			//�p�C�v���C���ݒ�
			static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//�V�F�[�_�[���
			static Shaders SHADERS;
			SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawModel_Append.hlsl", "VSmain", "vs_6_4");
			SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/shaders/DrawModel_Append.hlsl", "PSmain", "ps_6_4");

			//���[�g�p�����[�^
			static std::vector<RootParam>ROOT_PARAMETER =
			{
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�J�������o�b�t�@"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, "�A�N�e�B�u���̃��C�g���o�b�t�@"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "�f�B���N�V�������C�g��� (�\�����o�b�t�@)"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "�|�C���g���C�g��� (�\�����o�b�t�@)"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "�X�|�b�g���C�g��� (�\�����o�b�t�@)"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "�V�����C�g��� (�\�����o�b�t�@)"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�`��f�[�^�o�b�t�@"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�{�[���s��o�b�t�@"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�J���[�e�N�X�`��"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�G�~�b�V�u�e�N�X�`��"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�}�e���A����{���o�b�t�@"),
			};

			//�����_�[�^�[�Q�b�g�`�����
			std::vector<RenderTargetInfo>RENDER_TARGET_INFO =
			{
				RenderTargetInfo(mainFormat, blendMode),
				RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, blendMode),
				RenderTargetInfo(DXGI_FORMAT_R32_FLOAT, AlphaBlendMode_None)
			};
			//�p�C�v���C������
			s_drawModelPipeline[mainFormat][blendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, ModelMesh::Vertex::GetInputLayout(), ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, true) });
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

	s_main = Main;

	//�o�^���ꂽ�t�H�[�}�b�g�ƈقȂ�
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

	//DrawLine��p���_
	class LineVertex
	{
	public:
		Vec3<float>m_fromPos;
		Vec3<float>m_toPos;
		Color m_color;
		float m_thickness;
		RenderTargetSwitch m_drawSwitch;
		LineVertex(const Vec3<float>& FromPos, const Vec3<float>& ToPos, const Color& Color,
			const float& Thickness, const RenderTargetSwitch& TargetSwitch)
			:m_fromPos(FromPos), m_toPos(ToPos), m_color(Color), m_thickness(Thickness), m_drawSwitch(TargetSwitch) {}
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
	std::shared_ptr<ModelAnimator> Animator, 
	AlphaBlendMode BlendMode)
{
	static std::vector<std::shared_ptr<ConstantBuffer>>DRWA_DATA_BUFF;

	struct DrawData
	{
		Matrix m_transformMat;
		RenderTargetSwitch m_drawSwitch;
	};

	SetRegisteredTargets();

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(s_drawModelPipeline[s_main.lock()->GetDesc().Format][BlendMode]);

	if (DRWA_DATA_BUFF.size() < (s_drawModelCount + 1))
	{
		DRWA_DATA_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(DrawData), 1, nullptr, ("DrawModel_Append_DrawData -" + std::to_string(s_drawModelCount)).c_str()));
	}

	DrawData drawData;
	drawData.m_transformMat = Transform.GetMat();
	drawData.m_drawSwitch = Switch;
	DRWA_DATA_BUFF[s_drawModelCount]->Mapping(&drawData);

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
