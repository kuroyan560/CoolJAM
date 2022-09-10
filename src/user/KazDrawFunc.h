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
	static int s_drawRotaGraph2D;

	static void CountReset();
	static void DrawNonShadingModelSignalColor(const std::weak_ptr<Model> Model, Transform &Transform, Color &color, Camera &Cam, std::shared_ptr<ModelAnimator> Animator = nullptr, const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans);

	/// <summary>
/// ２D画像回転描画
/// </summary>
/// <param name="Center">中心座標</param>
/// <param name="ExtRate">拡大率</param>
/// <param name="Radian">回転角度</param>
/// <param name="Tex">テクスチャ</param>
/// <param name="Alpha">アルファ</param>
/// <param name="BlendMode">ブレンドモード</param>
/// <param name="LRTurn">左右反転フラグ</param>
	static void DrawRotaGraph2D(const Vec2<float> &Center, const Vec2<float> &ExtRate, const float &Radian,
		const std::shared_ptr<TextureBuffer> &Tex, const float &Alpha = 1.0f, const Vec2<float> &RotaCenterUV = { 0.5f,0.5f },
		const AlphaBlendMode &BlendMode = AlphaBlendMode_Trans, const Vec2<bool> &Mirror = { false,false });

};