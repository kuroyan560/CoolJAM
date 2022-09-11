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

class KazDrawFunc
{
public:
	//êp¸_
	class Vertex
	{
	public:
		Vec3<float>m_pos;
		Vec2<float>m_size;
		Color m_col;
		Vertex(const Vec3<float> &Pos, const Vec2<float> &Size, const Color &Color)
			:m_pos(Pos), m_size(Size), m_col(Color) {}
	};

	static int s_drawNonShadingModelSignalColor;
	static int s_drawBillBoardGraph;
	static int s_drawRotaGraph2D;

	static void CountReset();
	static void DrawNonShadingModelSignalColor(const std::weak_ptr<Model> Model, Transform &Transform, Color &color, Camera &Cam, std::shared_ptr<ModelAnimator> Animator = nullptr, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);
	static void DrawBillBoardGraph(Camera &Cam, const Vec3<float> &Pos, const Vec2<float> &Size, std::shared_ptr<TextureBuffer>Tex, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);

	/// <summary>
	/// QDĉñ]`ĉ
	/// </summary>
	/// <param name="Center">SÀW</param>
	/// <param name="ExtRate">gċĤ</param>
	/// <param name="Radian">ñ]px</param>
	/// <param name="Tex">eNX`</param>
	/// <param name="Alpha">At@</param>
	/// <param name="BlendMode">uh[h</param>
	/// <param name="LRTurn">ĥE½]tO</param>
	static void DrawRotaGraph2D(const Vec2<float> &Center, const Vec2<float> &ExtRate, const float &Radian,
		const std::shared_ptr<TextureBuffer> &Tex, const float &Alpha = 1.0f, const Vec2<float> &RotaCenterUV = { 0.5f,0.5f },
		const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans, const Vec2<bool> &Mirror = { false,false });
};