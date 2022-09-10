#pragma once
#include"Color.h"
#include"D3D12Data.h"
#include<vector>
#include"Transform.h"
#include<memory>
#include"Camera.h"
#include"Object.h"

class Model;
class LightManager;
class ModelAnimator;

//�����_�[�^�[�Q�b�g�ւ̕`��X�C�b�`
struct RenderTargetSwitch
{
	float m_main = 1.0f;
	float m_emissive = 1.0f;
	float m_depth = 1.0f;
	float pad;

	RenderTargetSwitch() {}
	RenderTargetSwitch(const float& Main, const float& Emissive, const float& Depth)
		:m_main(Main), m_emissive(Emissive), m_depth(Depth) {}
};

class DrawFunc_Append
{
	static int s_drawModelCount;
	static int s_drawLineCount;
	static std::weak_ptr<Camera>s_nowCam;
	static std::weak_ptr<LightManager>s_nowLigMgr;

	static std::weak_ptr<RenderTarget>s_main;
	static std::weak_ptr<RenderTarget>s_emissiveMap;
	static std::weak_ptr<RenderTarget>s_depthMap;
	static std::weak_ptr<DepthStencil>s_depthStencil;

	//�o�^���ꂽ�����_�[�^�[�Q�b�g���Z�b�g
	static void SetRegisteredTargets();

public:
	//�e�����_�[�^�[�Q�b�g�A�J�����A���C�g�}�l�[�W���̓o�^
	static void FrameInit(
		std::shared_ptr<RenderTarget>Main,
		std::shared_ptr<RenderTarget>EmissiveMap,
		std::shared_ptr<RenderTarget>DepthMap,
		std::shared_ptr<DepthStencil>DepthStencil,
		std::shared_ptr<Camera>NowCamera,
		std::shared_ptr<LightManager>NowLigMgr
	);

	/// <summary>
	/// ���`��
	/// </summary>
	/// <param name="From">�n�_</param>
	/// <param name="To">�I�_</param>
	/// <param name="LineColor">�F</param>
	/// <param name="Thickness">����</param>
	/// <param name="Switch">�����_�[�^�[�Q�b�g�ւ̕`�惌�[�g�i�A���t�@�j��Depth�� 0 or 1</param>
	/// <param name="BlendMode">�u�����h���[�h</param>
	static void DrawLine(const Vec3<float>& From, const Vec3<float>& To, const Color& LineColor, const float& Thickness,
		const RenderTargetSwitch& Switch = RenderTargetSwitch(), const AlphaBlendMode& BlendMode = AlphaBlendMode_Trans);

	/// <summary>
	/// ���f���`��
	/// </summary>
	/// <param name="Model">���f��</param>
	/// <param name="Transform">���f���̃g�����X�t�H�[��</param>
	/// <param name="Switch">�����_�[�^�[�Q�b�g�ւ̕`�惌�[�g�i�A���t�@�j��Depth�� 0 or 1</param>
	/// <param name="Animator">���f���̃A�j���[�^�[</param>
	/// <param name="BlendMode">�u�����h���[�h</param>
	static void DrawModel(const std::weak_ptr<Model>Model, Transform& Transform, 
		const RenderTargetSwitch& Switch = RenderTargetSwitch(),
		std::shared_ptr<ModelAnimator> Animator = nullptr,
		AlphaBlendMode BlendMode = AlphaBlendMode_None);
	static void DrawModel(const std::weak_ptr<ModelObject>ModelObject,
		const RenderTargetSwitch& Switch = RenderTargetSwitch(), 
		AlphaBlendMode BlendMode = AlphaBlendMode_None)
	{
		auto obj = ModelObject.lock();
		DrawModel(obj->m_model, obj->m_transform, Switch, obj->m_animator, BlendMode);
	}
};