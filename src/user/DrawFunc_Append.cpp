#include"DrawFunc_Append.h"
#include"KuroEngine.h"
#include"Model.h"
#include"ModelAnimator.h"
#include"LightManager.h"

int DrawFunc_Append::s_drawModelCount = 0;
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
	s_drawModelCount = 0;
	s_drawLineCount = 0;

	KuroEngine::Instance()->Graphics().ClearRenderTarget(Main);
	KuroEngine::Instance()->Graphics().ClearRenderTarget(EmissiveMap);
	KuroEngine::Instance()->Graphics().ClearRenderTarget(DepthMap);
	KuroEngine::Instance()->Graphics().ClearDepthStencil(DepthStencil);

	KuroEngine::Instance()->Graphics().SetRenderTargets(
		{ Main,EmissiveMap,DepthMap }, DepthStencil);

	s_nowCam = NowCamera;
	s_nowLigMgr = NowLigMgr;
}

void DrawFunc_Append::DrawLine(const Vec3<float>& From, const Vec3<float>& To, const Color& LineColor, const float& Thickness, const RenderTargetSwitch& Switch, const AlphaBlendMode& BlendMode)
{
	//if (!LineColor.m_a)return;

	//static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>PIPELINE;
	//static std::vector<std::shared_ptr<VertexBuffer>>LINE_VERTEX_BUFF;

	//const auto targetFormat = KuroEngine::Instance()->Graphics().GetRecentRenderTargetFormat(0);

	////DrawLine��p���_
	//class LineVertex
	//{
	//public:
	//	Vec3<float>m_fromPos;
	//	Vec3<float>m_toPos;
	//	Color m_color;
	//	float m_thickness;
	//	RenderTargetSwitch m_drawSwitch;
	//	LineVertex(const Vec3<float>& FromPos, const Vec3<float>& ToPos, const Color& Color,
	//		const float& Thickness, const RenderTargetSwitch& TargetSwitch)
	//		:m_fromPos(FromPos), m_toPos(ToPos), m_color(Color), m_thickness(Thickness), m_drawSwitch(TargetSwitch) {}
	//};

	////�p�C�v���C��������
	//if (!PIPELINE[targetFormat][BlendMode])
	//{
	//	//�p�C�v���C���ݒ�
	//	static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//	PIPELINE_OPTION.m_calling = false;

	//	//�V�F�[�_�[���
	//	static Shaders SHADERS;
	//	SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/users/DrawLine_Append.hlsl", "VSmain", "vs_6_4");
	//	SHADERS.m_gs = D3D12App::Instance()->CompileShader("resource/users/DrawLine_Append.hlsl", "GSmain", "gs_6_4");
	//	SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/users/DrawLine_Append.hlsl", "PSmain", "ps_6_4");

	//	//�C���v�b�g���C�A�E�g
	//	static std::vector<InputLayoutParam>INPUT_LAYOUT =
	//	{
	//		InputLayoutParam("FROM_POS",DXGI_FORMAT_R32G32B32_FLOAT),
	//		InputLayoutParam("TO_POS",DXGI_FORMAT_R32G32B32_FLOAT),
	//		InputLayoutParam("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT),
	//		InputLayoutParam("THICKNESS",DXGI_FORMAT_R32_FLOAT),
	//	};

	//	//���[�g�p�����[�^
	//	static std::vector<RootParam>ROOT_PARAMETER =
	//	{
	//		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�J�������o�b�t�@"),
	//	};

	//	//�����_�[�^�[�Q�b�g�`�����
	//	std::vector<RenderTargetInfo>RENDER_TARGET_INFO = { RenderTargetInfo(targetFormat, BlendMode) };
	//	//�p�C�v���C������
	//	PIPELINE[targetFormat][BlendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, false) });
	//}

	//KuroEngine::Instance()->Graphics().SetGraphicsPipeline(PIPELINE[targetFormat][BlendMode]);

	//if (LINE_VERTEX_BUFF.size() < (s_drawLineCount + 1))
	//{
	//	LINE_VERTEX_BUFF.emplace_back(D3D12App::Instance()->GenerateVertexBuffer(sizeof(LineVertex), 1, nullptr, ("DrawLine3D -" + std::to_string(s_drawLineCount)).c_str()));
	//}

	//LineVertex vertex(From, To, LineColor, Thickness);
	//LINE_VERTEX_BUFF[s_drawLineCount]->Mapping(&vertex);
	//Vec3<float>center = From.GetCenter(To);

	//KuroEngine::Instance()->Graphics().ObjectRender(
	//	LINE_VERTEX_BUFF[s_drawLineCount],
	//	{
	//		{Cam.GetBuff(),CBV}
	//	},
	//	center.z,
	//	true);

	//s_drawLineCount++;
}

void DrawFunc_Append::DrawModel(const std::weak_ptr<Model>Model, Transform& Transform,
	const RenderTargetSwitch& Switch,
	std::shared_ptr<ModelAnimator> Animator, 
	AlphaBlendMode BlendMode)
{
	static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>PIPELINE;
	static std::vector<std::shared_ptr<ConstantBuffer>>DRWA_DATA_BUFF;

	struct DrawData
	{
		Matrix m_transformMat;
		RenderTargetSwitch m_drawSwitch;
	};

	const auto targetFormat = KuroEngine::Instance()->Graphics().GetRecentRenderTargetFormat(0);

	//�p�C�v���C��������
	if (!PIPELINE[targetFormat][BlendMode])
	{
		//�p�C�v���C���ݒ�
		static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�V�F�[�_�[���
		static Shaders SHADERS;
		SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/user/DrawModel_Append.hlsl", "VSmain", "vs_6_4");
		SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/user/DrawModel_Append.hlsl", "PSmain", "ps_6_4");

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
			RenderTargetInfo(targetFormat, BlendMode),
			RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, BlendMode),
			RenderTargetInfo(DXGI_FORMAT_R32_FLOAT, AlphaBlendMode_None)
		};
		//�p�C�v���C������
		PIPELINE[targetFormat][BlendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, ModelMesh::Vertex::GetInputLayout(), ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, false) });
	}

	KuroEngine::Instance()->Graphics().SetGraphicsPipeline(PIPELINE[targetFormat][BlendMode]);

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
