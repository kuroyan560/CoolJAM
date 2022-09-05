#pragma once
#include<memory>
#include<wrl.h>
#include"D3D12Data.h"

class GaussianBlur
{
private:
	static const int s_weightNum = 8;
	static std::shared_ptr<ComputePipeline>s_xBlurPipeline;	//���u���[
	static std::shared_ptr<ComputePipeline>s_yBlurPipeline;	//�c�u���[
	static std::shared_ptr<ComputePipeline>s_finalPipeline;		//�e�u���[�e�N�X�`������
	static void GeneratePipeline();

private:
	//�d�݃e�[�u��
	float m_weights[s_weightNum];

	//�d�݃e�[�u���̒萔�o�b�t�@
	std::shared_ptr<ConstantBuffer>m_weightConstBuff;
	//�e�N�X�`�����̒萔�o�b�t�@
	std::shared_ptr<ConstantBuffer>m_texInfoConstBuff;

	//���u���[�̌���
	std::shared_ptr<TextureBuffer>m_xBlurResult;

	//�c�u���[�̌���
	std::shared_ptr<TextureBuffer>m_yBlurResult;

	//�ŏI����
	std::shared_ptr<TextureBuffer>m_finalResult;

public:
	//�\�[�X�摜 & ���ʕ`���ݒ�A�ڂ�������
	GaussianBlur(const Vec2<int>& Size, const DXGI_FORMAT& Format, const float& BlurPower = 8.0f);
	//�{�P�
	void SetBlurPower(const float& BlurPower);
	//�������s
	void Execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& CmdList, const std::shared_ptr<TextureBuffer>& SourceTex);
	//�O���t�B�b�N�X�}�l�[�W���ɓo�^
	void Register(const std::shared_ptr<TextureBuffer>& SourceTex);

	//���ʂ̃e�N�X�`���擾
	std::shared_ptr<TextureBuffer>& GetResultTex() { return m_finalResult; }

	//�E�B���h�E�T�C�Y�Ō��ʂ�`��
	void DrawResult(const AlphaBlendMode& AlphaBlend);
};