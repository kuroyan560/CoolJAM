#pragma once
#include"Vec.h"
#include<memory>
#include<vector>
class ComputePipeline;
class RenderTarget;
class TextureBuffer;
class GaussianBlur;
class ConstantBuffer;
class ModelObject;

struct LightBloomConfig
{
	//�o�͂���J���[�ɏ�Z
	Vec3<float>m_outputColorRate = { 1,1,1 };
	//���邳�̂������l
	float m_brightThreshold = 0.0f;
};

class LightBloomDevice
{
private:
	static std::shared_ptr<ComputePipeline>s_pipeline;
	void GeneratePipeline();

private:
	//�ݒ�
	LightBloomConfig m_config;

	//�G�~�b�V�u�}�b�v
	std::shared_ptr<TextureBuffer>m_processedEmissiveMap;
	//�ݒ��񑗐M�p
	std::shared_ptr<ConstantBuffer>m_constBuff;
	//�K�E�V�A���u���[
	std::shared_ptr<GaussianBlur>m_gaussianBlur;

public:
	LightBloomDevice();

	//�����_�[�^�[�Q�b�g�ɉ��Z���ĕ`��
	void Draw(std::weak_ptr<RenderTarget>EmissiveMap, std::weak_ptr<RenderTarget>Target);


	//�Z�b�^
	void SetOutputColorRate(const Vec3<float>& RGBrate = { 1,1,1 });
	void SetBrightThreshold(const float& Threshold = 0.0f);
	void SetBlurPower(const float& BlurPower = 8.0f);
};

