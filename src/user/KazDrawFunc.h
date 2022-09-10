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
	static int s_drawNonShadingModelSignalColor;
	static int s_drawBillBoardGraph;

	static void CountReset();
	static void DrawNonShadingModelSignalColor(const std::weak_ptr<Model> Model, Transform &Transform, Color &color, Camera &Cam, std::shared_ptr<ModelAnimator> Animator = nullptr, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);
	static void DrawBillBoardGraph(Camera &Cam, const Vec3<float> &Pos, const Vec2<float> &Size, std::shared_ptr<TextureBuffer>Tex, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);

private:
	//êÍópí∏ì_
	class Vertex
	{
	public:
		Vec3<float>m_pos;
		Vec2<float>m_size;
		Color m_col;
		Vertex(const Vec3<float> &Pos, const Vec2<float> &Size, const Color &Color)
			:m_pos(Pos), m_size(Size), m_col(Color) {}
	};

};