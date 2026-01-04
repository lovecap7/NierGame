#include "SaveDataManager.h"
#include "CSV/CSVDataLoader.h"
#include "CSV/ClearSaveData.h"
#include "CSV/OptionData.h"
#include "../Main/Application.h"

namespace
{
	//パス
	const std::wstring kClearSaveDataPath = L"Save/ClearSaveData";
	const std::wstring kOptionSaveDataPath = L"Save/OptionSaveData";
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
}
