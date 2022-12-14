#include "Player.h"
#include "../engine/UsersInput.h"
#include"Importer.h"
#include "Model.h"

Player::Player()
{

	/*===== コンストラクタ =====*/

	m_pos = Vec3<float>();
	m_inputVec = Vec3<float>();
	m_forwardVec = DEF_FORWARDVEC;
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
	m_forwardVec = DEF_FORWARDVEC;
	m_speed = 0;
	m_brakeTimer = 0;
	m_brakeBoostSpeed = 0;
	m_isEdge = false;
	m_isBrake = false;

}

void Player::Update(const float& MapSize, const float& EdgeScope)
{

	/*===== 更新処理 =====*/

	// 入力処理
	Input();

	// 移動処理
	Move();

	// 当たり判定処理
	CheckHit(MapSize, EdgeScope);

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
		m_inputVec = Vec3<float>(stickInput.y, 0.0f, stickInput.x);

	}

	// ブレーキ入力を保存。
	bool isBrakeBoostNow = 0.1f < m_brakeBoostSpeed;
	m_isBrake = UsersInput::Instance()->ControllerInput(0, A) && !isBrakeBoostNow;
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
		float forwardVecAngle = atan2f(m_forwardVec.x, m_forwardVec.z);
		forwardVecAngle += handleRot;

		// 加算した角度をベクトルに直す。
		m_forwardVec = Vec3<float>(sinf(forwardVecAngle), 0.0f, cosf(forwardVecAngle));

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

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== 描画処理 =====*/

	m_transform.SetPos(m_pos);

	// 入力の角度を求める。
	float inputAngle = atan2f(m_inputVec.x, m_inputVec.z);
	m_transform.SetRotate(DirectX::XMMatrixRotationY(inputAngle));

	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);

}

void Player::CheckHit(const float& MapSize, const float& EdgeScope)
{

	/*===== 当たり判定 =====*/

	// マップとの当たり判定。
	m_pos = KeepInMap(m_pos, MapSize);

	// エッジの判定。
	m_isEdge = MapSize - m_pos.Length() < EdgeScope;

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
