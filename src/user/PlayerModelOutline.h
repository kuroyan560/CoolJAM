#pragma once
#include"../engine/Common/Vec.h"
#include<memory>
#include "Model.h"
#include "Transform.h"

class PlayerModelOutline
{
public:
	PlayerModelOutline();
	void Init(Vec3<float> *POS, DirectX::XMMATRIX *ROTATION, float PLAYER_SCALE, float EXPAND_SCALE, std::shared_ptr<Model> MODEL);
	void Update();
	void Draw(Camera &CAMERA);

	/// <summary>
	/// �Ă�ł���Ԃ͋����������o���s��
	/// </summary>
	void EnoughPowerEffect();

	/// <summary>
	/// �Ă�ł���Ԃ͋������o���s��
	/// </summary>
	/// <param name="SPEED">�������̓_�ł̃X�s�[�h�𒲐����� 0~100</param>
	void PowerUpEffect(int SPEED);


private:
	//�v���C���[�̍��W
	Vec3<float> *m_pos;


	//���f���̏��----------
	float m_scale;
	float m_baseScale;
	DirectX::XMMATRIX *m_rotation;
	std::shared_ptr<Model> m_model;
	Transform m_transform;
	//���f���̏��----------


	bool m_powerUpFlag;
	float m_expandMaxScale;

	//�����������o----------
	bool m_enoughPowerFlag;
	float m_expandVertexRate;
	bool m_reversRateFlag, m_prevReversRateFlag;
	float m_powerNum;
	//�����������o----------


	Color m_color;

	void Rate(float *T, float RATE, float MAX_RATE)
	{
		if (*T < MAX_RATE)
		{
			*T += RATE;
		}
		if (MAX_RATE <= *T)
		{
			*T = MAX_RATE;
		}
	}

};
