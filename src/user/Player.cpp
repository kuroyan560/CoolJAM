#include "Player.h"
#include "../engine/UsersInput.h"
#include"Importer.h"

Player::Player()
{

	/*===== コンストラクタ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_speed = 0;
	m_brakeTimer = 0;
	m_brakeBoostSpeed = 0;
	m_isEdge = false;
	m_isBrake = false;

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");
}

void Player::Init()
{

	/*===== 初期化処理 =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = Vec3<float>();
	m_speed = 0;
	m_brakeTimer = 0;
	m_brakeBoostSpeed = 0;
	m_isEdge = false;
	m_isBrake = false;

}

void Player::Update()
{

	/*===== 更新処理 =====*/

	// 入力処理
	Input();

	// 移動処理
	Move();

	// 当たり判定処理
	CheckHit();

}

void Player::Draw()
{
}

void Player::Input()
{

	/*====== 入力処理 =====*/

	// スティックの入力を取得。
	Vec2<float> stickInput = UsersInput::Instance()->GetLeftStickVecRawFuna(0);

	// デッドラインを設ける。
	const float INPUT_DEADLINE = 0.25f;
	// 入力されていたら。
	if (INPUT_DEADLINE < stickInput.Length()) {

		// 入力を保存。
		m_inputVec = Vec3<float>(stickInput.x, 0.0f, stickInput.y);

	}

	// ブレーキ入力を保存。
	m_isBrake = UsersInput::Instance()->ControllerInput(0, A);
	if (m_isBrake) {

		++m_brakeTimer;

	}
	else {

		// ブレーキタイマーが1以上だったらブレーキを離した瞬間ということ。
		if (0 < m_brakeTimer) {

			// 経過時間から割合を求める。
			float brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER));

			// ブースト量を更新。
			float boostAmount = MAX_BRAKE_SPEED * brakeRate;
			if (m_brakeBoostSpeed < boostAmount) {

				m_brakeBoostSpeed = boostAmount;

			}

		}

		m_brakeTimer = 0;

	}

	// デバッグ機能
	if (UsersInput::Instance()->ControllerInput(0, B)) {

		Init();

	}

}

void Player::Move()
{

	/*===== 移動処理 =====*/

	// 移動方向ベクトルを入力方向ベクトルに向かって補完する。
	float cross = m_forwardVec.Cross(m_inputVec).y;
	if (cross != 0) {

		cross = cross < 0 ? -1.0f : 1.0f;
		float handleRot = (m_isBrake ? BRAKE_HANDLE_ROT : HANDLE_ROT) * cross;

		// 移動方向ベクトルを角度に直して値を加算する。
		float forwardVecAngle = atan2f(m_forwardVec.z, m_forwardVec.x);
		forwardVecAngle += handleRot;

		// 加算した角度をベクトルに直す。
		m_forwardVec = Vec3<float>(cosf(forwardVecAngle), 0.0f, sinf(forwardVecAngle));

}

	// ブレーキ状態の有無に応じて移動速度を変える。
	const float BASE_SPEED = m_isBrake ? BRAKE_SPEED : DEF_SPEED;

	// 移動速度を補完する。
	m_speed += (BASE_SPEED - m_speed) / 20.0f;

	// ブーストを減衰させる。
	m_brakeBoostSpeed -= SUB_BRAKE_SPEED;
	if (m_brakeBoostSpeed < 0.0f) {

		m_brakeBoostSpeed = 0.0f;

	}

	// 移動させる。
	m_pos += m_forwardVec * m_speed;

	// ブースト分移動させる。
	m_pos += m_forwardVec * m_brakeBoostSpeed;

}

void Player::CheckHit()
#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam)
{

	/*===== 当たり判定 =====*/



	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);
}

float Player::Saturate(const float& Value)
void Player::Finalize()
{

	/*===== 01に納める。 =====*/

	float value = Value;
	if (value < 0.0f) value = 0.0f;
	if (1.0f < value) value = 1.0f;

	return value;

}
