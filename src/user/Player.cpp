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

	Init();

	m_model = Importer::Instance()->LoadModel("resource/user/", "player.glb");

}

void Player::Init()
{

	/*===== 初期化処理 =====*/

	m_shotTimer = 0;
	m_isEdge = false;
	m_move = m_initMove;
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
	CheckHit(BulletMgr, MapSize, EdgeScope);

}

void Player::Input(Camera& Cam, const Vec2<float>& WindowSize)
{

	/*====== 入力処理 =====*/

	// スティックの入力を取得。
	Vec2<float> stickInput = UsersInput::Instance()->GetLeftStickVecRawFuna(0);

	// マウスの入力からベクトルを求める。

	auto myPos = m_transform.GetPos();
	Vec2<float> screenPos = KuroFunc::ConvertWorldToScreen(myPos, Cam.GetViewMat(), Cam.GetProjectionMat(), WindowSize);
	Vec2<float> mouseDir = Vec2<float>(UsersInput::Instance()->GetMousePos() - screenPos).GetNormal();
	auto inputVec = Vec3<float>(mouseDir.y, 0.0f, mouseDir.x);

	// デッドラインを設ける。
	const float INPUT_DEADLINE = 0.25f;
	// 入力されていたら。
	if (INPUT_DEADLINE < stickInput.Length()) {

		// 入力を保存。
		inputVec = Vec3<float>(stickInput.y, 0.0f, stickInput.x);

	}

	// ブレーキ入力を保存。
	bool isDrift = UsersInput::Instance()->ControllerInput(0, A) || UsersInput::Instance()->MouseInput(LEFT);

	// デバッグ機能
	if (UsersInput::Instance()->ControllerInput(0, B)) {

		Init();

	}

}

void Player::Move()
{

	/*===== 移動処理 =====*/

	// ブレーキ状態の有無に応じて移動速度を変える。

	// 移動させる。
	auto myPos = m_transform.GetPos();
	myPos += m_move;
	m_transform.SetPos(myPos);
}

#include"DrawFunc3D.h"
void Player::Draw(Camera& Cam) {

	/*===== 描画処理 =====*/

	m_transform.SetFront(m_move.GetNormal());
	DrawFunc3D::DrawNonShadingModel(m_model, m_transform, Cam);


}

void Player::Shot(std::weak_ptr<BulletMgr> BulletMgr, std::weak_ptr<EnemyMgr> EnemyMgr) {

	/*===== 弾を撃つ処理 =====*/

	++m_shotTimer;
	if (SHOT_TIMER <= m_shotTimer) {

		m_shotTimer = 0;

		auto myPos = m_transform.GetPos();

		// 一番近くにいる敵を検索する。
		Vec3<float> nearestEnemy = EnemyMgr.lock()->SearchNearestEnemy(myPos);

		// 敵の方向に向かって弾を撃つ。
		BulletMgr.lock()->GeneratePlayerBullet(myPos, (nearestEnemy - myPos).GetNormal());

	}

}

void Player::DrawDebugInfo(Camera& Cam) {

	/*===== デバッグ情報を描画 =====*/

}

void Player::CheckHit(std::weak_ptr<BulletMgr> BulletMgr, const float& MapSize, const float& EdgeScope)
{

	/*===== 当たり判定 =====*/

	// マップとの当たり判定。
	auto myPos = m_transform.GetPos();

	myPos = KeepInMap(myPos, MapSize);

	// エッジの判定。
	m_isEdge = MapSize - myPos.Length() < EdgeScope;

	// 敵弾との当たり判定。
	int hitCount = BulletMgr.lock()->CheckHitEnemyBullet(myPos, SCALE);

	m_transform.SetPos(myPos);
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
	auto myPos = m_transform.GetPos();
	Vec2<float> pos = Vec2(myPos.x, myPos.z);
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

			float length = (Vec2<float>(myPos.x, myPos.z) - index).Length();
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

#include"imguiApp.h"
void Player::OnImguiDebug()
{
	ImGui::Begin("Player");

	ImGui::Text("move : { %.2f , %.2f , %.2f }", m_move.x, m_move.y, m_move.z);

	static float s_initMove[3] = { m_move.x,m_move.y,m_move.z };
	if (ImGui::DragFloat3("initMove", s_initMove, 0.01f))
	{
		m_initMove = { s_initMove[0],s_initMove[1],s_initMove[2] };
	}

	ImGui::End();
}