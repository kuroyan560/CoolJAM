#pragma once
#include<memory>
#include<wrl.h>
#include"D3D12Data.h"
#include<array>

class GaussianBlur
{
private:
	static const int THREAD_DIV = 32;
	static enum PROCESS { X_BLUR, Y_BLUR, FINAL, PROCESS_NUM };
	static const int s_weightNum = 8;
	static std::array<std::shared_ptr<ComputePipeline>, PROCESS_NUM>s_csPipeline;
	static void GeneratePipeline();

private:
	//�d�݃e�[�u��
	float m_weights[s_weightNum];

	//�d�݃e�[�u���̒萔�o�b�t�@
	std::shared_ptr<ConstantBuffer>m_weightConstBuff;
	//�e�N�X�`�����̒萔�o�b�t�@
	std::shared_ptr<ConstantBuffer>m_texInfoConstBuff;
	//�u���[�̌���
	std::array<std::shared_ptr<TextureBuffer>, PROCESS_NUM>m_blurResult;

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
	std::shared_ptr<TextureBuffer>& GetResultTex() { return m_blurResult[FINAL]; }

	//�E�B���h�E�T�C�Y�Ō��ʂ�`��
	void DrawResult(const AlphaBlendMode& AlphaBlend);
};