#include "Tutorial.h"
#include "EnemyWave.h"
#include "Player.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
#include "KuroEngine.h"
#include "TutorialText.h"
#include "TutorialHexagon.h"
#include "TutorialNice.h"

Tutorial::Tutorial() {

	/*===== コンストラクタ =====*/

	m_floorRenderTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, Color(0, 0, 0, 0), Vec2<int>(512, 512), L"FloorScreen");
	m_damageWallTimer = 0;
	m_enemyTutorialAppearTimer = 0;
	m_enemyTutorialExitTimer = 0;
	m_feverTutorialAppearTimer = 0;
	m_feverTutorialSpawnDelay = 0;
	m_isFever = false;
	m_isGenerateEnemy = false;

	// チュートリアル用の画像をロード
	m_tutorialTexture[0] = std::make_shared<TutorialText>("resource/user/tutorial/0.png");
	m_tutorialTexture[1] = std::make_shared<TutorialText>("resource/user/tutorial/1.png");
	m_tutorialTexture[2] = std::make_shared<TutorialText>("resource/user/tutorial/1_1.png");
	m_tutorialTexture[3] = std::make_shared<TutorialText>("resource/user/tutorial/1_2.png");
	m_tutorialTexture[4] = std::make_shared<TutorialText>("resource/user/tutorial/2.png");
	m_tutorialTexture[5] = std::make_shared<TutorialText>("resource/user/tutorial/3.png");
	m_tutorialTexture[6] = std::make_shared<TutorialText>("resource/user/tutorial/4.png");
	m_tutorialTexture[7] = std::make_shared<TutorialText>("resource/user/tutorial/5.png");
	m_tutorialTexture[8] = std::make_shared<TutorialText>("resource/user/tutorial/6.png");
	m_tutorialTexture[9] = std::make_shared<TutorialText>("resource/user/tutorial/7.png");
	m_tutorialTexture[10] = std::make_shared<TutorialText>("resource/user/tutorial/8.png");
	m_tutorialTexture[11] = std::make_shared<TutorialText>("resource/user/tutorial/9.png");
	m_tutorialTexture[12] = std::make_shared<TutorialText>("resource/user/tutorial/10.png");
	m_tutorialTexture[13] = std::make_shared<TutorialText>("resource/user/tutorial/11.png");

	// 戻るのアイコンを表示
	m_returnTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/tutorial/return.png");
	m_returnIconSize = Vec2<float>();
	m_isNearReturnIcon = false;

	// 六角形をロード
	m_Hexagon = std::make_shared<TutorialHexagon>("resource/user/tutorial/hexagon.png");

	// ナイスをロード
	m_nice = std::make_shared<TutorialNice>("resource/user/tutorial/nice.png");

	m_textureIndex = 0;

}

void Tutorial::Init() {

	/*===== 初期化処理 =====*/

	m_damageWallTimer = 0;
	m_enemyTutorialAppearTimer = 0;
	m_enemyTutorialExitTimer = 0;
	m_feverTutorialAppearTimer = 0;
	m_feverTutorialSpawnDelay = 0;
	m_textureIndex = 0;
	m_isFever = false;
	m_isGenerateEnemy = false;
	m_isNearReturnIcon = false;
	m_Hexagon->Init();
	m_nice->Init();
	m_returnIconSize = Vec2<float>();

	for (auto& index : m_tutorialTexture) {

		index->Init();

	}

}

void Tutorial::Update(std::weak_ptr<Player> PlayerIns, std::weak_ptr<EnemyMgr> EnemyMgrIns, std::weak_ptr<BulletMgr> BulletMgrIns, bool& IsCameraHomePosition, const float& MapSize, bool& IsTransition) {

	/*===== 更新処理 =====*/

	// テクスチャのインデックスごとに処理を分ける。

	// 0 自動で走る説明
	if (m_textureIndex == 0) {

		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

		// プレイヤーの移動した量を初期化。
		PlayerIns.lock()->ClearMovedLength();

	}
	// 1 マウスクリックでドリフトの説明。
	else if (m_textureIndex == 1) {

		// 一定量移動したら次のチュートリアルへ。
		const float MOVE_AMOUNT = 200.0f;
		bool isNext = false;
		if (MOVE_AMOUNT <= PlayerIns.lock()->GetMovedLength()) {

			isNext = true;
			IsCameraHomePosition = true;

			// 六角形を消す。
			m_Hexagon->Exit();

			// ナイスを出す。
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}
		// 中心に到達していたらカメラを寄せる。
		else if (m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			IsCameraHomePosition = false;

			// 出現していなかったら出現させる。
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		UpdateText(isNext);

	}
	// 2 ダッシュの説明。
	else if (m_textureIndex == 2) {

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

		// プレイヤーのダッシュしたカウントを初期化。
		PlayerIns.lock()->ResetDashCount();

	}
	// 3 ダッシュしてみよう！
	else if (m_textureIndex == 3) {

		// 三回ダッシュしたら次のチュートリアルへ。
		const int DASH_COUNT = 3;
		bool isNext = false;
		if (DASH_COUNT <= PlayerIns.lock()->GetDashCount()) {

			isNext = true;
			IsCameraHomePosition = true;

			// 六角形を消す。
			m_Hexagon->Exit();

			// ナイスを出す。
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}
		// 中心に到達していたらカメラを寄せる。
		else if (m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			IsCameraHomePosition = false;

			// 出現していなかったら出現させる。
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		UpdateText(isNext);

	}
	// 壁に気をつけよう！
	else if (m_textureIndex == 4) {

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// ドリフト中に弾を撃つ！
	else if (m_textureIndex == 5) {

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// 敵を倒そう！
	else if (m_textureIndex == 6) {

		// 敵が存在していなくて、現在のテキストが中心に来ていたら。
		if (!m_isGenerateEnemy && m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			// カメラを寄せて敵を生成する。
			m_isGenerateEnemy = true;
			IsCameraHomePosition = false;
			EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::STOPPING), MapSize);

			// 出現していなかったら出現させる。
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		// 敵を生成していて、敵が全て死んでいたら。
		if (m_isGenerateEnemy && EnemyMgrIns.lock()->GetAllEnemyDead()) {

			// カメラを引く。
			IsCameraHomePosition = true;

			// 六角形を消す。
			m_Hexagon->Exit();

			// ナイスを出す。
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}

		// 敵が全て死んでいたら更新。
		bool isTransition = UpdateText(EnemyMgrIns.lock()->GetAllEnemyDead() && m_tutorialTexture[m_textureIndex]->GetCanExit());

		// 次へ遷移する際に敵を生成したフラグを初期化する。
		if (isTransition) {

			m_isGenerateEnemy = false;

		}

	}
	// エレキ虫の説明
	else if (m_textureIndex == 7) {

		IsCameraHomePosition = true;

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// エレキ虫出現。
	else if (m_textureIndex == 8) {

		// 敵が存在していなくて、現在のテキストが中心に来ていたら。
		if (!m_isGenerateEnemy && m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			// カメラを寄せて敵を生成する。
			m_isGenerateEnemy = true;
			IsCameraHomePosition = false;
			EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::ELEC_MUSHI), MapSize);

			// 出現していなかったら出現させる。
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		// 敵を生成していて、敵が全て死んでいたら。
		if (m_isGenerateEnemy && EnemyMgrIns.lock()->GetAllEnemyDead()) {

			// カメラを引く。
			IsCameraHomePosition = true;

			// 六角形を消す。
			m_Hexagon->Exit();

			// ナイスを出す。
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}

		// 左クリックで更新。
		bool isTransition = UpdateText(EnemyMgrIns.lock()->GetAllEnemyDead() && m_tutorialTexture[m_textureIndex]->GetCanExit());

		// 次へ遷移する際に敵を生成したフラグを初期化する。
		if (isTransition) {

			m_isGenerateEnemy = false;

		}

	}
	// フィーバー時のカソクの話
	else if (m_textureIndex == 9) {

		IsCameraHomePosition = true;

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// 無敵の話。
	else if (m_textureIndex == 10) {

		IsCameraHomePosition = true;

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// 時間経過で終わりの話。
	else if (m_textureIndex == 11) {

		IsCameraHomePosition = true;

		// 左クリックで更新。
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// 時間内に敵を沢山倒そう！
	else if (m_textureIndex == 12) {

		// プレイヤーがフィーバー状態か、
		if (PlayerIns.lock()->GetIsFever() && m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			// カメラを寄せる。
			IsCameraHomePosition = false;

			// 敵を生成したフラグを立てる。
			m_isGenerateEnemy = true;

			// 敵を沢山沸かせる。
			++m_feverTutorialSpawnDelay;
			if (FEVER_TUTORIAL_SPAWN_DELAY <= m_feverTutorialSpawnDelay) {

				m_feverTutorialSpawnDelay = 0;

				// 敵を生成する。
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(100.0f, 0.0f, 0.0f), Vec3<float>(0, 0, -1), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(-100.0f, 0.0f, 0.0f), Vec3<float>(0, 0, 1), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, 100.0f), Vec3<float>(1, 0, 0), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, -100.0f), Vec3<float>(-1, 0, 0), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);

				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, -50.0f), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::TRACKING), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::TRACKING), MapSize);

			}

			// 出現していなかったら出現させる。
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}
		else {

			// カメラを引く。
			IsCameraHomePosition = true;

			// 敵が生存していたら全てを殺す。
			if (!EnemyMgrIns.lock()->GetAllEnemyDead()) {

				EnemyMgrIns.lock()->AllKill(BulletMgrIns);

			}

			// 六角形を消す。
			m_Hexagon->Exit();

			// ナイスを出す。
			if (!m_nice->GetIsActive() && !PlayerIns.lock()->GetIsFever()) {

				m_nice->Appear();

			}

		}

		// フィーバーが終わったら。
		UpdateText(!PlayerIns.lock()->GetIsFever() && m_isGenerateEnemy);

	}
	// 好きなように操作しようの話。
	else if (m_textureIndex == 13) {

		// ずっと更新しない。
		UpdateText(false);

		// マウスカーソルと戻るのアイコンの当たり判定を行う。
		Vec2<float> mousePos = UsersInput::Instance()->GetMousePos();
		float length = Vec2<float>(mousePos - RETURN_ICON_POS).Length();
		if (length < RETURN_ICON_SIZE.x * 2.0f) {

			m_isNearReturnIcon = true;

		}
		else {

			m_isNearReturnIcon = false;

		}

		// 遷移中だったら。
		if (IsTransition) {

			m_returnIconSize.x -= m_returnIconSize.x / 2.0f;
			m_returnIconSize.y -= m_returnIconSize.y / 2.0f;

		}
		else {

			// 戻るのアイコンのサイズを調整。
			if (m_isNearReturnIcon) {
				m_returnIconSize.x += (RETURN_ICON_EXP_SIZE.x - m_returnIconSize.x) / 2.0f;
				m_returnIconSize.y += (RETURN_ICON_EXP_SIZE.y - m_returnIconSize.y) / 2.0f;

				// クリックされていたら遷移させる。
				if (UsersInput::Instance()->MouseOnTrigger(LEFT)) {

					IsTransition = true;

				}

			}
			else {
				m_returnIconSize.x += (RETURN_ICON_SIZE.x - m_returnIconSize.x) / 2.0f;
				m_returnIconSize.y += (RETURN_ICON_SIZE.y - m_returnIconSize.y) / 2.0f;
			}

		}

	}

	// テキストの更新処理
	for (auto& index : m_tutorialTexture) {

		if (!index->GetIsActive()) continue;

		index->Update();

	}

	// 六角形の更新処理
	m_Hexagon->Update();

	// ナイスの更新処理
	m_nice->Update();

}

#include "DrawFunc2D.h"
void Tutorial::Draw() {

	/*===== 描画処理 =====*/

	// レンダーターゲットをセット。
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_floorRenderTarget);
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_floorRenderTarget });

	for (auto& index : m_tutorialTexture) {

		if (!index->GetIsActive()) continue;

		index->Draw();

	}

	// 六角形の描画処理
	m_Hexagon->Draw();

	// ナイスの描画
	m_nice->Draw();

}

bool Tutorial::UpdateText(const bool& IsExit) {

	/*===== クリックしてテキストを流すだけの更新処理 =====*/

	// 出現していたら。
	if (m_tutorialTexture[m_textureIndex]->GetIsActive()) {

		// 退出が可能で、左クリックされたら。
		bool isNiceExit = m_nice->GetIsExit() || !m_nice->GetIsActive();
		if (m_tutorialTexture[m_textureIndex]->GetCanExit() && IsExit && !m_tutorialTexture[m_textureIndex]->GetIsExit() && isNiceExit) {

			// 退出させる。
			m_tutorialTexture[m_textureIndex]->Exit();

		}

	}

	// 出現していなかったら出現させる。
	if (!m_tutorialTexture[m_textureIndex]->GetIsActive()) {

		m_tutorialTexture[m_textureIndex]->Activate();

	}

	// すべての処理が終わっていたら。
	if (m_tutorialTexture[m_textureIndex]->GetisFinish()) {

		m_tutorialTexture[m_textureIndex]->Init();
		++m_textureIndex;
		if (static_cast<int>(m_tutorialTexture.size()) - 1 <= m_textureIndex) {

			m_textureIndex = static_cast<int>(m_tutorialTexture.size()) - 1;

		}

		// 次に遷移した。
		return true;

	}

	// 遷移してない。
	return false;

}

bool Tutorial::GetPlayerFeverGaugeStop(std::weak_ptr<EnemyMgr> EnemyMgrIns) {

	/*===== プレイヤーとフィーバーの更新を止めるかのフラグ =====*/

	bool isStop = false;

	// 敵が全て死んでいて、イージングが終わっていたら。
	bool isEnemyAllDead = m_isGenerateEnemy && EnemyMgrIns.lock()->GetAllEnemyDead() && 1.0f <= m_tutorialTexture[m_textureIndex]->GetEasingTimer();
	// テクスチャのインデックスが8(エレキ虫を倒す) かつ Exitだったら。
	if (m_textureIndex == 8 && (m_tutorialTexture[m_textureIndex]->GetIsExit() || isEnemyAllDead)) isStop = true;

	// テクスチャのインデックスが9~11(フィーバーの説明)だったら
	if (9 <= m_textureIndex && m_textureIndex <= 11) isStop = true;

	// テクスチャのインデックスが12(時間内に敵を沢山倒そう) かつ !Exitだったら。
	if (m_textureIndex == 12 && !m_tutorialTexture[m_textureIndex]->GetCanExit()) isStop = true;

	return isStop;

}

void Tutorial::DrawReturnIcon() {

	// 戻るのアイコンを描画。
	DrawFunc2D::DrawExtendGraph2D(RETURN_ICON_POS - m_returnIconSize, RETURN_ICON_POS + m_returnIconSize, m_returnTexture);

}