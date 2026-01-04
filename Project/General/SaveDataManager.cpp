#include "SaveDataManager.h"
#include "CSV/CSVDataLoader.h"
#include "CSV/OptionData.h"
#include "../Main/Application.h"

namespace
{
	const std::wstring kOptionSaveDataPath = L"Save/OptionSaveData";
}

void SaveDataManager::LoadSave()
{
	auto& csvLoader = CSVDataLoader::GetInstance();

	//オプションデータのロード
	auto data = csvLoader.LoadCSV(kOptionSaveDataPath.c_str()).front();
	m_optionData = std::make_shared<OptionData>(data);
}
