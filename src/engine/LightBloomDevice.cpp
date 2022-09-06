#include "LightBloomDevice.h"
#include"KuroEngine.h"
#include"GaussianBlur.h"
#include"DrawFunc2D.h"

std::shared_ptr<ComputePipeline>LightBloomDevice::s_pipeline;

void LightBloomDevice::GeneratePipeline()
{
	//�V�F�[�_�[
	auto cs = D3D12App::Instance()->CompileShader("resource/engine/LightBloom.hlsl", "CSmain", "cs_6_4");

	//���[�g�p�����[�^
	std::vector<RootParam>rootParams =
	{
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"���C�g�u���[���ݒ�"),
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�G�~�b�V�u�}�b�v"),
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,"���H��G�~�b�V�u�}�b�v"),
	};

	//�p�C�v���C������
	s_pipeline = D3D12App::Instance()->GenerateComputePipeline(cs, rootParams, { WrappedSampler(false,false) });
}

LightBloomDevice::LightBloomDevice()
{
	//�p�C�v���C������
	if (!s_pipeline)GeneratePipeline();

	//�E�B���h�E�T�C�Y�擾
	const auto winSize = WinApp::Instance()->GetExpandWinSize().Int();

	//�G�~�b�V�u�}�b�v
	m_processedEmissiveMap = D3D12App::Instance()->GenerateTextureBuffer(winSize, DXGI_FORMAT_R32G32B32A32_FLOAT, "LightBloom - EmissiveMap - Processed");

	//�萔�o�b�t�@����
	m_constBuff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(LightBloomConfig), 1, &m_config, "LgihtBloom - Config - ConstantBuffer");

	//�K�E�V�A���u���[
	m_gaussianBlur = std::make_shared<GaussianBlur>(winSize, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void LightBloomDevice::Draw(std::weak_ptr<RenderTarget>EmissiveMap, std::weak_ptr<RenderTarget>Target)
{
	auto emissiveMap = EmissiveMap.lock();

	//�G�~�b�V�u�}�b�v���������l�Ȃǂɉ����ĉ��H
	KuroEngine::Instance()->Graphics().SetComputePipeline(s_pipeline);
	static const int DIV = 4;
	Vec3<int>threadNum = { emissiveMap->GetGraphSize().x / DIV,emissiveMap->GetGraphSize().y / DIV,1 };

	KuroEngine::Instance()->Graphics().Dispatch(threadNum,
		{
			{m_constBuff,CBV},
			{emissiveMap,SRV,D3D12_RESOURCE_STATE_COMMON},
			{m_processedEmissiveMap,UAV}
		});

	//�G�~�b�V�u�}�b�v�Ƀu���[��������
	m_gaussianBlur->Register(m_processedEmissiveMap);

	KuroEngine::Instance()->Graphics().SetRenderTargets({ Target.lock() });

	//���ʂ�`��
	m_gaussianBlur->DrawResult(AlphaBlendMode_Add);
}

void LightBloomDevice::SetOutputColorRate(const Vec3<float>& RGBrate)
{
	m_config.m_outputColorRate = RGBrate;
	m_constBuff->Mapping(&m_config);
}

void LightBloomDevice::SetBrightThreshold(const float& Threshold)
{
	m_config.m_brightThreshold = Threshold;
	m_constBuff->Mapping(&m_config);
}

void LightBloomDevice::SetBlurPower(const float& BlurPower)
{
	m_gaussianBlur->SetBlurPower(BlurPower);
}
