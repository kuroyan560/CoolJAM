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

	/*===== �R���X�g���N�^ =====*/

	m_floorRenderTarget = D3D12App::Instance()->GenerateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, Color(0, 0, 0, 0), Vec2<int>(512, 512), L"FloorScreen");
	m_damageWallTimer = 0;
	m_enemyTutorialAppearTimer = 0;
	m_enemyTutorialExitTimer = 0;
	m_feverTutorialAppearTimer = 0;
	m_feverTutorialSpawnDelay = 0;
	m_isFever = false;
	m_isGenerateEnemy = false;

	// �`���[�g���A���p�̉摜�����[�h
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

	// �߂�̃A�C�R����\��
	m_returnTexture = D3D12App::Instance()->GenerateTextureBuffer("resource/user/tutorial/return.png");
	m_returnIconSize = Vec2<float>();
	m_isNearReturnIcon = false;

	// �Z�p�`�����[�h
	m_Hexagon = std::make_shared<TutorialHexagon>("resource/user/tutorial/hexagon.png");

	// �i�C�X�����[�h
	m_nice = std::make_shared<TutorialNice>("resource/user/tutorial/nice.png");

	m_textureIndex = 0;

}

void Tutorial::Init() {

	/*===== ���������� =====*/

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

	/*===== �X�V���� =====*/

	// �e�N�X�`���̃C���f�b�N�X���Ƃɏ����𕪂���B

	// 0 �����ő������
	if (m_textureIndex == 0) {

		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

		// �v���C���[�̈ړ������ʂ��������B
		PlayerIns.lock()->ClearMovedLength();

	}
	// 1 �}�E�X�N���b�N�Ńh���t�g�̐����B
	else if (m_textureIndex == 1) {

		// ���ʈړ������玟�̃`���[�g���A���ցB
		const float MOVE_AMOUNT = 200.0f;
		bool isNext = false;
		if (MOVE_AMOUNT <= PlayerIns.lock()->GetMovedLength()) {

			isNext = true;
			IsCameraHomePosition = true;

			// �Z�p�`�������B
			m_Hexagon->Exit();

			// �i�C�X���o���B
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}
		// ���S�ɓ��B���Ă�����J�������񂹂�B
		else if (m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			IsCameraHomePosition = false;

			// �o�����Ă��Ȃ�������o��������B
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		UpdateText(isNext);

	}
	// 2 �_�b�V���̐����B
	else if (m_textureIndex == 2) {

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

		// �v���C���[�̃_�b�V�������J�E���g���������B
		PlayerIns.lock()->ResetDashCount();

	}
	// 3 �_�b�V�����Ă݂悤�I
	else if (m_textureIndex == 3) {

		// �O��_�b�V�������玟�̃`���[�g���A���ցB
		const int DASH_COUNT = 3;
		bool isNext = false;
		if (DASH_COUNT <= PlayerIns.lock()->GetDashCount()) {

			isNext = true;
			IsCameraHomePosition = true;

			// �Z�p�`�������B
			m_Hexagon->Exit();

			// �i�C�X���o���B
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}
		// ���S�ɓ��B���Ă�����J�������񂹂�B
		else if (m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			IsCameraHomePosition = false;

			// �o�����Ă��Ȃ�������o��������B
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		UpdateText(isNext);

	}
	// �ǂɋC�����悤�I
	else if (m_textureIndex == 4) {

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// �h���t�g���ɒe�����I
	else if (m_textureIndex == 5) {

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// �G��|�����I
	else if (m_textureIndex == 6) {

		// �G�����݂��Ă��Ȃ��āA���݂̃e�L�X�g�����S�ɗ��Ă�����B
		if (!m_isGenerateEnemy && m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			// �J�������񂹂ēG�𐶐�����B
			m_isGenerateEnemy = true;
			IsCameraHomePosition = false;
			EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::STOPPING), MapSize);

			// �o�����Ă��Ȃ�������o��������B
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		// �G�𐶐����Ă��āA�G���S�Ď���ł�����B
		if (m_isGenerateEnemy && EnemyMgrIns.lock()->GetAllEnemyDead()) {

			// �J�����������B
			IsCameraHomePosition = true;

			// �Z�p�`�������B
			m_Hexagon->Exit();

			// �i�C�X���o���B
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}

		// �G���S�Ď���ł�����X�V�B
		bool isTransition = UpdateText(EnemyMgrIns.lock()->GetAllEnemyDead() && m_tutorialTexture[m_textureIndex]->GetCanExit());

		// ���֑J�ڂ���ۂɓG�𐶐������t���O������������B
		if (isTransition) {

			m_isGenerateEnemy = false;

		}

	}
	// �G���L���̐���
	else if (m_textureIndex == 7) {

		IsCameraHomePosition = true;

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// �G���L���o���B
	else if (m_textureIndex == 8) {

		// �G�����݂��Ă��Ȃ��āA���݂̃e�L�X�g�����S�ɗ��Ă�����B
		if (!m_isGenerateEnemy && m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			// �J�������񂹂ēG�𐶐�����B
			m_isGenerateEnemy = true;
			IsCameraHomePosition = false;
			EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::ELEC_MUSHI), MapSize);

			// �o�����Ă��Ȃ�������o��������B
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}

		// �G�𐶐����Ă��āA�G���S�Ď���ł�����B
		if (m_isGenerateEnemy && EnemyMgrIns.lock()->GetAllEnemyDead()) {

			// �J�����������B
			IsCameraHomePosition = true;

			// �Z�p�`�������B
			m_Hexagon->Exit();

			// �i�C�X���o���B
			if (!m_nice->GetIsActive()) {

				m_nice->Appear();

			}

		}

		// ���N���b�N�ōX�V�B
		bool isTransition = UpdateText(EnemyMgrIns.lock()->GetAllEnemyDead() && m_tutorialTexture[m_textureIndex]->GetCanExit());

		// ���֑J�ڂ���ۂɓG�𐶐������t���O������������B
		if (isTransition) {

			m_isGenerateEnemy = false;

		}

	}
	// �t�B�[�o�[���̃J�\�N�̘b
	else if (m_textureIndex == 9) {

		IsCameraHomePosition = true;

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// ���G�̘b�B
	else if (m_textureIndex == 10) {

		IsCameraHomePosition = true;

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// ���Ԍo�߂ŏI���̘b�B
	else if (m_textureIndex == 11) {

		IsCameraHomePosition = true;

		// ���N���b�N�ōX�V�B
		UpdateText(UsersInput::Instance()->MouseOnTrigger(LEFT));

	}
	// ���ԓ��ɓG���R�|�����I
	else if (m_textureIndex == 12) {

		// �v���C���[���t�B�[�o�[��Ԃ��A
		if (PlayerIns.lock()->GetIsFever() && m_tutorialTexture[m_textureIndex]->GetCanExit()) {

			// �J�������񂹂�B
			IsCameraHomePosition = false;

			// �G�𐶐������t���O�𗧂Ă�B
			m_isGenerateEnemy = true;

			// �G���R��������B
			++m_feverTutorialSpawnDelay;
			if (FEVER_TUTORIAL_SPAWN_DELAY <= m_feverTutorialSpawnDelay) {

				m_feverTutorialSpawnDelay = 0;

				// �G�𐶐�����B
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(100.0f, 0.0f, 0.0f), Vec3<float>(0, 0, -1), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(-100.0f, 0.0f, 0.0f), Vec3<float>(0, 0, 1), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, 100.0f), Vec3<float>(1, 0, 0), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, -100.0f), Vec3<float>(-1, 0, 0), static_cast<int>(ENEMY_INFO::ID::TORUS_MOVE), MapSize);

				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, -50.0f), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::TRACKING), MapSize);
				EnemyMgrIns.lock()->Generate(PlayerIns.lock()->GetPos(), Vec3<float>(0.0f, 0.0f, 50.0f), Vec3<float>(), static_cast<int>(ENEMY_INFO::ID::TRACKING), MapSize);

			}

			// �o�����Ă��Ȃ�������o��������B
			if (!m_Hexagon->GetIsAlive()) {

				m_Hexagon->Appear();

			}

		}
		else {

			// �J�����������B
			IsCameraHomePosition = true;

			// �G���������Ă�����S�Ă��E���B
			if (!EnemyMgrIns.lock()->GetAllEnemyDead()) {

				EnemyMgrIns.lock()->AllKill(BulletMgrIns);

			}

			// �Z�p�`�������B
			m_Hexagon->Exit();

			// �i�C�X���o���B
			if (!m_nice->GetIsActive() && !PlayerIns.lock()->GetIsFever()) {

				m_nice->Appear();

			}

		}

		// �t�B�[�o�[���I�������B
		UpdateText(!PlayerIns.lock()->GetIsFever() && m_isGenerateEnemy);

	}
	// �D���Ȃ悤�ɑ��삵�悤�̘b�B
	else if (m_textureIndex == 13) {

		// �����ƍX�V���Ȃ��B
		UpdateText(false);

		// �}�E�X�J�[�\���Ɩ߂�̃A�C�R���̓����蔻����s���B
		Vec2<float> mousePos = UsersInput::Instance()->GetMousePos();
		float length = Vec2<float>(mousePos - RETURN_ICON_POS).Length();
		if (length < RETURN_ICON_SIZE.x * 2.0f) {

			m_isNearReturnIcon = true;

		}
		else {

			m_isNearReturnIcon = false;

		}

		// �J�ڒ���������B
		if (IsTransition) {

			m_returnIconSize.x -= m_returnIconSize.x / 2.0f;
			m_returnIconSize.y -= m_returnIconSize.y / 2.0f;

		}
		else {

			// �߂�̃A�C�R���̃T�C�Y�𒲐��B
			if (m_isNearReturnIcon) {
				m_returnIconSize.x += (RETURN_ICON_EXP_SIZE.x - m_returnIconSize.x) / 2.0f;
				m_returnIconSize.y += (RETURN_ICON_EXP_SIZE.y - m_returnIconSize.y) / 2.0f;

				// �N���b�N����Ă�����J�ڂ�����B
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

	// �e�L�X�g�̍X�V����
	for (auto& index : m_tutorialTexture) {

		if (!index->GetIsActive()) continue;

		index->Update();

	}

	// �Z�p�`�̍X�V����
	m_Hexagon->Update();

	// �i�C�X�̍X�V����
	m_nice->Update();

}

#include "DrawFunc2D.h"
void Tutorial::Draw() {

	/*===== �`�揈�� =====*/

	// �����_�[�^�[�Q�b�g���Z�b�g�B
	KuroEngine::Instance()->Graphics().ClearRenderTarget(m_floorRenderTarget);
	KuroEngine::Instance()->Graphics().SetRenderTargets({ m_floorRenderTarget });

	for (auto& index : m_tutorialTexture) {

		if (!index->GetIsActive()) continue;

		index->Draw();

	}

	// �Z�p�`�̕`�揈��
	m_Hexagon->Draw();

	// �i�C�X�̕`��
	m_nice->Draw();

}

bool Tutorial::UpdateText(const bool& IsExit) {

	/*===== �N���b�N���ăe�L�X�g�𗬂������̍X�V���� =====*/

	// �o�����Ă�����B
	if (m_tutorialTexture[m_textureIndex]->GetIsActive()) {

		// �ޏo���\�ŁA���N���b�N���ꂽ��B
		bool isNiceExit = m_nice->GetIsExit() || !m_nice->GetIsActive();
		if (m_tutorialTexture[m_textureIndex]->GetCanExit() && IsExit && !m_tutorialTexture[m_textureIndex]->GetIsExit() && isNiceExit) {

			// �ޏo������B
			m_tutorialTexture[m_textureIndex]->Exit();

		}

	}

	// �o�����Ă��Ȃ�������o��������B
	if (!m_tutorialTexture[m_textureIndex]->GetIsActive()) {

		m_tutorialTexture[m_textureIndex]->Activate();

	}

	// ���ׂĂ̏������I����Ă�����B
	if (m_tutorialTexture[m_textureIndex]->GetisFinish()) {

		m_tutorialTexture[m_textureIndex]->Init();
		++m_textureIndex;
		if (static_cast<int>(m_tutorialTexture.size()) - 1 <= m_textureIndex) {

			m_textureIndex = static_cast<int>(m_tutorialTexture.size()) - 1;

		}

		// ���ɑJ�ڂ����B
		return true;

	}

	// �J�ڂ��ĂȂ��B
	return false;

}

bool Tutorial::GetPlayerFeverGaugeStop(std::weak_ptr<EnemyMgr> EnemyMgrIns) {

	/*===== �v���C���[�ƃt�B�[�o�[�̍X�V���~�߂邩�̃t���O =====*/

	bool isStop = false;

	// �G���S�Ď���ł��āA�C�[�W���O���I����Ă�����B
	bool isEnemyAllDead = m_isGenerateEnemy && EnemyMgrIns.lock()->GetAllEnemyDead() && 1.0f <= m_tutorialTexture[m_textureIndex]->GetEasingTimer();
	// �e�N�X�`���̃C���f�b�N�X��8(�G���L����|��) ���� Exit��������B
	if (m_textureIndex == 8 && (m_tutorialTexture[m_textureIndex]->GetIsExit() || isEnemyAllDead)) isStop = true;

	// �e�N�X�`���̃C���f�b�N�X��9~11(�t�B�[�o�[�̐���)��������
	if (9 <= m_textureIndex && m_textureIndex <= 11) isStop = true;

	// �e�N�X�`���̃C���f�b�N�X��12(���ԓ��ɓG���R�|����) ���� !Exit��������B
	if (m_textureIndex == 12 && !m_tutorialTexture[m_textureIndex]->GetCanExit()) isStop = true;

	return isStop;

}

void Tutorial::DrawReturnIcon() {

	// �߂�̃A�C�R����`��B
	DrawFunc2D::DrawExtendGraph2D(RETURN_ICON_POS - m_returnIconSize, RETURN_ICON_POS + m_returnIconSize, m_returnTexture);

}