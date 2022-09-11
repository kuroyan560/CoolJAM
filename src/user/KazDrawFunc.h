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
#include"DrawFunc_Append.h"
#include<map>

class KazDrawFunc
{
public:
	static std::weak_ptr<Camera>s_nowCam;

	//��p���_
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
	static int s_appendDrawModel;

	static void CountReset();
	static void DrawNonShadingModelSignalColor(const std::weak_ptr<Model> Model, Transform &Transform, Color &color, Camera &Cam, std::shared_ptr<ModelAnimator> Animator = nullptr, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);
	static void DrawBillBoardGraph(Camera &Cam, const Vec3<float> &Pos, const Vec2<float> &Size, std::shared_ptr<TextureBuffer>Tex, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);

	/// <summary>
	/// �QD�摜��]�`��
	/// </summary>
	/// <param name="Center">���S���W</param>
	/// <param name="ExtRate">�g�嗦</param>
	/// <param name="Radian">��]�p�x</param>
	/// <param name="Tex">�e�N�X�`��</param>
	/// <param name="Alpha">�A���t�@</param>
	/// <param name="BlendMode">�u�����h���[�h</param>
	/// <param name="LRTurn">���E���]�t���O</param>
	static void DrawRotaGraph2D(const Vec2<float> &Center, const Vec2<float> &ExtRate, const float &Radian,
		const std::shared_ptr<TextureBuffer> &Tex, const float &Alpha = 1.0f, const Vec2<float> &RotaCenterUV = { 0.5f,0.5f },
		const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans, const Vec2<bool> &Mirror = { false,false });


};