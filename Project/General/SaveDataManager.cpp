#include "SaveDataManager.h"
#include "CSV/CSVDataLoader.h"
#include "CSV/ClearSaveData.h"
#include "CSV/OptionData.h"
#include "CSV/HighScoreData.h"
#include "../Main/Application.h"

namespace
{
	//パス
	const std::wstring kClearSaveDataPath = L"Save/ClearSaveData";
	const std::wstring kOptionSaveDataPath = L"Save/OptionSaveData";
	const std::wstring kHighScoreSaveDataPath = L"Save/HighScoreSaveData";
}

void SaveDataManager::LoadSave()
{
	auto& csvLoader = CSVDataLoader::GetInstance();

	//クリアデータのロード
	auto clearData = csvLoader.LoadCSV(kClearSaveDataPath.c_str()).front();
	m_clearData = std::make_shared<ClearSaveData>(clearData);

	//オプションデータのロード
	auto optionData = csvLoader.LoadCSV(kOptionSaveDataPath.c_str()).front();
	m_optionData = std::make_shared<OptionData>(optionData);

	//スコアデータのロード
	for (auto& data : csvLoader.LoadCSV(kHighScoreSaveDataPath.c_str()))
	{
		m_highScoreDatas.emplace_back(std::make_shared<HighScoreData>(data));
	}
}

std::shared_ptr<HighScoreData> SaveDataManager::GetHighScoreData(StageIndex index) const
{
	//一致したものを返す
	for (auto& data : m_highScoreDatas)
	{
		if (data->GetStageIndex() == index)
		{
			return data;
		}
	}

	return m_highScoreDatas.front();
}
