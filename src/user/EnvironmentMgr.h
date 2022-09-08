#pragma once
#include"ImguiDebugInterFace.h"
#include<memory>
#include<string>
#include<array>
#include"Transform.h"
class Model;
class ModelObject;
class Camera;

class EnvironmentMgr : public ImguiDebugInterface
{
public:
	enum STATUS { DEFAULT, FEVER, NUM, NONE = -1 };

private:
	//�X�J�C�h�[�����f��
	std::shared_ptr<Model>m_skyDomeModelArray[STATUS::NUM];

	//�����f��
	std::shared_ptr<Model>m_pillarModelArray[STATUS::NUM];
	//���̐�
	static const int PILLAR_NUM = 12;
	float m_pillarPosRadius = 160.0f;

	//�����f��
	std::shared_ptr<ModelObject>m_floorModelObj;

	//���݂̃X�e�[�^�X
	int m_nowStatus = STATUS::DEFAULT;

	//���ɃZ�b�g�����X�e�[�^�X
	int m_nextStatus = STATUS::NONE;

	//�X�e�[�^�X�̐؂�ւ��i�s���[�g
	float m_statusChangeRate = 1.0f;

	//�X�e�[�^�X�؂�ւ�莞��
	int m_statusChangeTime = 250;

public:
	EnvironmentMgr();
	~EnvironmentMgr() {}

	void Init();
	void Update();
	void Draw(Camera& Cam);

	void ChangeStatus(STATUS NextStatus)
	{
		m_nextStatus = NextStatus;
		m_statusChangeRate = 0.0f;
	}
	void OnImguiDebug()override;
};

static std::array<std::string, EnvironmentMgr::STATUS::NUM> s_skyDomeNameArray =
{
	"DEFAULT","FEVER"
};