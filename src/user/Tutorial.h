
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

	/*===== �����o�ϐ� =====*/
	// �Z�p�`
	std::shared_ptr<TutorialHexagon> m_Hexagon;

	// �i�C�X
	std::shared_ptr<TutorialNice> m_nice;

	// �߂�̃A�C�R���B
	std::shared_ptr<TextureBuffer> m_returnTexture;
	bool m_isNearReturnIcon;
	Vec2<float> m_returnIconSize;
	Vec2<float> RETURN_ICON_SIZE = Vec2<float>(64.0f, 64.0f);
	Vec2<float> RETURN_ICON_EXP_SIZE = Vec2<float>(94.0f, 94.0f);
	Vec2<float> RETURN_ICON_POS = Vec2<float>(RETURN_ICON_SIZE.x * 2.0f, WinApp::Instance()->GetExpandWinSize().y - RETURN_ICON_SIZE.y * 2.0f);

	/*-- �`���[�g���A���Ɏg�p����ϐ� --*/

	int m_damageWallTimer;		// �_���[�W�ǂ̃`���[�g���A���̕\�����ԁB
	const int DAMAGE_WALL_TUTORIAL_TIMER = 240;

	int m_enemyTutorialAppearTimer;		// �G�̃`���[�g���A���œG�̐����̃e�L�X�g��\�����鎞�ԁB
	const int ENEMY_TUTORIAL_APPEAR_TIMER = 180;
	int m_enemyTutorialExitTimer;		// �G�̃`���[�g���A���œG��|���Ă��玟�̃`���[�g���A���ɑJ�ڂ���܂ł̎��ԁB
	const int ENEMY_TUTORIAL_EXIT_TIMER = 60;

	int m_feverTutorialAppearTimer;		// �t�B�[�o�[�̃`���[�g���A���ŃG���L�����o������܂ł̎��ԁB
	const int FEVER_TUTORIAL_APPEAR_TIMER = 180;

	int m_feverTutorialSpawnDelay;		// �t�B�[�o�[���ɓG�𕦂�����x��
	const int FEVER_TUTORIAL_SPAWN_DELAY = 60;
	bool m_isFever;						// �t�B�[�o�[�ɂȂ������ǂ���

	// �`���[�g���A���p�̃e�N�X�`��
	std::array<std::shared_ptr<TutorialText>, 14> m_tutorialTexture;

	int m_textureIndex;	// ���݂̃`���[�g���A���p�e�N�X�`���̃C���f�b�N�X

	bool m_isGenerateEnemy;	// �`���[�g���A������g�p����A�G�𐶐��������̃t���O


public:

	/*===== �����o�֐� =====*/

	Tutorial();
	void Init();
	void Update(std::weak_ptr<Player> PlayerIns, std::weak_ptr<EnemyMgr> EnemyMgrIns, std::weak_ptr<BulletMgr> BulletMgrIns, bool& IsCameraHomePosition, const float& MapSize, bool& IsTransition);
	void Draw();
	void DrawReturnIcon();

	// �v���C���[�ƃt�B�[�o�[�Q�[�W�̍X�V���~�߂邩�̃Q�b�^�B
	bool GetPlayerFeverGaugeStop(std::weak_ptr<EnemyMgr> EnemyMgrIns);

private:

	// �N���b�N���ăe�L�X�g�𗬂������̍X�V����
	bool UpdateText(const bool& IsExit);

};