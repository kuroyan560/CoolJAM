#include "EnemyWaveLoader.h"
#include"EnemyWave.h"
#include"KuroFunc.h"
#include "SlowMgr.h"

const std::string EnemyWaveLoader::s_dir = "resource/user/enemyWave/";
const std::string EnemyWaveLoader::s_extention = ".enwav";

void EnemyWaveLoader::Save(const std::string& FileName, const std::vector<std::shared_ptr<EnemyWave>>& Waves, const int& FinalWaveTime)
{
    FILE* fp;
    fopen_s(&fp, (s_dir + FileName + s_extention).c_str(), "wb");
    assert(fp != nullptr);

    //�Ō�̃E�F�[�u�̏I������
    assert(KuroFunc::SaveData(fp, &FinalWaveTime, sizeof(FinalWaveTime), 1));

    //�E�F�[�u��
    const int waveCount = static_cast<int>(Waves.size());
    assert(KuroFunc::SaveData(fp, &waveCount, sizeof(waveCount), 1));

    for (int i = 0; i < Waves.size(); ++i)
    {
        auto& w = *Waves[i];

        //�G�l�~�[�̐�
        const int enemyCount = static_cast<int>(w.m_enemys.size());
        assert(KuroFunc::SaveData(fp, &enemyCount, sizeof(enemyCount), 1));

        //�G�l�~�[�������
        assert(KuroFunc::SaveData(fp, w.m_enemys.data(), sizeof(ENEMY_INFO::SPAWN_INFO), enemyCount));

        //�E�F�[�u�̊J�n�t���[��
        assert(KuroFunc::SaveData(fp, &w.m_waveStartFrame, sizeof(w.m_waveStartFrame), 1));

        //�E�F�[�u���{�[�i�X�X�e�[�W��
        assert(KuroFunc::SaveData(fp, &w.m_isBounusStageFlag, sizeof(w.m_isBounusStageFlag), 1));
    }

    fclose(fp);
}

std::vector<std::shared_ptr<EnemyWave>> EnemyWaveLoader::Load(const std::string& FileName, int& FinalWaveTime)
{
    if (!KuroFunc::ExistFile(s_dir + FileName + s_extention))
    {
        printf("�w�肳�ꂽEnemyWave�t�@�C����������܂���B\n");
        return std::vector<std::shared_ptr<EnemyWave>>();
    }

    std::vector<std::shared_ptr<EnemyWave>>result;

    FILE* fp;
    fopen_s(&fp, (s_dir + FileName + s_extention).c_str(), "rb");
    if (fp == nullptr)assert(0);

    //�Ō�̃E�F�[�u�̏I������
    //if (!KuroFunc::LoadData(fp, &FinalWaveTime, sizeof(FinalWaveTime), 1))assert(0);

    //�E�F�[�u��
    int waveCount = 0;
    if (!KuroFunc::LoadData(fp, &waveCount, sizeof(waveCount), 1))assert(0);

    //�E�F�[�u����
    result.resize(waveCount);

    for (int i = 0; i < waveCount; ++i)
    {
        result[i] = std::make_shared<EnemyWave>(0, false);

        //�G�l�~�[�̐�
        int enemyCount = 0;
        if (!KuroFunc::LoadData(fp, &enemyCount, sizeof(enemyCount), 1))assert(0);

        //�G�l�~�[�������
        result[i]->m_enemys.resize(enemyCount);
        if (!KuroFunc::LoadData(fp, result[i]->m_enemys.data(), sizeof(ENEMY_INFO::SPAWN_INFO), enemyCount))assert(0);

        //�E�F�[�u�̊J�n�t���[��
        if (!KuroFunc::LoadData(fp, &result[i]->m_waveStartFrame, sizeof(result[i]->m_waveStartFrame), 1))assert(0);

        //�E�F�[�u���{�[�i�X�X�e�[�W��
        if (!KuroFunc::LoadData(fp, &result[i]->m_isBounusStageFlag, sizeof(result[i]->m_isBounusStageFlag), 1))assert(0);
    }

    fclose(fp);

    return result;
}
