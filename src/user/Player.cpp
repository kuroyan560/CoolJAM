#include "Player.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "KuroFunc.h"
#include "GameManager.h"
#include "DriftParticle.h"
#include "KazCollisionHelper.h"
#include "PlayerHP.h"
#include"ModelAnimator.h"
#include "SlowMgr.h"
#include"AudioApp.h"

Player::Player()
{

	/*===== コンストラクタ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_prevForwardVec = DEF_FORWARDVEC;
	m_speed = MIN_SPEED;
	m_brakeTimer = 0;
	m_driftParticleDelay = 0;
	m_rotX = 0.01f;
	m_rotX = 0;
	m_brakeBoostTimer = 0;
	m_shotTimer = 0;
	m_RedTime = 0;
	m_movedLength = 0;
	m_colorEasingTimer = 0;
	m_isRed = true;
	m_isEdge = false;
	m_isChangeRed = false;
	m_isChangeGreen = false;
	m_isBrake = false;
	m_isDamageEffectDrawPlayer = true;
	m_isDamageEffect = false;
	m_damageEffectTimer = 0;
	m_damageEffectCount = 0;
	m_dashCounter = 0;
	m_hp = MAX_HP;
	m_isFever = false;
	m_isPrevFever = false;
	m_feverTime = 0;
	m_mousePointerScale = false;

	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0;

	m_modelObj = std::make_shared<ModelObject>("resource/user/", "player.glb");
	m_mousePointer = std::make_shared<ModelObject>("resource/user/", "mousePoint3D.glb");

	materialColor = m_modelObj->m_model->m_meshes[0].material->constData.pbr.baseColor;

	for (auto& index : m_driftParticle) {

		index = std::make_shared<DriftParticle>();

	}

	float angleInterval = DirectX::XM_PI / 10.0f;
	for (auto& index : m_hpUI) {

		int indexCount = static_cast<int>(&index - &m_hpUI[0]);

		// 回転量
		float drawAngle = 0;

		// インデックスが10以上だったら。
		if (10 < indexCount) {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * GetDigits(indexCount, 0, 0);

		}
		else {

			drawAngle = DirectX::XM_PIDIV2 - angleInterval * static_cast<float>(indexCount);

		}

		index = std::make_shared<PlayerHP>(-drawAngle);

	}

	m_dashLight = std::make_unique<PlayerDashLighting>();

	m_damageSE = AudioApp::Instance()->LoadAudio("resource/user/sound/damage.wav");
}

void Player::Init()
{

	/*===== 初期化処理 =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_prevForwardVec = DEF_FORWARDVEC;
	m_speed = MIN_SPEED;
	m_brakeTimer = 0;
	m_driftParticleDelay = 0;
	m_rotX = 0.01f;
	m_shotTimer = 0;
	m_RedTime = 0;
	m_movedLength = 0;
	m_dashCounter = 0;
	m_colorEasingTimer = 0;
	m_isRed = true;
	m_brakeBoostTimer = 0;
	m_isEdge = false;
	m_isBrake = false;
	m_isChangeRed = false;
	m_isChangeGreen = false;
	m_isDamageEffectDrawPlayer = true;
	m_isDamageEffect = false;
	m_damageEffectTimer = 0;
	m_damageEffectCount = 0;
	m_hp = MAX_HP;
	m_isFever = false;
	m_isPrevFever = false;
	m_feverTime = 0;
	m_mousePointerScale = 0;

	m_knockBackVec = Vec3<float>();
	m_knockBackSpeed = 0;

	for (auto& index : m_driftParticle) {

		index->Init();

	}

	for (auto& index : m_hpUI) {

		index->Init();

	}

	m_outlineModel.Init(&m_pos, &m_rotation, 1.0f, 0.3f, Importer::Instance()->LoadModel("resource/user/", "playerOutline.glb"));

	//ダッシュ時のエフェクト
	m_dashLight->Init(&m_pos);

	m_modelObj->m_animator->Play("ToFloater", false, false);
}

void Player::Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope, bool IsStopFeverTimer, bool IsNoInput)
{

	/*===== 更新処理 =====*/

	// 入力処理
	if (!IsNoInput) {
		Input(Cam, WindowSize);
	}

	// 移動処理
	Move(BulletMgr, IsStopFeverTimer);

	// 弾発射処理
	Shot(BulletMgr, EnemyMgr);

	// 当たり判定処理
	CheckHit(BulletMgr, EnemyMgr, MapSize, EdgeScope);

	// エフェクト全般の更新処理
	UpdateEffect();

	// HPUIの更新処理
	for (auto& index : m_hpUI) {

		index->Invalidate();

	}
	if (10 < m_hp) {

		for (int index = 0; index < GetDigits(m_hp, 0, 0); ++index) {

			m_hpUI[index]->Activate();

		}

	}
	else {

		for (int index = 0; index < m_hp; ++index) {

			m_hpUI[index]->Activate();

		}

	}

	// HPUIの更新処理
	for (auto& index : m_hpUI) {

		index->Update(m_pos, SCALE);

	}

	//アニメーター更新
	m_modelObj->m_animator->Update();
}

void Player::Input(Camera& Cam, const Vec2<float>& WindowSize)
{

	/*====== 入力処理 =====*/

	// マウスの入力からベクトルを求める。
	Vec2<float> screenPos = KuroFunc::ConvertWorldToScreen(m_pos, Cam.GetViewMat(), Cam.GetProjectionMat(), WindowSize);
	Vec2<float> mouseDir = Vec2<float>(UsersInput::Instance()->GetMousePos() - screenPos).GetNormal();
	m_inputVec = Vec3<float>(mouseDir.y, 0.0f, mouseDir.x);

	// 正面ベクトルを保存。
	m_prevForwardVec = m_forwardVec;

	// スクリーン座標系での正面ベクトル。
	float screenForwardAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
	Vec2<float> screenForwardVec = Vec2<float>(cosf(screenForwardAngle), sinf(screenForwardAngle));

	// ブレーキ入力を保存。
	bool oldBrake = m_isBrake;
	m_isBrake = UsersInput::Instance()->MouseInput(LEFT);

	//タイヤアニメーション
	if (!oldBrake && m_isBrake)	//フローター → タイヤ
	{
		m_modelObj->m_animator->Play("ToWheel", false, false);
	}
	else if (oldBrake && !m_isBrake)	//タイヤ → フローター
	{
		m_modelObj->m_animator->Play("ToFloater", false, false);
	}

	if (m_isBrake) {

		++m_brakeTimer;

		// ブレーキブーストのタイマーを初期化する。
		m_brakeBoostTimer = 0;

		// マウスのわーるど座標を求める。
		Vec2<int> winSize = Vec2<int>(WindowSize.x, WindowSize.y);
		Vec3<float> mousePos = KuroFunc::ConvertScreenToWorld(UsersInput::Instance()->GetMousePos(), 0, Cam.GetViewMat(), Cam.GetProjectionMat(), winSize);
		Vec3<float> mousePosFar = KuroFunc::ConvertScreenToWorld(UsersInput::Instance()->GetMousePos(), 1, Cam.GetViewMat(), Cam.GetProjectionMat(), winSize);
		Vec3<float> mouseWorldPos;
		bool isHit = CheckHitRayToStagePolygon(mousePos, Vec3<float>(mousePosFar - mousePos).GetNormal(), mouseWorldPos);

		// マウス座標を更新。
		mouseWorldPos.y = 0;
		m_mousePointer->m_transform.SetPos(mouseWorldPos);

		// ベクトルをすごくゆっくり補完する。
		float cross = m_forwardVec.Cross(m_inputVec).y;
		float dot = m_forwardVec.Dot(m_inputVec);
		if (dot < 0.0f) {

			cross = 1.0f * (cross < 0 ? -1.0f : 1.0f);

		}
		float nowAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		if (cross != 0) {

			// 保管量
			float rot = 0.04f * cross * SlowMgr::Instance()->m_slow;

			float rotAngle = nowAngle + rot;

			m_forwardVec = Vec3<float>(sinf(rotAngle), 0.0f, cosf(rotAngle));

		}

		// マウスカーソルのサイズを補完する。
		m_mousePointerScale += (MOUSE_POINTER_SCALE - m_mousePointerScale) / 2.0f;

		GenerateDriftParticle(nowAngle, cross);

	}
	else {

		// ブレーキタイマーが1以上だったらブレーキを離した瞬間ということ。
		if (0 < m_brakeTimer) {

			// 経過時間から割合を求める。
			float brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER));

			// 移動速度を求める。
			m_speed = brakeRate * (MIN_SPEED + MAX_SPEED);

			// ブレーキブーストの効果時間を計算する。
			if (0.5f < brakeRate) {
				m_brakeBoostTimer = brakeRate * MAX_BRAKE_TIMER;
			}

			// ダッシュした回数をカウントする。
			m_dashCounter += 1.0f * SlowMgr::Instance()->m_slow;

		}

		// 最大値、最小値を超えないようにする。
		if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;
		if (MAX_SPEED < m_speed) m_speed = MAX_SPEED;

		m_brakeTimer = 0;


		// マウスカーソルのサイズを補完する。
		if (0 < m_mousePointerScale) {
			m_mousePointerScale -= m_mousePointerScale / 2.0f;
		}

	}


}

void Player::Move(std::weak_ptr<BulletMgr> BulletMgr, bool IsStopFeverTimer)
{

	/*===== 移動処理 =====*/

	m_isPrevFever = m_isFever;

	if (BulletMgr.lock()->GetIsKillElecMushi()) {

		m_isFever = true;
		m_feverTime = FEVER_TIME;
		BulletMgr.lock()->BrakeIsKillElecMushi();

	}

	// ノックバックの移動量を設定。
	if (0 < m_knockBackSpeed) {

		// 移動させる。
		m_pos += m_knockBackVec * m_knockBackSpeed;

		// 移動量を0に近づける。
		m_knockBackSpeed -= m_knockBackSpeed / 18.0f;

	}

	// フィーバー状態だったら
	if (m_isFever) {

		// フィーバーの時間経過を止めるフラグが立っていたら減らさない。チュートリアルでフィーバーのタイマーを減らしたくない場合があったので作成しました。
		if (!IsStopFeverTimer) {

			m_feverTime -= 1.0f * SlowMgr::Instance()->m_slow;
			if (m_feverTime <= 0) {

				m_isFever = false;

			}

		}

		m_speed += (MAX_SPEED - m_speed) / 2.0f * SlowMgr::Instance()->m_slow;

	}
	else {

		// ブレーキ状態の有無に応じて移動速度を変える。
		if (m_isBrake) {

			m_speed += (BRAKE_SPEED - m_speed) / 10.0f * SlowMgr::Instance()->m_slow;

		}

	}

	// 座標を保存。
	m_prevPos = m_pos;

	// 移動させる。
	m_pos += m_forwardVec * m_speed * SlowMgr::Instance()->m_slow;
	m_movedLength += Vec3<float>(m_forwardVec * m_speed * SlowMgr::Instance()->m_slow).Length();

	float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
	float prevForwardVecAngle = atan2f(m_prevForwardVec.x, m_prevForwardVec.z);
	float subAngle = forwardVecAngle - prevForwardVecAngle;

	if (subAngle != 0) {

		m_rotX = -subAngle;

	}
	else {

		m_rotX -= m_rotX / 10.0f;

	}

}

void Player::UpdateEffect()
{

	/*===== エフェクト全般の更新処理 =====*/

	// ドリフトパーティクルの更新処理
	for (auto& index : m_driftParticle) {

		if (!index->GetIsActive()) continue;

		index->Update(m_pos);

	}


	// ダメージエフェクトの更新処理
	if (m_isDamageEffect) {

		m_damageEffectTimer += 1.0f * SlowMgr::Instance()->m_slow;
		if (DAMAGE_EFFECT_DRAW_CHANGE_SPAN <= m_damageEffectTimer) {

			// プレイヤーを表示しているかどうかで処理を変える。
			if (m_isDamageEffectDrawPlayer) {

				m_isDamageEffectDrawPlayer = false;

			}
			else {

				m_isDamageEffectDrawPlayer = true;

				++m_damageEffectCount;
				if (DAMAGE_EFFECT_COUNT <= m_damageEffectCount) {

					// ダメージエフェクトを終える。
					m_isDamageEffect = false;

				}

			}

			m_damageEffectTimer = 0;

		}

		// アウトラインの色を赤にする。
		m_outlineModel.ChangeColor(ColorPalette::S_RED);

		// 自機の色を赤にする。	
		m_modelObj->m_model->m_meshes[0].material->constData.pbr.baseColor = Vec3<float>(ColorPalette::S_RED.m_r, ColorPalette::S_RED.m_g, ColorPalette::S_RED.m_b);
		m_modelObj->m_model->m_meshes[0].material->Mapping();

		// 自機の色を赤にする。	
		m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor = Vec3<float>(ColorPalette::S_RED.m_r, ColorPalette::S_RED.m_g, ColorPalette::S_RED.m_b);
		m_modelObj->m_model->m_meshes[1].material->Mapping();

	}
	else {

		// アウトラインの色を緑にする。
		m_outlineModel.ChangeColor(ColorPalette::S_GREEN_COLOR);

		// 自機の色を元に戻す。	
		m_modelObj->m_model->m_meshes[0].material->constData.pbr.baseColor = materialColor;
		m_modelObj->m_model->m_meshes[0].material->Mapping();

		// 自機の色を緑にする。	
		m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor = Vec3<float>(ColorPalette::S_GREEN_COLOR.m_r, ColorPalette::S_GREEN_COLOR.m_g, ColorPalette::S_GREEN_COLOR.m_b);
		m_modelObj->m_model->m_meshes[1].material->Mapping();

	}



	// デバッグ用
	if (UsersInput::Instance()->KeyOnTrigger(DIK_O)) {

		float prevHP = m_hp;
		--m_hp;
		if (m_hp <= 0) m_hp = 0;
		if (m_hp <= 2 && 2 < prevHP) {
			m_isChangeRed = true;
			m_colorEasingTimer = 0;
		}

	}
	// デバッグ用
	if (UsersInput::Instance()->KeyOnTrigger(DIK_P)) {

		float prevHP = m_hp;
		++m_hp;

		if (3 <= m_hp && prevHP < 3) {

			m_isChangeGreen = true;

		}

		if (MAX_HP <= m_hp) {

			m_hp = MAX_HP;

		}

	}


	//パワーが溜まり切った演出
	if (UsersInput::Instance()->KeyInput(DIK_A))
	{
		m_outlineModel.EnoughPowerEffect();
	}

	bool dashFlag = 1.0f <= m_brakeBoostTimer;
	//ダッシュ中の演出
	if (dashFlag)
	{
		m_outlineModel.PowerUpEffect(50);
	}

	//プレイヤーのアウトライン用
	m_outlineModel.Update();

	//ダッシュ時のエフェクト
	m_dashLight->Update(dashFlag);
}


#include"DrawFunc_Append.h"
void Player::Draw(Camera& Cam, const bool& IsTitle)
{
	/*===== 描画処理 =====*/

	m_modelObj->m_transform.SetPos(m_pos);

	// 入力の角度を求める。
	Vec3<float> movedVec = m_forwardVec;
	//movedVec.Normalize();
	Vec2<float> inputVec = Vec2<float>(movedVec.x, movedVec.z);
	float inputAngle = atan2f(inputVec.x, inputVec.y);

	// Y軸回転のクォータニオン求める。
	auto resultY = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), inputAngle));
	// X軸回転のクォータニオン求める。
	auto resultX = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(m_forwardVec.x, m_forwardVec.y, m_forwardVec.z, 1.0f), m_rotX * 25.0f));

	// クォータニオンをかける。
	m_rotation = resultY * resultX;

	inputATan2f = atan2f(m_inputVec.x, m_inputVec.z);
	m_modelObj->m_transform.SetRotate(m_rotation);

	if (m_isDamageEffectDrawPlayer) {

		DrawFunc_Append::DrawModel(m_modelObj);

	}

	// マウスのカーソルを描画
	m_mousePointer->m_transform.SetScale(m_mousePointerScale);
	DrawFunc_Append::DrawModel(m_mousePointer);

	// ドリフトパーティクルの描画処理
	for (auto& index : m_driftParticle) {

		if (!index->GetIsActive()) continue;

		index->Draw();

	}

	if (!IsTitle) {

		// アウトラインを描画
		if (!m_isDamageEffect) {
			m_outlineModel.Draw(Cam);
		}

		m_dashLight->Draw(Cam);

		// 敵のHPの板ポリを描画
		for (auto& index : m_hpUI) {

			index->Draw();

		}

	}

}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== 弾を撃つ処理 =====*/

	// ドリフトしていなかったら処理とを飛ばす。
	if (!m_isBrake) return;

	m_shotTimer += 1.0f * SlowMgr::Instance()->m_slow;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		// 一番近くにいる敵を検索する。
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemyToVector(m_pos, m_inputVec, 0.8f);

		Vec3<float> shotEnemyPos = m_pos + m_inputVec * 20.0f;
		const float AUTO_AIM_SCALE = 15.0f;
		if (nearestEnemy != Vec3<float>(-1, -1, -1) && Vec3<float>(nearestEnemy - m_pos).Length() <= AUTO_AIM_SCALE) {

			shotEnemyPos = nearestEnemy;

		}

		// 多少分散させる。
		shotEnemyPos.x += KuroFunc::GetRand(-2.0f, 2.0f);
		shotEnemyPos.z += KuroFunc::GetRand(-2.0f, 2.0f);

		// 敵の方向に向かって弾を撃つ。
		BulletMgr.lock()->GeneratePlayerBullet(m_pos, (shotEnemyPos - m_pos).GetNormal());

	}

}

#include"DrawFunc3D.h"
void Player::DrawDebugInfo(Camera& Cam) {

	/*===== デバッグ情報を描画 =====*/

	if (m_isBrake) {

		Vec2<float> inputVec = Vec2<float>(m_inputVec.x, m_inputVec.z);
		float brakeRate = 0;

		// 経過時間から割合を求める。
		brakeRate = Saturate(static_cast<float>(m_brakeTimer) / static_cast<float>(MAX_BRAKE_TIMER)); // 0.5f ~ 1.5f の範囲


		DrawFunc3D::DrawLine(Cam, m_pos, m_pos + Vec3<float>(inputVec.x, 0.0f, inputVec.y).GetNormal() * (brakeRate * 20.0f), Color(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);

	}

}

#include"ShakeMgr.h"
void Player::CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== 当たり判定 =====*/

	// マップとの当たり判定。
	bool isHitMap = false;
	m_pos = KazCollisionHelper::KeepInMap(m_pos, MapSize);

	// エッジの判定。
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

	// ダメージエフェクト中は当たり判定を無効化する。
	if (m_isDamageEffect) return;

	// 壁際との当たり判定を行う。
	if (MapSize - MAP_EDGE <= m_pos.Length()) {

		Damage();

		// ノックバックの移動量を設定。
		m_knockBackVec = -m_pos.GetNormal();
		m_knockBackSpeed = KNOCK_BACK_SPEED;

	}

	// 敵弾との当たり判定。
	int hitCount = BulletMgr.lock()->CheckHitEnemyBullet(m_pos, SCALE);
	if (0 < hitCount) {
		// 当たった判定。
		Damage();
	}

	// ブースト量が一定以上だったらある程度の範囲の敵を倒す。
	m_brakeBoostTimer -= 1.0f * SlowMgr::Instance()->m_slow;
	if (0 < m_brakeBoostTimer) {

		//EnemyMgr.lock()->AttackEnemy(m_pos, BOOST_SCALE, BulletMgr);

	}
	else {

		m_brakeBoostTimer = 0;

	}

	// フィーバー状態だったら
	if (m_isFever) {

		int hitCount = EnemyMgr.lock()->AttackEnemy(m_pos, BOOST_SCALE, BulletMgr);

		if (0 < hitCount) {

			// シェイクをかける。
			ShakeMgr::Instance()->Shake(5);

		}

	}

	// 敵との当たり判定。
	if (EnemyMgr.lock()->CheckHitEnemy(m_pos, SCALE)) {
		Damage();
	}

}

void Player::Damage()
{
	// 当たった判定。
	m_damageEffectTimer = 0;
	m_damageEffectCount = 0;
	m_isDamageEffect = true;
	float prevHP = m_hp;
	--m_hp;
	if (m_hp <= 0) m_hp = 0;
	if (m_hp <= 2 && 2 < prevHP) {
		m_isChangeRed = true;
		m_colorEasingTimer = 0;
	}

	AudioApp::Instance()->PlayWaveDelay(m_damageSE);
}

void Player::Finalize()
{


}

float Player::Saturate(const float& Value)
{

	/*===== 01に納める。 =====*/

	float value = Value;
	if (value < 0.0f) value = 0.0f;
	if (1.0f < value) value = 1.0f;

	return value;
}

void Player::GenerateDriftParticle(const float& NowAngle, const float& Cross) {

	++m_driftParticleDelay;
	if (DRIFT_PARTICLE_DELAY < m_driftParticleDelay) {

		// ドリフトパーティクルを生成
		Vec3<float> generatePos;
		int generateCount = 0;
		const float MAX_GENERATE_COUNT = 3.0f;
		for (auto& index : m_driftParticle) {

			if (index->GetIsActive()) continue;

			// 外積の結果がマイナス(左)だったら
			float shotAngle = NowAngle;
			if (Cross < 0) {

				shotAngle += DirectX::XM_PI;

			}
			else {

				shotAngle -= DirectX::XM_PI;

			}

			// 生成場所を決める。
			if (static_cast<int>(&index - &m_driftParticle[0]) % 2 == 0) {

				// 前輪部分
				generatePos = m_pos + m_forwardVec * 3.0f;

			}
			else {

				// 後輪部分
				generatePos = m_pos - m_forwardVec * 2.5f;

			}

			index->Generate(generatePos, shotAngle, Cross);

			++generateCount;

			// ドリフト最初のFだったら。
			if (m_brakeTimer <= 15) {

				if (10 < generateCount) {

					break;

				}

			}
			else {

				if (MAX_GENERATE_COUNT * fabs(Cross) < generateCount) {

					break;

				}

			}

		}

		m_driftParticleDelay = 0;

	}


}

bool Player::ChangeBodyColorEasing(const float& AddEasingAmount, EASING_TYPE EasingType, EASE_CHANGE_TYPE EaseChangeType, const Vec3<float>& StartColor, const Vec3<float>& EndColor, const bool& IsEaseX)
{

	/*===== 色をイージングで変える =====*/

	m_colorEasingTimer += AddEasingAmount;
	if (1.0f <= m_colorEasingTimer) {

		m_colorEasingTimer = 1.0f;

	}

	// イージング量を計算する。
	float easingAmount = KuroMath::Ease(EaseChangeType, EasingType, m_colorEasingTimer, 0.0f, 1.0f);

	// 色を計算する。
	Vec3<float> nowHSV = StartColor;
	if (IsEaseX) {
		nowHSV.x += Vec3<float>(EndColor - StartColor).x * easingAmount;
	}
	else {
		nowHSV.z += Vec3<float>(EndColor - StartColor).z * easingAmount;
	}
	if (nowHSV.x < 0) {
		nowHSV.x += 360.0f;
	}
	m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor = ColorPalette::HSVtoRGB(nowHSV);
	m_modelObj->m_model->m_meshes[1].material->Mapping();

	// アウトラインの色を変える。
	Color outlineColor;
	outlineColor.m_r = m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor.x;
	outlineColor.m_g = m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor.y;
	outlineColor.m_b = m_modelObj->m_model->m_meshes[1].material->constData.pbr.baseColor.z;
	outlineColor.m_a = 1.0f;
	m_outlineModel.ChangeColor(outlineColor);

	return 1.0f <= m_colorEasingTimer;

}