#pragma once
#include<memory>
#include <vector>
#include<string>
class EnemyWave;
class EnemyWaveLoader
{
public:
	static const std::string s_dir;
	static const std::string s_extention;
	static void Save(const std::string& FileName, const std::vector<std::shared_ptr<EnemyWave>>& Waves, const int& FinalWaveTime);
	static std::vector<std::shared_ptr<EnemyWave>>Load(const std::string& FileName, int& FinalWaveTime);
};