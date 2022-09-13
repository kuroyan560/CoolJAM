#include "EnemyWaveEditor.h"
#include"imguiApp.h"
#include"EnemyWaveMgr.h"
#include"EnemyWave.h"
#include"EnemyMgr.h"

void EnemyWaveEditor::EditWithImgui(EnemyWaveMgr& WaveMgr, std::weak_ptr<EnemyMgr> EnemyMgr)
{

	ImGui::Begin("EnemyWaveEditor", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{

			}
			if (ImGui::MenuItem("Load"))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("WaveTest", nullptr, nullptr, !m_test))
			{
				WaveMgr.Init(WaveMgr.m_waves[m_waveIdx]->m_waveStartFrame);
				EnemyMgr.lock()->Init();
				m_test = true;
			}
			if (ImGui::MenuItem("FullTest", nullptr, nullptr, !m_test))
			{
				WaveMgr.Init();
				EnemyMgr.lock()->Init();
				m_test = true;
			}
			if (ImGui::MenuItem("Stop", nullptr, nullptr, m_test))
			{
				WaveMgr.Init();
				EnemyMgr.lock()->Init();
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
		ImGui::End();
		return;
	}

	//�E�F�[�u�ǉ�
	if (ImGui::Button("AddWave"))
	{
		WaveMgr.AddWave(std::make_shared<EnemyWave>(30, false));
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
	ImGui::DragInt("NowEditWave", &m_waveIdx, 1, 0, WaveMgr.m_waves.size() - 1);

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
		wave->AddEnemy({ 0,0,0 }, { 0,0,0 }, ENEMY_INFO::ID::STOPPING, 0);
	}

	if (!enemys.empty())
	{
		const int enemyCount = static_cast<int>(enemys.size());

		//�G�l�~�[�𐶐����Ƀ\�[�g
		std::sort(enemys.begin(), enemys.end(), [](ENEMY_INFO::SPAWN_INFO& a, ENEMY_INFO::SPAWN_INFO b) {
			return a.m_generateFrame < b.m_generateFrame;
			});

		//�G�l�~�[�ꗗ
		if (ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar))
		{
			for (int i = 0; i < enemyCount; ++i)
			{
				const auto& info = enemys[i];
				std::string str = std::to_string(i) + " : " + std::to_string(info.m_generateFrame) + " - " + ENEMY_INFO::GetIDName(info.m_id);
				ImGui::RadioButton(str.c_str(), &m_enemyIdx);
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
		}
		//�����t���[��
		if (ImGui::DragInt("GenerateFrame", &enemy.m_generateFrame) && enemy.m_generateFrame < 0)
		{
			enemy.m_generateFrame = 0;	//�}�C�i�X�h�~
		}

		//�����ʒu
		static float s_generatePos[3] = { enemy.m_pos.x,enemy.m_pos.y,enemy.m_pos.z };
		if (ImGui::DragFloat3("GeneratePos", s_generatePos), 0.05f)
		{
			enemy.m_pos = { s_generatePos[0],s_generatePos[1],s_generatePos[2] };
		}

		//�O�x�N�g��
		static float s_forwardVec[3] = { enemy.m_forwardVec.x,enemy.m_forwardVec.y,enemy.m_forwardVec.z };
		if (ImGui::DragFloat3("ForwardVec", s_forwardVec), 0.05f)
		{
			enemy.m_forwardVec = { s_forwardVec[0],s_forwardVec[1],s_forwardVec[2] };
		}

		//�G�l�~�[�폜
		if (ImGui::Button("RemoveEnemy"))
		{
			enemys.erase(enemys.begin() + m_enemyIdx);
		}
	}


	ImGui::End();
}
