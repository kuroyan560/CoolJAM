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

    //最後のウェーブの終了時間
    assert(KuroFunc::SaveData(fp, &FinalWaveTime, sizeof(FinalWaveTime), 1));

    //ウェーブ数
    const int waveCount = static_cast<int>(Waves.size());
    assert(KuroFunc::SaveData(fp, &waveCount, sizeof(waveCount), 1));

    for (int i = 0; i < Waves.size(); ++i)
    {
        auto& w = *Waves[i];

        //エネミーの数
        const int enemyCount = static_cast<int>(w.m_enemys.size());
        assert(KuroFunc::SaveData(fp, &enemyCount, sizeof(enemyCount), 1));

        //エネミー生成情報
        assert(KuroFunc::SaveData(fp, w.m_enemys.data(), sizeof(ENEMY_INFO::SPAWN_INFO), enemyCount));

        //ウェーブの開始フレーム
        assert(KuroFunc::SaveData(fp, &w.m_waveStartFrame, sizeof(w.m_waveStartFrame), 1));

        //ウェーブがボーナスステージか
        assert(KuroFunc::SaveData(fp, &w.m_isBounusStageFlag, sizeof(w.m_isBounusStageFlag), 1));
    }

    fclose(fp);
}

std::vector<std::shared_ptr<EnemyWave>> EnemyWaveLoader::Load(const std::string& FileName, int& FinalWaveTime)
{
    if (!KuroFunc::ExistFile(s_dir + FileName + s_extention))
    {
        printf("指定されたEnemyWaveファイルが見つかりません。\n");
        return std::vector<std::shared_ptr<EnemyWave>>();
    }

    std::vector<std::shared_ptr<EnemyWave>>result;

    FILE* fp;
    fopen_s(&fp, (s_dir + FileName + s_extention).c_str(), "rb");
    assert(fp != nullptr);

    //最後のウェーブの終了時間
    assert(KuroFunc::LoadData(fp, &FinalWaveTime, sizeof(FinalWaveTime), 1));

    //ウェーブ数
    int waveCount = 0;
    assert(KuroFunc::LoadData(fp, &waveCount, sizeof(waveCount), 1));

    //ウェーブ生成
    result.resize(waveCount);

    for (int i = 0; i < waveCount; ++i)
    {
        result[i] = std::make_shared<EnemyWave>(0, false);

        //エネミーの数
        int enemyCount = 0;
        assert(KuroFunc::LoadData(fp, &enemyCount, sizeof(enemyCount), 1));

        //エネミー生成情報
        result[i]->m_enemys.resize(enemyCount);
        assert(KuroFunc::LoadData(fp, result[i]->m_enemys.data(), sizeof(ENEMY_INFO::SPAWN_INFO), enemyCount));

        //ウェーブの開始フレーム
        assert(KuroFunc::LoadData(fp, &result[i]->m_waveStartFrame, sizeof(result[i]->m_waveStartFrame), 1));

        //ウェーブがボーナスステージか
        assert(KuroFunc::LoadData(fp, &result[i]->m_isBounusStageFlag, sizeof(result[i]->m_isBounusStageFlag), 1));
    }

    fclose(fp);

    return result;
}
