#include "Player.h"
#include "../engine/UsersInput.h"
#include "Importer.h"
#include "Model.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "KuroFunc.h"
#include "GameManager.h"

Player::Player()
{

	/*===== コンストラクタ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
	m_prevForwardVec = DEF_FORWARDVEC;
	m_speed = MIN_SPEED;
	m_brakeTimer = 0;
	m_rotX = 0.01f;
	m_rotX = 0;
	m_brakeBoostTimer = 0;
	m_shotTimer = 0;
	m_isEdge = false;
	m_isBrake = false;

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");

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
	m_rotX = 0.01f;
	m_shotTimer = 0;
	m_brakeBoostTimer = 0;
	m_isEdge = false;
	m_isBrake = false;

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

		++m_brakeTimer;

		// ブレーキブーストのタイマーを初期化する。
		m_brakeBoostTimer = 0;

		// ベクトルをすごくゆっくり補完する。
		float cross = m_forwardVec.Cross(m_inputVec).y;
		if (cross != 0) {

			// 保管量
			float rot = 0.04f * cross;

			float nowAngle = atan2f(m_forwardVec.x, m_forwardVec.z) + rot;

			m_forwardVec = Vec3<float>(sinf(nowAngle), 0.0f, cosf(nowAngle));

		}

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

	// デバッグ機能
	if (UsersInput::Instance()->ControllerInput(0, B)) {

		Init();

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

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);

	// 入力の角度を求める。
	Vec3<float> movedVec = m_pos - m_prevPos;
	movedVec.Normalize();
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
	auto resultX = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(m_forwardVec.x, m_forwardVec.y, m_forwardVec.z, 1.0f), m_rotX * 20.0f));

	// クォータニオンをかける。
	auto resultQ = resultY * resultX;

	m_transform.SetRotate(resultQ);

	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);


}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== 弾を撃つ処理 =====*/

	++m_shotTimer;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		// 一番近くにいる敵を検索する。
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemy(m_pos);

		// 敵の方向に向かって弾を撃つ。
		BulletMgr.lock()->GeneratePlayerBullet(m_pos, (nearestEnemy - m_pos).GetNormal());

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

#include <limits>
bool Player::CheckHitModel(std::weak_ptr<Model> Model, Transform ModelTransform, const Vec3<float>& RayDir, const float& LineLength, Vec3<float>& ShortesetPos)
{

	/*===== モデルとレイの線分での当たり判定 =====*/

	// モデルから頂点情報を抜き取る。
	std::vector<Vec3<float>> vertex;
	for (auto& meshIndex : Model.lock()->m_meshes) {

		for (auto& vertexIndex : meshIndex.mesh->vertices) {

			vertex.emplace_back(vertexIndex.pos);

		}

	}

	// 頂点にTransformをかける。
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	matWorld *= DirectX::XMMatrixScaling(ModelTransform.GetScale().x, ModelTransform.GetScale().y, ModelTransform.GetScale().z);
	matWorld *= ModelTransform.GetRotate();
	matWorld *= DirectX::XMMatrixTranslation(ModelTransform.GetPos().x, ModelTransform.GetPos().y, ModelTransform.GetPos().z);
	for (auto& index : vertex) {

		index = MulMat(index, matWorld);

	}

	// 頂点数を保存。
	const int VERTEX_COUNT = static_cast<int>(vertex.size());

	// 頂点数が0だったらアサート。
	if (VERTEX_COUNT <= 0) assert(0);

	// 当たり判定を行い、衝突地点を保存する。
	std::vector<Vec2<float>> hitPos;
	// プレイヤー側のレイの設定。
	Vec2<float> pos = Vec2(m_pos.x, m_pos.z);
	Vec2<float> rayDir = Vec2 <float>(RayDir.x, RayDir.z);
	for (auto& index : vertex) {

		// モデル側のレイの設定。
		Vec2<float> modelPosA = Vec2<float>(index.x, index.z);
		Vec2<float> modelPosB;

		// 現在のインデックスのひとつ先の頂点を求める。
		int nowIndex = static_cast<int>(&index - &vertex[0]);
		if (VERTEX_COUNT - 1 <= nowIndex) {
			modelPosB = Vec2<float>(vertex[0].x, vertex[0].z);
		}
		else {
			modelPosB = Vec2<float>(vertex[nowIndex + 1].x, vertex[nowIndex + 1].z);
		}

		bool isHit = IsIntersected(pos, pos + rayDir * LineLength, modelPosA, modelPosB);
		if (isHit) {

			hitPos.emplace_back(CalIntersectPoint(pos, pos + rayDir * LineLength, modelPosA, modelPosB));

		}

	}

	// 当たった頂点が一つもなかったら。
	if (static_cast<int>(hitPos.size()) <= 0) {

		return false;

	}
	else {

		// 最短距離を求める。
		float shortestLength = std::numeric_limits<float>().max();
		int shortestIndex = 0;

		for (auto& index : hitPos) {

			float length = (Vec2<float>(m_pos.x, m_pos.z) - index).Length();
			if (shortestLength < length) continue;

			shortestLength = length;
			shortestIndex = static_cast<int>(&index - &hitPos[0]);

		}

		ShortesetPos = Vec3<float>(hitPos[shortestIndex].x, 0, hitPos[shortestIndex].y);

	}

	return true;

}

bool Player::IsIntersected(const Vec2<float>& posA1, const Vec2<float>& posA2, const Vec2<float>& posB1, const Vec2<float>& posB2)
{
	/*--線分の外積を計算して交差判定を行う--*/
	//第一回 線分Aから見たBの交差判定
	Vec2<float> buffA = Vec2<float>(posA2.x - posA1.x, posA2.y - posA1.y);
	buffA.Normalize();
	Vec2<float> buffB = Vec2<float>(posB1.x - posA1.x, posB1.y - posA1.y);
	buffB.Normalize();
	Vec2<float> buffC = Vec2<float>(posA2.x - posA1.x, posA2.y - posA1.y);
	buffC.Normalize();
	Vec2<float> buffD = Vec2<float>(posB2.x - posA1.x, posB2.y - posA1.y);
	buffD.Normalize();
	float buffE = buffA.Cross(buffB);
	float buffF = buffC.Cross(buffD);
	float result1 = buffE * buffF;
	bool zero1 = false;
	if (buffE * buffF == 0) zero1 = true;
	//第二回 線分Bから見たAの交差判定
	buffA = Vec2<float>(posB2.x - posB1.x, posB2.y - posB1.y);
	buffA.Normalize();
	buffB = Vec2<float>(posA1.x - posB1.x, posA1.y - posB1.y);
	buffB.Normalize();
	buffC = Vec2<float>(posB2.x - posB1.x, posB2.y - posB1.y);
	buffC.Normalize();
	buffD = Vec2<float>(posA2.x - posB1.x, posA2.y - posB1.y);
	buffD.Normalize();
	buffE = buffA.Cross(buffB);
	buffF = buffC.Cross(buffD);
	float result2 = buffE * buffF;
	bool zero2 = false;
	if (buffE * buffF == 0) zero2 = true;

	//線分が交差している時は、線分から見て交差判定したい線分の端点2つが両側にある時。
	//外積で左右判定をすると、交差している時は値の結果が+と-になる。
	//つまり両方の外積を掛けて結果が-になった場合のみ交差している。
	//線分AからみてBを判定、線分BからみてAを判定と二通り判定を行う。
	//この2つの判定結果を掛けた時に-、若しくは0の時2つの線分は交差している。
	if ((result1 <= 0 && result2 <= 0) ||
		(result1 <= 0 && zero2) ||
		(zero1 && result2 <= 0)) {
		return true;
	}
	return false;
}

Vec2<float> Player::CalIntersectPoint(Vec2<float> posA1, Vec2<float> posA2, Vec2<float> posB1, Vec2<float> posB2)
{
	//交点を求める この式は資料そのまま
	Vec2<float> buff = Vec2<float>(posB2.x - posB1.x, posB2.y - posB1.y);
	double d1 = abs(buff.Cross(Vec2<float>(posA1.x - posB1.x, posA1.y - posB1.y)));
	double d2 = abs(buff.Cross(Vec2<float>(posA2.x - posB1.x, posA2.y - posB1.y)));
	double t = d1 / (d1 + d2);

	return Vec2<float>(posA1.x + (posA2.x - posA1.x) * t, posA1.y + (posA2.y - posA1.y) * t);
}
