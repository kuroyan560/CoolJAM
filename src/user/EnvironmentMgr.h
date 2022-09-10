#pragma once
#include"ImguiDebugInterFace.h"
#include<memory>
#include<string>
#include<array>
#include"Transform.h"
#include"Light.h"
class Model;
class ModelObject;
class Camera;
class LightManager;

class EnvironmentMgr : public ImguiDebugInterface
{
public:
	enum STATUS { DEFAULT, FEVER, NUM, NONE = -1 };

private:
	//�X�J�C�h�[�����f��
	std::array<std::shared_ptr<Model>, STATUS::NUM>m_skyDomeModelArray;

	//�����f��
	std::array<std::shared_ptr<Model>, STATUS::NUM>m_pillarModelArray;
	//���̐�
	static const int PILLAR_NUM = 12;
	float m_pillarPosRadius = 160.0f;
	float m_pillarPosY = -50.0f;

	//���݂̃X�e�[�^�X
	int m_nowStatus = STATUS::DEFAULT;

	//���ɃZ�b�g�����X�e�[�^�X
	int m_nextStatus = STATUS::NONE;

	//�X�e�[�^�X�̐؂�ւ��i�s���[�g
	float m_statusChangeRate = 1.0f;

	//�X�e�[�^�X�؂�ւ�莞��
	int m_statusChangeTime = 250;

	//���C�g�}�l�[�W��
	std::shared_ptr<LightManager>m_ligMgr;

	//�f�B���N�V�������C�g
	Light::Direction m_dirLigDef;

public:
	EnvironmentMgr();
	~EnvironmentMgr() {}

	void Init();
	void Update();
	void Draw(Camera& Cam);

	//���X�e�[�^�X�ύX
	void ChangeStatus(STATUS NextStatus)
	{
		m_nextStatus = NextStatus;
		m_statusChangeRate = 0.0f;
	}

	//�Q�b�^
	std::shared_ptr<LightManager>& GetLigMgr() { return m_ligMgr; }

	void OnImguiDebug()override;
};

static std::array<std::string, EnvironmentMgr::STATUS::NUM> s_skyDomeNameArray =
{
	"DEFAULT","FEVER"
};