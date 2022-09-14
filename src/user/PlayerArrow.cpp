#include "PlayerArrow.h"

PlayerArrow::PlayerArrow()
{

	m_texBuffer = D3D12App::Instance()->GenerateTextureBuffer("resource/user/mouseToPlayerArrow.png");

}

#include "DrawFunc2D.h"
#include "DrawFunc_Append.h"
void PlayerArrow::Draw(const Vec3<float>& PlayerPos, const Vec3<float>& MousePos)
{


	// ���������߂�B
	float length = Vec3<float>(PlayerPos - MousePos).Length();

	// �`�悷�鐔�����߂�B
	int drawCount = length / ARROW_SPAN;

	// 3�ȉ���������`�悵�Ȃ��B
	if (drawCount <= 3) return;

	// �}�E�X���W�܂ł̃x�N�g���B
	Vec3<float> mouseDir = Vec3<float>(MousePos - PlayerPos).GetNormal();

	for (int index = 0; index < drawCount; ++index) {

		// 3�ȉ���������`�悵�Ȃ��B
		if (index <= 3) continue;


		// �`��ʒu�����߂�B
		Vec3<float> drawPos = PlayerPos + mouseDir * (ARROW_SPAN * index) + Vec3<float>(0, 1, 0);

		Transform transform;
		transform.SetPos(drawPos);
		transform.SetRotate(DirectX::XMMatrixRotationY(atan2f(mouseDir.x, mouseDir.z)));
		transform.SetScale(5);

		DrawFunc_Append::DrawPlane(transform, m_texBuffer, RenderTargetSwitch(1, 0, 1), AlphaBlendMode_Trans);

	}


}
