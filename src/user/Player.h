#pragma once
#include"Transform.h"
#include<memory>
class Model;
class Camera;

class Player
{
	//�g�����X�t�H�[��
	Transform m_transform;

	//���f��
	std::shared_ptr<Model>m_model;


public:
	Player();
	void Init();
	void Update();
	void Draw(Camera& Cam);
	void Finalize();

	//�g�����X�t�H�[���Q�b�^
	const Transform& GetTransform()const { return m_transform; }
};

