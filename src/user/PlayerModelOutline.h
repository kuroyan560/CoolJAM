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
	/// 呼んでいる間は強化完了演出を行う
	/// </summary>
	void EnoughPowerEffect();

	/// <summary>
	/// 呼んでいる間は強化演出を行う
	/// </summary>
	/// <param name="SPEED">強化中の点滅のスピードを調整する 0~100</param>
	void PowerUpEffect(int SPEED);


private:
	//プレイヤーの座標
	Vec3<float> *m_pos;


	//モデルの状態----------
	float m_scale;
	float m_baseScale;
	DirectX::XMMATRIX *m_rotation;
	std::shared_ptr<Model> m_model;
	Color m_modelColor;
	Transform m_transform;
	//モデルの状態----------


	bool m_powerUpFlag;
	float m_expandMaxScale;

	struct OutlineRateData
	{
		float m_expandVertexRate;
		bool m_reversRateFlag;
		float m_powerNum;
		float m_countReversNum;
		float m_countReversMaxNum;

		void Init()
		{
			m_expandVertexRate = 0.0f;
			m_powerNum = 0.0f;
			m_countReversNum = 0.0f;
			m_countReversMaxNum = 15.0f;
			m_reversRateFlag = false;
		}
	};

	//強化演出中----------
	bool m_enoughPowerFlag;
	OutlineRateData m_enoughPowerRateData;
	OutlineRateData m_powerUpRateData;
	//強化演出演出----------

	Vec2<float>m_waveTextureSize;
	std::shared_ptr<Model> m_waveModel;
	float m_waveEaseRate;
	float m_waveScaleMax;
	Transform m_waveTransform;
	float m_waveAlpha;

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
