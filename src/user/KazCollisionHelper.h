#pragma once
#include"Transform.h"
#include "Vec.h"
#include<memory>
#include<DirectXMath.h>

class Model;
class Camera;

namespace KazCollisionHelper
{
	// マップ内に収まるようにする。
	Vec3<float> KeepInMap(Vec3<float>& Pos, const float& MapSize);

	/// <summary>
	/// レイとモデルの線分での当たり判定
	/// </summary>
	/// <param name="Model"> 当たり判定を行うモデル </param>
	/// <param name="ModelTransform"> 当たり判定を行うモデルのTransform </param>
	/// <param name="RayDir"> レイを飛ばす方向 </param>
	/// <param name="LineLength"> 伸ばすレイの長さ </param>
	/// <param name="ShortesetPos"> 最短の衝突点 </param>
	/// <returns> 衝突判定の有無 </returns>
	bool CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& Pos,const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos);

	// 線分と線分の当たり判定
	bool IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2);

	// 線分の交点を求める。
	Vec2<float> CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2);

	// ベクトルに行列を掛ける。
	inline Vec3<float> MulMat(Vec3<float> Vec, const DirectX::XMMATRIX& Mat) {
		DirectX::XMVECTOR vector = DirectX::XMVector3Transform({ Vec.x, Vec.y, Vec.z }, Mat);
		return Vec3<float>(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]);
	}

};

