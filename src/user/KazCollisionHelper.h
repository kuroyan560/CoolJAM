#pragma once
#include"Transform.h"
#include "Vec.h"
#include<memory>
#include<DirectXMath.h>

class Model;
class Camera;

namespace KazCollisionHelper
{
	// �}�b�v���Ɏ��܂�悤�ɂ���B
	Vec3<float> KeepInMap(Vec3<float>& Pos, const float& MapSize);

	/// <summary>
	/// ���C�ƃ��f���̐����ł̓����蔻��
	/// </summary>
	/// <param name="Model"> �����蔻����s�����f�� </param>
	/// <param name="ModelTransform"> �����蔻����s�����f����Transform </param>
	/// <param name="RayDir"> ���C���΂����� </param>
	/// <param name="LineLength"> �L�΂����C�̒��� </param>
	/// <param name="ShortesetPos"> �ŒZ�̏Փ˓_ </param>
	/// <returns> �Փ˔���̗L�� </returns>
	bool CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& Pos,const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos);

	// �����Ɛ����̓����蔻��
	bool IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2);

	// �����̌�_�����߂�B
	Vec2<float> CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2);

	// �x�N�g���ɍs����|����B
	inline Vec3<float> MulMat(Vec3<float> Vec, const DirectX::XMMATRIX& Mat) {
		DirectX::XMVECTOR vector = DirectX::XMVector3Transform({ Vec.x, Vec.y, Vec.z }, Mat);
		return Vec3<float>(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]);
	}

};

