#include "Player.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "KuroFunc.h"
#include "GameManager.h"
#include "DriftParticle.h"

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
	m_hp = MAX_HP;

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");
	m_pileModel = Importer::Instance()->LoadModel("resource/user/", "pile.glb");

	for (auto& index : m_driftParticle) {

		index = std::make_shared<DriftParticle>();

	}

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

	for (auto& index : m_driftParticle) {

		index->Init();

	}

}

void Player::Update(Camera& Cam, std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const Vec2<float>& WindowSize, const float& MapSize, const float& EdgeScope)
{

	/*===== 更新処理 =====*/

	// 入力処理
	Input(Cam, WindowSize);

	// 移動処理
	Move();

	// 弾発射処理
	Shot(BulletMgr, EnemyMgr);

	// 当たり判定処理
	CheckHit(BulletMgr, EnemyMgr, MapSize, EdgeScope);

	// エフェクト全般の更新処理
	UpdateEffect();

}

void Player::Input(Camera& Cam, const Vec2<float>& WindowSize)
{

	/*====== 入力処理 =====*/

	// スティックの入力を取得。
	Vec2<float> stickInput = UsersInput::Instance()->GetLeftStickVecRawFuna(0);

	// マウスの入力からベクトルを求める。
	Vec2<float> screenPos = KuroFunc::ConvertWorldToScreen(m_pos, Cam.GetViewMat(), Cam.GetProjectionMat(), WindowSize);
	Vec2<float> mouseDir = Vec2<float>(UsersInput::Instance()->GetMousePos() - screenPos).GetNormal();
	m_inputVec = Vec3<float>(mouseDir.y, 0.0f, mouseDir.x);

	// デッドラインを設ける。
	const float INPUT_DEADLINE = 0.25f;
	// 入力されていたら。
	if (INPUT_DEADLINE < stickInput.Length()) {

		// 入力を保存。
		m_inputVec = Vec3<float>(stickInput.y, 0.0f, stickInput.x);

	}

	// 正面ベクトルを保存。
	m_prevForwardVec = m_forwardVec;


	// ブレーキ入力を保存。
	m_isBrake = UsersInput::Instance()->ControllerInput(0, A) || UsersInput::Instance()->MouseInput(LEFT);
	if (m_isBrake) {



		// クリックした最初のFで杭までの長さを保存する。
		if (m_brakeTimer == 0) {

			// マウスのわーるど座標を求める。
			Vec2<int> winSize = Vec2<int>(WindowSize.x, WindowSize.y);
			Vec3<float> mousePos = KuroFunc::ConvertScreenToWorld(UsersInput::Instance()->GetMousePos(), 0, Cam.GetViewMat(), Cam.GetProjectionMat(), winSize);
			Vec3<float> mousePosFar = KuroFunc::ConvertScreenToWorld(UsersInput::Instance()->GetMousePos(), 1, Cam.GetViewMat(), Cam.GetProjectionMat(), winSize);
			Vec3<float> mouseWorldPos;
			bool isHit = CheckHitRayToStagePolygon(mousePos, Vec3<float>(mousePosFar - mousePos).GetNormal(), mouseWorldPos);

			// 杭のいちを設定。
			m_pileTransform.SetPos(mouseWorldPos);

			pileLength = Vec3<float>(m_pos - mouseWorldPos).Length();

		}

		// 杭座標から離れすぎないようにする。
		if (pileLength < Vec3<float>(m_pileTransform.GetPos() - m_pos).Length()) {

			m_pos = m_pileTransform.GetPos() + Vec3<float>(m_pos - m_pileTransform.GetPos()).GetNormal() * pileLength;

		}



		++m_brakeTimer;

		// ブレーキブーストのタイマーを初期化する。
		m_brakeBoostTimer = 0;

		// ベクトルをすごくゆっくり補完する。
		float cross = m_forwardVec.Cross(m_inputVec).y;
		float dot = m_forwardVec.Dot(m_inputVec);
		if (dot < 0.0f) {

			cross = 1.0f * (cross < 0 ? -1.0f : 1.0f);

		}
		float nowAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		if (cross != 0) {

			// 保管量
			float rot = 0.04f * cross;

			float rotAngle = nowAngle + rot;

			m_forwardVec = Vec3<float>(sinf(rotAngle), 0.0f, cosf(rotAngle));

		}

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

		}

		// 最大値、最小値を超えないようにする。
		if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;
		if (MAX_SPEED < m_speed) m_speed = MAX_SPEED;

		m_brakeTimer = 0;

	}

}

void Player::Move()
{

	/*===== 移動処理 =====*/

	// ブレーキ状態の有無に応じて移動速度を変える。
	if (m_isBrake) {

		m_speed += (BRAKE_SPEED - m_speed) / 10.0f;

	}

	// 座標を保存。
	m_prevPos = m_pos;

	// 移動させる。
	m_pos += m_forwardVec * m_speed;

	// 正面ベクトルが回転した量を計算する。
	if (UsersInput::Instance()->KeyInput(DIK_SPACE)) {

		int a = 0;

	}

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

		++m_damageEffectTimer;
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

	// 赤色に変えるフラグが経っていたら。
	if (m_isChangeRed) {

		bool isEnd = ChangeBodyColorEasing(0.1f, Sine, In, GREEN_HSV, RED_HSV, true);

		// イージングが終わったら。
		if (isEnd) {

			m_isRed = true;
			m_isChangeRed = false;
			m_colorEasingTimer = 0.0f;

		}

	}

	// 緑色に変えるフラグが立っていたら。
	else if (m_isChangeGreen) {

		bool isEnd = ChangeBodyColorEasing(0.1f, Sine, In, RED_HSV, GREEN_HSV, true);

		if (isEnd) {

			m_isChangeGreen = false;
			m_colorEasingTimer = 0.0f;

		}

	}

	// HPが3以下だったら自機を点滅させる。
	else if (m_hp <= 2) {

		// 赤の時。
		if (m_isRed) {

			// 赤でいる時間のタイマーを更新する。
			++m_RedTime;
			if (RED_TIME[m_hp] <= m_RedTime) {

				// 一定時間経過してたらイージングで緑にする。
				bool isEnd = ChangeBodyColorEasing(ADD_COLOR_EASING_TIMER[m_hp], Exp, In, RED_HSV, DARK_RED_HSV, false);

				if (isEnd) {

					m_isRed = false;
					m_colorEasingTimer = 0.0f;

				}

			}

		}
		else {

			// 一定時間経過してたらイージングで暗い赤にする。
			bool isEnd = ChangeBodyColorEasing(ADD_COLOR_EASING_TIMER[m_hp], Exp, In, DARK_RED_HSV, RED_HSV, false);

			if (isEnd) {

				m_isRed = true;
				m_colorEasingTimer = 0.0f;
				m_RedTime = 0;

			}

		}

	}

}

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);

	// 入力の角度を求める。
	Vec3<float> movedVec = m_forwardVec;
	//movedVec.Normalize();
	Vec2<float> inputVec = Vec2<float>(movedVec.x, movedVec.z);
	float inputAngle = atan2f(inputVec.x, inputVec.y);

	if (UsersInput::Instance()->KeyInput(DIK_O)) {

		m_rotX += 0.01f;

	}
	if (UsersInput::Instance()->KeyInput(DIK_P)) {

		m_rotX -= 0.01f;

	}

	// Y軸回転のクォータニオン求める。
	auto resultY = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), inputAngle));
	// X軸回転のクォータニオン求める。
	auto resultX = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(m_forwardVec.x, m_forwardVec.y, m_forwardVec.z, 1.0f), m_rotX * 25.0f));

	// クォータニオンをかける。
	auto resultQ = resultY * resultX;

	m_transform.SetRotate(resultQ);

	if (m_isDamageEffectDrawPlayer) {

		DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

	}

	// 杭を描画
	DrawFunc3D::DrawNonShadingModel(m_pileModel, m_pileTransform, Cam);

	// ドリフトパーティクルの描画処理
	for (auto& index : m_driftParticle) {

		if (!index->GetIsActive()) continue;

		index->Draw(Cam);

	}

}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== 弾を撃つ処理 =====*/

	++m_shotTimer;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		// 一番近くにいる敵を検索する。
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemy(m_pos);

		// Vec3<float>(-1,-1,-1)だったら敵がいないので撃たない。
		if (nearestEnemy == Vec3<float>(-1, -1, -1)) return;

		// 多少分散させる。
		Vec3<float> shotEnemyPos = nearestEnemy;
		shotEnemyPos.x += KuroFunc::GetRand(-3.0f, 3.0f);
		shotEnemyPos.z += KuroFunc::GetRand(-3.0f, 3.0f);

		// 敵の方向に向かって弾を撃つ。
		BulletMgr.lock()->GeneratePlayerBullet(m_pos, (shotEnemyPos - m_pos).GetNormal());

	}

}

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

void Player::CheckHit(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== 当たり判定 =====*/

	// マップとの当たり判定。
	m_pos = KeepInMap(m_pos, MapSize);

	// エッジの判定。
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

	// 敵弾との当たり判定。
	int hitCount = BulletMgr.lock()->CheckHitEnemyBullet(m_pos, SCALE);

	// 敵とのエッジの判定。
	if (EnemyMgr.lock()->CheckEnemyEdge(m_pos, SCALE)) {

		m_isEdge = true;

	}

	// ブースト量が一定以上だったらある程度の範囲の敵を倒す。
	--m_brakeBoostTimer;
	if (0 < m_brakeBoostTimer) {

		EnemyMgr.lock()->AttackEnemy(m_pos, BOOST_SCALE);

	}
	else {

		m_brakeBoostTimer = 0;

	}

	// 敵との当たり判定。
	if (EnemyMgr.lock()->CheckHitEnemy(m_pos, SCALE)) {

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

	}

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

Vec3<float> Player::KeepInMap(Vec3<float>& Pos, const float& MapSize)
{

	/*===== マップ内に収まるようにする =====*/

	Vec3<float> pos = Pos;
	if (MapSize <= pos.Length()) {

		pos = pos.GetNormal() * MapSize;

	}

	return pos;

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

Vec3<float> Player::RGBtoHSV(const Vec3<float>& RGB)
{
	double hue = 0;
	double s = 0;
	double value = 0;
	double max = 0;
	double min = 0;
	int colorIndex = 0;
	Vec3<float> rgb = RGB;
	SearchMaxMinColor(rgb, max, min, colorIndex);

	/*RGB -> HSV*/
	//色相を求める
	//Rが最大値の場合
	if (colorIndex == 0) {
		hue = 60.0f * ((rgb.y - rgb.z) / (max - min));
	}
	//Gが最大値の場合
	if (colorIndex == 1) {
		hue = 60.0f * (rgb.z - rgb.x) / (max - min) + 120;
	}
	//Bが最大値の場合
	if (colorIndex == 2) {
		hue = 60.0f * (rgb.x - rgb.y) / (max - min) + 240;
	}
	//すべての色が同じだった場合
	if (colorIndex == 3) {
		hue = 0;
	}
	//色相がマイナス値だった場合は360を足す
	if (hue < 0) {
		hue += 360;
	}

	//彩度を求める
	s = (max - min) / max * 255.0f;

	//明度を求める
	value = max;

	return Vec3<float>(hue, s, value);

}

Vec3<float> Player::HSVtoRGB(const Vec3<float>& HSV)
{
	/*HSV -> RGB*/

	//最大値と最小値を求める
	float max = HSV.z;
	float min = max - ((HSV.y / 255.0f) * max);

	//RGBを求める
	float hue = HSV.x;
	Vec3<float> rgb;
	if (hue >= 0 && hue < 60) {
		rgb.x = max;
		rgb.y = (hue / 60) * (max - min) + min;
		rgb.z = min;
	}
	else if (hue >= 60 && hue < 120) {
		rgb.x = ((120 - hue) / 60) * (max - min) + min;
		rgb.y = max;
		rgb.z = min;
	}
	else if (hue >= 120 && hue < 180) {
		rgb.x = min;
		rgb.y = max;
		rgb.z = ((hue - 120) / 60) * (max - min) + min;
	}
	else if (hue >= 180 && hue < 240) {
		rgb.x = min;
		rgb.y = ((240 - hue) / 60) * (max - min) + min;
		rgb.z = max;
	}
	else if (hue >= 240 && hue < 300) {
		rgb.x = ((hue - 240) / 60) * (max - min) + min;
		rgb.y = min;
		rgb.z = max;
	}
	else if (hue >= 300 && hue <= 360) {
		rgb.x = max;
		rgb.y = min;
		rgb.z = ((360 - hue) / 60) * (max - min) + min;
	}

	rgb = Vec3<float>(rgb.x, rgb.y, rgb.z);

	return rgb;
}

void Player::SearchMaxMinColor(Vec3<float>& Color, double& max, double& min, int& rgb)
{

	float r = Color.x;
	float g = Color.y;
	float b = Color.z;
	max = r;
	min = r;
	rgb = 0;

	//すべての色が同じ値の場合
	if (r == g && g == b) {
		max = r;
		min = r;
		rgb = 3;
		return;
	}

	//最大値を求める
	if (max < g) {
		max = g;
		rgb = 1;
	}
	if (max < b) {
		max = b;
		rgb = 2;
	};
	//最小値を求める
	if (min > g) {
		min = g;
	}
	if (min > b) {
		min = b;
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
	m_model->m_meshes[1].material->constData.pbr.baseColor = HSVtoRGB(nowHSV);
	m_model->m_meshes[1].material->Mapping();

	return 1.0f <= m_colorEasingTimer;

}

bool Player::CheckHitRayToStagePolygon(const Vec3<float>& RayStartPos, const Vec3<float>& RayDir, Vec3<float>& HitPos)
{

	// 平面の頂点
	Vec3<float> p1 = Vec3<float>(10000, 0, 10000);
	Vec3<float> p2 = Vec3<float>(-10000, 0, -10000);
	Vec3<float> p3 = Vec3<float>(10000, 0, -10000);

	// レイの開始地点から平面におろした垂線の長さを求める
	Vec3<float> planeNorm = Vec3<float>(0, 1, 0);
	float rayToOriginLength = RayStartPos.Dot(planeNorm);
	float planeToOriginLength = p1.Dot(planeNorm);
	// 視点から平面におろした垂線の長さ
	float perpendicularLine = rayToOriginLength - planeToOriginLength;

	// 三角関数を利用して視点から衝突店までの距離を求める
	float dist = planeNorm.Dot(RayDir);
	float impDistance = perpendicularLine / -dist;

	// 衝突地点
	HitPos = RayStartPos + RayDir * impDistance;

	return true;
}
