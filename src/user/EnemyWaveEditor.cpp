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

	//テスト中
	if (m_test)
	{
		ImGui::Text("Now Testing ...");

		if (WaveMgr.EndAllWaves())
		{
			ImGui::Text("All waves end. Please stop this test.");
		}
		ImGui::End();
		return;
	}

	//ウェーブ追加
	if (ImGui::Button("AddWave"))
	{
		WaveMgr.AddWave(std::make_shared<EnemyWave>(30, false));
		m_waveIdx++;
	}
	//ウェーブ削除
	ImGui::SameLine();
	if (ImGui::Button("RemoveWave"))
	{
		WaveMgr.m_waves.erase(WaveMgr.m_waves.begin() + m_waveIdx);
	}

	//ウェーブが空なら１つ追加して編集対象にする
	if (WaveMgr.m_waves.empty())
	{
		WaveMgr.m_waves.emplace_back(std::make_shared<EnemyWave>(1, false));
	}
	//ウェーブ選択
	ImGui::DragInt("NowEditWave", &m_waveIdx, 1, 0, WaveMgr.m_waves.size() - 1);

	ImGui::Separator();


	m_waveIdx = std::clamp(m_waveIdx, 0, static_cast<int>(WaveMgr.m_waves.size()) - 1);
	auto wave = WaveMgr.m_waves[m_waveIdx];

	if (ImGui::DragInt("StartFrame", &wave->m_waveStartFrame) && wave->m_waveStartFrame < 0)
	{
		wave->m_waveStartFrame = 0;	//マイナス防止
	}
	ImGui::Checkbox("IsBonusStage", &wave->m_isBounusStageFlag);

	ImGui::Separator();

	//エネミー編集
	auto& enemys = wave->m_enemys;
	//エネミー追加
	if (ImGui::Button("AddEnemy"))
	{
		wave->AddEnemy({ 0,0,0 }, { 0,0,0 }, ENEMY_INFO::ID::STOPPING, 1);
	}

	if (!enemys.empty())
	{
		const int enemyCount = static_cast<int>(enemys.size());

		//エネミーを生成順にソート
		std::sort(enemys.begin(), enemys.end(), [](ENEMY_INFO::SPAWN_INFO& a, ENEMY_INFO::SPAWN_INFO b) {
			return a.m_generateFrame < b.m_generateFrame;
			});

		//エネミー一覧
		if (ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar))
		{
			for (int i = 0; i < enemyCount; ++i)
			{
				const auto& info = enemys[i];
				std::string str = std::to_string(i) + " : " + std::to_string(info.m_generateFrame) + " - " + ENEMY_INFO::GetIDName(info.m_id);
				ImGui::RadioButton(str.c_str(), &m_enemyIdx, i);
			}

			ImGui::EndChild();
		}

		//インデックス範囲超え防止
		m_enemyIdx = std::clamp(m_enemyIdx, 0, enemyCount - 1);

		//現在選択中のエネミー
		auto& enemy = enemys[m_enemyIdx];

		//種別選択
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
		//生成フレーム
		if (ImGui::DragInt("GenerateFrame", &enemy.m_generateFrame) && enemy.m_generateFrame < 1)
		{
			enemy.m_generateFrame = 1;	//マイナス防止
		}

		//生成位置
		static float s_generatePos[3] = { enemy.m_pos.x,enemy.m_pos.y,enemy.m_pos.z };
		if (ImGui::DragFloat3("GeneratePos", s_generatePos), 0.05f)
		{
			enemy.m_pos = { s_generatePos[0],s_generatePos[1],s_generatePos[2] };
		}

		//前ベクトル
		static float s_forwardVec[3] = { enemy.m_forwardVec.x,enemy.m_forwardVec.y,enemy.m_forwardVec.z };
		if (ImGui::DragFloat3("ForwardVec", s_forwardVec), 0.05f)
		{
			enemy.m_forwardVec = { s_forwardVec[0],s_forwardVec[1],s_forwardVec[2] };
		}

		//エネミー削除
		if (ImGui::Button("RemoveEnemy"))
		{
			enemys.erase(enemys.begin() + m_enemyIdx);
		}
	}


	ImGui::End();
}
