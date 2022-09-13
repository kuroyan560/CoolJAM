#pragma once
#include "D3D12App.h"
#include "D3D12Data.h"
#include "WinApp.h"
#include <memory>
#include <array>

class Player;
class EnemyMgr;
class BulletMgr;
class TutorialText;
class TutorialHexagon;
class TutorialNice;

class Tutorial {

private:

	/*===== メンバ変数 =====*/
	// 六角形
	std::shared_ptr<TutorialHexagon> m_Hexagon;

	// ナイス
	std::shared_ptr<TutorialNice> m_nice;

	// 戻るのアイコン。
	std::shared_ptr<TextureBuffer> m_returnTexture;
	bool m_isNearReturnIcon;
	Vec2<float> m_returnIconSize;
	Vec2<float> RETURN_ICON_SIZE = Vec2<float>(64.0f, 64.0f);
	Vec2<float> RETURN_ICON_EXP_SIZE = Vec2<float>(94.0f, 94.0f);
	Vec2<float> RETURN_ICON_POS = Vec2<float>(RETURN_ICON_SIZE.x * 2.0f, WinApp::Instance()->GetExpandWinSize().y - RETURN_ICON_SIZE.y * 2.0f);

	/*-- チュートリアルに使用する変数 --*/

	int m_damageWallTimer;		// ダメージ壁のチュートリアルの表示時間。
	const int DAMAGE_WALL_TUTORIAL_TIMER = 240;

	int m_enemyTutorialAppearTimer;		// 敵のチュートリアルで敵の説明のテキストを表示する時間。
	const int ENEMY_TUTORIAL_APPEAR_TIMER = 180;
	int m_enemyTutorialExitTimer;		// 敵のチュートリアルで敵を倒してから次のチュートリアルに遷移するまでの時間。
	const int ENEMY_TUTORIAL_EXIT_TIMER = 60;

	int m_feverTutorialAppearTimer;		// フィーバーのチュートリアルでエレキ虫が出現するまでの時間。
	const int FEVER_TUTORIAL_APPEAR_TIMER = 180;

	int m_feverTutorialSpawnDelay;		// フィーバー中に敵を沸かせる遅延
	const int FEVER_TUTORIAL_SPAWN_DELAY = 60;
	bool m_isFever;						// フィーバーになったかどうか

	// チュートリアル用のテクスチャ
	std::array<std::shared_ptr<TutorialText>, 14> m_tutorialTexture;

	int m_textureIndex;	// 現在のチュートリアル用テクスチャのインデックス

	bool m_isGenerateEnemy;	// チュートリアル中二使用する、敵を生成したかのフラグ


public:

	/*===== メンバ関数 =====*/

	Tutorial();
	void Init();
	void Update(std::weak_ptr<Player> PlayerIns, std::weak_ptr<EnemyMgr> EnemyMgrIns, std::weak_ptr<BulletMgr> BulletMgrIns, bool& IsCameraHomePosition, const float& MapSize, bool& IsTransition);
	void Draw();
	void DrawReturnIcon();

	// プレイヤーとフィーバーゲージの更新を止めるかのゲッタ。
	bool GetPlayerFeverGaugeStop(std::weak_ptr<EnemyMgr> EnemyMgrIns);

private:

	// クリックしてテキストを流すだけの更新処理
	bool UpdateText(const bool& IsExit);

};