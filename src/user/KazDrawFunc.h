#pragma once
#include"Color.h"
#include"D3D12Data.h"
#include<vector>
#include"Transform.h"
#include<memory>
#include"Camera.h"
#include"Object.h"
#include"KuroEngine.h"
#include"Model.h"
#include"LightManager.h"
#include"CubeMap.h"
#include"ModelAnimator.h"
#include<map>

namespace KazDrawFunc
{
	static int s_drawNonShadingCount;

	static void CountReset()
	{
		s_drawNonShadingCount = 0;
	};

	static void DrawNonShadingModel(const std::weak_ptr<Model> Model, Transform &Transform, Color &color,Camera &Cam, std::shared_ptr<ModelAnimator> Animator = nullptr, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans)
	{
		return;
		static std::map<DXGI_FORMAT, std::array<std::shared_ptr<GraphicsPipeline>, AlphaBlendModeNum>>PIPELINE;
		static std::vector<std::shared_ptr<ConstantBuffer>>TRANSFORM_BUFF;
		static std::vector<std::shared_ptr<ConstantBuffer>>COLOR_BUFF;

		const auto targetFormat = KuroEngine::Instance()->Graphics().GetRecentRenderTargetFormat(0);

		//パイプライン未生成
		if (!PIPELINE[targetFormat][BlendMode])
		{
			//パイプライン設定
			static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//シェーダー情報
			static Shaders SHADERS;
			SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/engine/DrawNonShadingModel.hlsl", "VSmain", "vs_6_4");
			SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/users/shaders/DrawNonShadingModel.hlsl", "PSmain", "ps_6_4");

			//ルートパラメータ
			static std::vector<RootParam>ROOT_PARAMETER =
			{
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カメラ情報バッファ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"トランスフォームバッファ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"ボーン行列バッファ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"カラーテクスチャ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"マテリアル基本情報バッファ"),
				RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"色"),
			};

			//レンダーターゲット描画先情報;
			std::vector<RenderTargetInfo>RENDER_TARGET_INFO = { RenderTargetInfo(targetFormat, BlendMode) };
			//パイプライン生成
			PIPELINE[targetFormat][BlendMode] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, ModelMesh::Vertex::GetInputLayout(), ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, false) });
		}

		KuroEngine::Instance()->Graphics().SetGraphicsPipeline(PIPELINE[targetFormat][BlendMode]);

		if (TRANSFORM_BUFF.size() < (s_drawNonShadingCount + 1))
		{
			TRANSFORM_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(Matrix), 1, nullptr, ("DrawNonShadingModel_Transform -" + std::to_string(s_drawNonShadingCount)).c_str()));
		}
		if (COLOR_BUFF.size() < (s_drawNonShadingCount + 1))
		{
			COLOR_BUFF.emplace_back(D3D12App::Instance()->GenerateConstantBuffer(sizeof(DirectX::XMFLOAT4), 1, nullptr, ("Color" + std::to_string(s_drawNonShadingCount)).c_str()));
		}

		TRANSFORM_BUFF[s_drawNonShadingCount]->Mapping(&Transform.GetMat());
		COLOR_BUFF[s_drawNonShadingCount]->Mapping(&color);

		auto model = Model.lock();
		std::shared_ptr<ConstantBuffer>boneBuff;
		if (Animator)boneBuff = Animator->GetBoneMatBuff();

		for (int meshIdx = 0; meshIdx < model->m_meshes.size(); ++meshIdx)
		{
			const auto &mesh = model->m_meshes[meshIdx];
			KuroEngine::Instance()->Graphics().ObjectRender(
				mesh.mesh->vertBuff,
				mesh.mesh->idxBuff,
				{
					{Cam.GetBuff(),CBV},
					{TRANSFORM_BUFF[s_drawNonShadingCount],CBV},
					{boneBuff,CBV},
					{mesh.material->texBuff[COLOR_TEX],SRV},
					{mesh.material->buff,CBV},
					{COLOR_BUFF[s_drawNonShadingCount],CBV}
				},
				Transform.GetPos().z,
				true);
		}

		s_drawNonShadingCount++;
	}

}