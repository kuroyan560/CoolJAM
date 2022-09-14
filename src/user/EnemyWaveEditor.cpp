#include "EnemyWaveEditor.h"
#include"imguiApp.h"
#include"EnemyWaveMgr.h"
#include"EnemyWave.h"
#include"EnemyMgr.h"
#include"KuroFunc.h"
#include"EnemyWaveLoader.h"
#include "SlowMgr.h"
#include"Player.h"

void EnemyWaveEditor::EditWithImgui(EnemyWaveMgr& WaveMgr, std::weak_ptr<EnemyMgr> EnemyMgr, Player& Player)
{
	//�Z�[�u���邩����
	if (m_saveMode)
	{
		ImGui::SetNextWindowPos(ImVec2(640, 320));
		ImGui::Begin("SaveEnemyWave");

		static char s_saveFileNameBuff[256];
		ImGui::InputText("SaveFileName", s_saveFileNameBuff, 256);

		//�����̃t�@�C��������ꍇ�x����
		if (KuroFunc::ExistFile(EnemyWaveLoader::s_dir + s_saveFileNameBuff + EnemyWaveLoader::s_extention))
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "This file name is already used. Are you sure overwriting?");
		}

		//���s
		if (ImGui::Button("Done"))
		{
			EnemyWaveLoader::Save(s_saveFileNameBuff, WaveMgr.m_waves, m_finalWaveTime);
			m_saveMode = false;
		}

		ImGui::End();
	}
	else if (m_loadMode)
	{
		ImGui::SetNextWindowPos(ImVec2(640, 320));
		ImGui::Begin("LoadEnemyWave");

		static char s_loadFileNameBuff[256];
		ImGui::InputText("LoadWaveName", s_loadFileNameBuff, 256);

		//�t�@�C�������邩�m�F
		if (KuroFunc::ExistFile(EnemyWaveLoader::s_dir + s_loadFileNameBuff + EnemyWaveLoader::s_extention))
		{
			//���s
			if (ImGui::Button("Done"))
			{
				WaveMgr.m_waves = EnemyWaveLoader::Load(s_loadFileNameBuff, m_finalWaveTime);
				m_loadMode = false;
			}
		}

		ImGui::End();
	}

	if (m_saveMode || m_loadMode)return;

	ImGui::Begin("EnemyWaveEditor", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				m_saveMode = true;
			}
			if (ImGui::MenuItem("Load"))
			{
				m_loadMode = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("WaveTest", nullptr, nullptr, !m_test))
			{
				WaveMgr.Init(m_finalWaveTime, WaveMgr.m_waves[m_waveIdx]->m_waveStartFrame);
				EnemyMgr.lock()->Init();
				Player.Init();
				m_test = true;
			}
			if (ImGui::MenuItem("FullTest", nullptr, nullptr, !m_test))
			{
				WaveMgr.Init(m_finalWaveTime);
				Player.Init();
				EnemyMgr.lock()->Init();
				m_test = true;
			}
			if (ImGui::MenuItem("Stop", nullptr, nullptr, m_test))
			{
				EnemyMgr.lock()->AllDisappear();
				m_test = false;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	//�e�X�g��
	if (m_test)
	{
		ImGui::Text("Now Testing ...");

		if (WaveMgr.EndAllWaves())
		{
			ImGui::Text("All waves end. Please stop this test.");
		}
		ImGui::Separator();

		ImGui::Text("NowTimer : { %d }", WaveMgr.m_frameTimer);

		if (WaveMgr.m_nowWaveIdx + 1 <= WaveMgr.m_waves.size() - 1)
		{
			ImGui::Text("NextWaveBegin : { %d }", WaveMgr.m_waves[WaveMgr.m_nowWaveIdx + 1]->m_waveStartFrame);
		}
		else
		{
			ImGui::Text("NextWaveBegin : { Final Wave }");
			ImGui::Text("UntilFinish : { %d }", WaveMgr.m_finalWaveTimer);
		}

		ImGui::End();
		return;
	}

	ImGui::Text("NowWaveCount : { %d }", WaveMgr.m_waves.size());
	if (ImGui::DragInt("FinalWaveTime", &m_finalWaveTime) && m_finalWaveTime < 0)m_finalWaveTime = 0;

	ImGui::Separator();

	//�E�F�[�u�ǉ�
	if (ImGui::Button("AddWave"))
	{
		WaveMgr.AddWave(std::make_shared<EnemyWave>(30, false));
		m_waveIdx++;
	}
	//�E�F�[�u�폜
	ImGui::SameLine();
	if (ImGui::Button("RemoveWave"))
	{
		WaveMgr.m_waves.erase(WaveMgr.m_waves.begin() + m_waveIdx);
	}

	//�E�F�[�u����Ȃ�P�ǉ����ĕҏW�Ώۂɂ���
	if (WaveMgr.m_waves.empty())
	{
		WaveMgr.m_waves.emplace_back(std::make_shared<EnemyWave>(1, false));
	}
	//�E�F�[�u�I��
	ImGui::InputInt("NowEditWave", &m_waveIdx, 1, 0, WaveMgr.m_waves.size() - 1);

	ImGui::Separator();


	m_waveIdx = std::clamp(m_waveIdx, 0, static_cast<int>(WaveMgr.m_waves.size()) - 1);
	auto wave = WaveMgr.m_waves[m_waveIdx];

	if (ImGui::DragInt("StartFrame", &wave->m_waveStartFrame) && wave->m_waveStartFrame < 0)
	{
		wave->m_waveStartFrame = 0;	//�}�C�i�X�h�~
	}
	ImGui::Checkbox("IsBonusStage", &wave->m_isBounusStageFlag);

	ImGui::Separator();

	//�G�l�~�[�ҏW
	auto& enemys = wave->m_enemys;
	//�G�l�~�[�ǉ�
	if (ImGui::Button("AddEnemy"))
	{
		if (!enemys.empty())
		{
			wave->AddEnemy(
				enemys[m_enemyIdx].m_pos, enemys[m_enemyIdx].m_forwardVec,
				enemys[m_enemyIdx].m_id, enemys[m_enemyIdx].m_generateFrame,
				enemys[m_enemyIdx].m_shotTimer);
			m_enemyIdx = static_cast<int>(enemys.size() - 1);
		}
		else
		{
			wave->AddEnemy({ 0,0,0 }, { 1,0,0 }, ENEMY_INFO::ID::STOPPING, 60, 60);
		}
	}

	if (!enemys.empty())
	{
		const int enemyCount = static_cast<int>(enemys.size());

		ImGui::SameLine();
		//�G�l�~�[�𐶐����Ƀ\�[�g
		if (ImGui::Button("Sort"))
		{
			std::sort(enemys.begin(), enemys.end(), [](ENEMY_INFO::SPAWN_INFO& a, ENEMY_INFO::SPAWN_INFO b) {
				return a.m_generateFrame < b.m_generateFrame;
				});
		}

		ImGui::Separator();
		//�ꊇ�ύX
		static int s_bulkChangeShotTimer = 60;
		ImGui::InputInt("BulkChange_ShotTime", &s_bulkChangeShotTimer);
		if (ImGui::Button("BulkChange"))
		{
			for (auto& e : enemys)e.m_shotTimer = s_bulkChangeShotTimer;
		}

		//�G�l�~�[�ꗗ
		if (ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 360), ImGuiWindowFlags_NoTitleBar))
		{
			for (int i = 0; i < enemyCount; ++i)
			{
				const auto& info = enemys[i];
				std::string str = std::to_string(i) + " : " + std::to_string(info.m_generateFrame) + " - " + ENEMY_INFO::GetIDName(info.m_id);
				if (ImGui::RadioButton(str.c_str(), &m_enemyIdx, i))m_enemyIdx = i;
			}

			ImGui::EndChild();
		}

		//�C���f�b�N�X�͈͒����h�~
		m_enemyIdx = std::clamp(m_enemyIdx, 0, enemyCount - 1);

		//���ݑI�𒆂̃G�l�~�[
		auto& enemy = enemys[m_enemyIdx];

		//��ʑI��
		if (ImGui::BeginCombo("Type", ENEMY_INFO::GetIDName(enemy.m_id).c_str()))
		{
			for (ENEMY_INFO::ID id = ENEMY_INFO::ID(0); id < ENEMY_INFO::ID::ID_COUNT; id = ENEMY_INFO::ID((int)id + 1))
			{
				bool isSelected = (enemy.m_id == id);
				if (ImGui::Selectable(ENEMY_INFO::GetIDName(id).c_str(), isSelected))
				{
					enemy.m_id = id;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		//�����t���[��
		if (ImGui::DragInt("GenerateFrame", &enemy.m_generateFrame) && enemy.m_generateFrame < 1)
		{
			enemy.m_generateFrame = 1;	//�}�C�i�X�h�~
		}

		//�����ʒu
		float s_generatePos[3] = { enemy.m_pos.x,enemy.m_pos.y,enemy.m_pos.z };
		if (ImGui::DragFloat3("GeneratePos", s_generatePos), 0.05f)
		{
			enemy.m_pos = { s_generatePos[0],s_generatePos[1],s_generatePos[2] };
		}

		//�O�x�N�g��
		float s_forwardVec[3] = { enemy.m_forwardVec.x,enemy.m_forwardVec.y,enemy.m_forwardVec.z };
		if (ImGui::DragFloat3("ForwardVec", s_forwardVec), 0.05f)
		{
			enemy.m_forwardVec = { s_forwardVec[0],s_forwardVec[1],s_forwardVec[2] };
		}

		//�e���̎��ԃX�p��
		if (ImGui::DragInt("ShotTime", &enemy.m_shotTimer) && enemy.m_shotTimer < 0)
		{
			enemy.m_shotTimer = 0;	//�}�C�i�X�h�~
		}

		//�G�l�~�[�폜
		if (ImGui::Button("RemoveEnemy"))
		{
			enemys.erase(enemys.begin() + m_enemyIdx);
		}
	}


	ImGui::End();
}

#include"DrawFunc3D.h"
#include"Importer.h"
#include"Model.h"
void EnemyWaveEditor::DebugDraw(Camera& Cam, EnemyWaveMgr& WaveMgr)
{
	if (!m_isActive)return;
	if (m_test)return;

	static std::shared_ptr<Model>model;
	if (!model)
	{
		model = Importer::Instance()->LoadModel("resource/user/", "enemy.glb");
	}

	auto& wave = WaveMgr.m_waves[m_waveIdx];

	for (auto& e : wave->m_enemys)
	{
		Transform transform;
		transform.SetPos(e.m_pos);
		transform.SetScale(3.0f);
		DrawFunc3D::DrawNonShadingModel(model, transform, Cam);
		DrawFunc3D::DrawLine(Cam, e.m_pos, e.m_pos + e.m_forwardVec * 5.0f, Color(), 0.3f);
	}
}
