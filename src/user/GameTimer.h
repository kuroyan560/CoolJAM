#pragma once
#include"../Common/Vec.h"
#include<vector>
#include"../Common/Singleton.h"
#include<array>
#include"../engine/D3D12App.h"
#include"Font.h"

//InToTheAbyss���珈�������p

/// <summary>
/// �Q�[���̑S�̂̎��Ԃ��v�Z���܂�
/// </summary>
class GameTimer
{
	bool interruput;
public:
	GameTimer();

	/// <summary>
	/// ���Ԃ̕`����W�����������܂�
	/// </summary>
	/// <param name="TIME">�t���[����</param>
	void Init(int TIME);
	void Finalize();
	void Update();
	void Draw();

	/// <summary>
	/// ���Ԃ��v��܂�
	/// </summary>
	void Start();

	/// <summary>
	/// �t���[���P�ʂŎ��Ԃ�n���܂�
	/// </summary>
	/// <returns>�t���[����</returns>
	int GetFlame();

	/// <summary>
	/// �b�P�ʂŎ��Ԃ�n���܂�
	/// </summary>
	/// <returns>�b��</returns>
	int GetSeceond();

	/// <summary>
	/// ���Ԑ؂��m�点�܂�
	/// </summary>
	/// <returns>true...���Ԑ؂�,false...���Ԑ؂ꂶ��Ȃ�</returns>
	bool TimeUpFlag();

	/// <summary>
	/// �J�E���g�_�E�����I�����������n���܂�
	/// </summary>
	/// <returns>true...�J�E���g�_�E���I��,false...�I�����Ă��Ȃ�</returns>
	bool StartGame();

	/// <summary>
	/// ���Ԍv���̈ꎞ���f�t���O�Z�b�g
	/// </summary>
	void SetInterruput(const bool &Flg) { interruput = Flg; }


	bool FinishAllEffect();

	bool IsStart();
	void Debug();

	Vec2<float> timerPos;
private:

	std::vector<int> number;

	int timer;
	float flame;
	bool startFlag;
	bool timeUpFlag;
	bool finishAllEffectFlag;
	int timeUpInterval;

	std::vector<int> minitueHandle;
	std::vector<int> timeHandle;
	std::vector<int> miriHandle;

	float countDownEndPos;
	Vec2<int> texSize;
	int countDownNum;

	bool countDownFlag;
	int countDownHandle[4], finishHandle;

	int finishTimer;

	bool counDownFinishFlag;

	std::array<int, 5> countDownSE;

	std::vector<int> CountNumber(int TIME);

	// UI�̃T�C�Y�̃I�t�Z�b�g
	const float OFFSET_SIZE = 0.98;
	float timerSize;
	int timerAlpha;

	bool isLessThan5SecondsLeft;	// �c��ܕb����t���O

	// �c��ܕb�ȉ��̂Ƃ��ɐ^�񒆂ɏo���J�E���g�_�E���p�̕ϐ�
	float centerCountDownSize;
	int centerCoundDownAlpha;


	std::array<std::shared_ptr<TextureBuffer>, 12>textureBufferArray;

	float startRate;
	float endRate;
	float startEasePosX;
	float endEasePosX;
};