#pragma once
#include"Transform.h"
#include"Vec.h"
#include<memory>
#include<DirectXMath.h>
#include"ImguiDebugInterFace.h"

class Model;
class Camera;
class BulletMgr;
class EnemyMgr;

class Player : public ImguiDebugInterface
{

private:

	/*===== �����o�ϐ� =====*/

	//�g�����X�t�H�[��
	Transform m_transform;
	const float SCALE = 1.0f;
	bool m_isEdge;				// ���ɂ��邩

	// �e�֌W
	int m_shotTimer;
	const float SHOT_TIMER = 15;

	//���f��
	std::shared_ptr<Model>m_model;

/*--- �͂� ---*/
	Vec3<float>m_initMove;
	Vec3<float>m_move;

public:

	bool m_isDebugParam = false;
	float MIN_SPEED = 0.4f;	// �ŏ��̈ړ����x
	float MAX_SPEED = 2.5f;	// �ő�̈ړ����x
	float BRAKE_SPEED = 0.2f;

	/*===== �����o�֐� =====*/

	Player();
	void Init();
	void Finalize();
	void Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope);
	void Draw(Camera& Cam);
	void DrawDebugInfo(Camera& Cam);

	Vec3<float> GetPos() { return m_transform.GetPos(); }


private:

	// ���͏���
	void Input(Camera& Cam, const Vec2<float>& WindowSize);

	// �ړ�����
	void Move();

	// �e�����B
	void Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr);

	// �����蔻��
	void CheckHit(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const float& EdgeScope);

	// 01�ɔ[�߂�B
	float Saturate(const float& Value);

	// �g�����X�t�H�[���Q�b�^
	const Transform& GetTransform()const { return m_transform; }

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
	bool CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos);

	// �����Ɛ����̓����蔻��
	bool IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2);

	// �����̌�_�����߂�B
	Vec2<float> CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2);

	// �x�N�g���ɍs����|����B
	inline Vec3<float> MulMat(Vec3<float> Vec, const DirectX::XMMATRIX& Mat) {
		DirectX::XMVECTOR vector = DirectX::XMVector3Transform({ Vec.x, Vec.y, Vec.z }, Mat);
		return Vec3<float>(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]);
	}

	void OnImguiDebug()override;
};

