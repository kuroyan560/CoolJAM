#include <DrawFunc_Append.h>
#include "EnemyHP.h"
#include "../engine/Importer.h"

EnemyHP::EnemyHP(const float& Angle) {

	/*===== �R���X�g���N�^ =====*/

	m_model = Importer::Instance()->LoadModel("resource/user/", "enemy_hp.glb");
	m_alpha = 0.0f;
	m_isActive = false;
	m_drawAngle = 0.0f;
	m_drawAngle = Angle;

}

void EnemyHP::Init() {

	/*===== ���������� =====*/

	m_alpha = 0.0f;
	m_isActive = false;
	m_drawAngle = 0.0f;

}

void EnemyHP::Update(const Vec3<float>& EnemyPos, const float& EnemySize) {

	/*===== �X�V���� =======*/

	// ���W��ݒ�B
	m_transform.SetPos(EnemyPos);

	// ���S���炸�炷�ʁB
	float moveScale = EnemySize + (SIZE * 2.5f);

	// ���炷�p�x�B
	Vec3<float> moveDir = Vec3<float>(cosf(m_drawAngle), 0.0f, sinf(m_drawAngle));

	// ���W��`��p�x�����炷�B
	m_transform.SetPos(m_transform.GetPos() + moveDir * moveScale);

	// �����t���O�������Ă�����B
	if (m_isActive) {

		m_alpha += (1.0f - m_alpha) / 10.0f;

	}
	else {

		// �A���t�@��0���傫��������B
		if (0 < m_alpha) {

			m_alpha += (0.0f - m_alpha) / 10.0f;

		}

	}

}

void EnemyHP::Draw() {

	/*===== �`�揈�� =====*/

	// �A���t�@�l��0.05f�ȉ��������珈�����s��Ȃ��B
	if (m_alpha <= 0.05f) return;

	m_transform.SetPos(m_transform.GetPos() + Vec3<float>(0.0f, 0.1f, 0.0f));
	m_transform.SetRotate(DirectX::XMMatrixRotationY(-m_drawAngle));
	m_transform.SetScale(SIZE);

	RenderTargetSwitch renderTargetSwitch = RenderTargetSwitch(m_alpha, 1.0f, 1.0f);
	DrawFunc_Append::DrawModel(m_model, m_transform, renderTargetSwitch, false, true, nullptr, AlphaBlendMode_Trans);

}