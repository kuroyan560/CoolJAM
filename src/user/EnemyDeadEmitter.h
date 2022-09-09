#pragma once
#include"../user/EnemyDeadLineParticle.h"
#include"../user/EnemyDeadSquareParticle.h"
#include<array>
#include"../engine/Common/Vec.h"
#include<memory>

class Camera;
class GraphicsPipeline;
class VertexBuffer;
class ConstantBuffer;
class RenderTarget;
class DepthStencil;
class TextureBuffer;
class EnemyDeadEmitter
{
private:
	static std::shared_ptr<GraphicsPipeline>s_linePtGraphicsPipeline;
	static std::shared_ptr<GraphicsPipeline>s_squarePtGraphicsPipeline;
	static std::shared_ptr<TextureBuffer>s_ptTex;
	static void GeneratePipeline();

public:
	EnemyDeadEmitter();
	void Init(const Vec3<float> &POS);
	void Update();
	void Draw(Camera& CAMERA, 
		std::weak_ptr<RenderTarget>MAIN, 
		std::weak_ptr<RenderTarget>EMMISIVE_MAP,
		std::weak_ptr<DepthStencil>DEPTH_STENCIL);

private:
	struct EmitterInfo
	{
		//ラインパーティクルの太さ
		float m_linePtThickness = 0.3f;
		//テクスチャパーティクルサイズ
		float m_squarePtSize = 2.0f;
		//エミッシブ強さ
		float m_emissiveStrongth = 1.0f;
	}m_emitterInfo;

	Vec3<float>pos;

	static const int PARTICLE_NUM = 20;

	//頂点バッファ（ラインパーティクルのバッファ）
	std::shared_ptr<VertexBuffer>m_linePtBuff;
	std::array<EnemyDeadLineParticle, PARTICLE_NUM>m_lineParticle;

	//頂点バッファ（四角パーティクルのバッファ）
	std::shared_ptr<VertexBuffer>m_squarePtBuff;
	std::array<EnemyDeadSquareParticle, PARTICLE_NUM>m_SquareParticle;

	//定数バッファ（エミッターのバッファ）
	std::shared_ptr<ConstantBuffer>m_emiBuff;

};

