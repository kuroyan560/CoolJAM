#pragma once
#include"Transform.h"
#include<memory>
class Model;
class Camera;

class Player
{
	//トランスフォーム
	Transform m_transform;

	//モデル
	std::shared_ptr<Model>m_model;


public:
	Player();
	void Init();
	void Update();
	void Draw(Camera& Cam);
	void Finalize();

	//トランスフォームゲッタ
	const Transform& GetTransform()const { return m_transform; }
};

