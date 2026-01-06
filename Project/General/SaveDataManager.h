#pragma once
#include <memory>
#include <vector>
#include "StageIndex.h"
class OptionData;
class ClearSaveData;
class HighScoreData;
class SaveDataManager final
{
private:
	//シングルトンの準備
	SaveDataManager() = default;
	~SaveDataManager() = default;
	//コピー禁止
	SaveDataManager(const SaveDataManager&) = delete;
	SaveDataManager& operator = (const SaveDataManager&) = delete;
	//ムーブ禁止
	SaveDataManager(SaveDataManager&&) = delete;
	SaveDataManager& operator = (SaveDataManager&&) = delete;
public:
	//インスタンスを取得
	static SaveDataManager& GetInstance()
	{
		static SaveDataManager instance;
		return instance;
	}
	//セーブデータのロード
	void LoadSave();
	
	//クリアデータ
	std::shared_ptr<ClearSaveData> GetClearData()const { return m_clearData; };
	//オプションデータ
	std::shared_ptr<OptionData> GetOptionData()const { return m_optionData; };
	//スコアデータ
	std::shared_ptr<HighScoreData> GetHighScoreData(StageIndex index)const;
private:
	//クリアデータ
	std::shared_ptr<ClearSaveData> m_clearData;
	//オプションデータ
	std::shared_ptr<OptionData> m_optionData;
	//スコアデータ
	std::vector<std::shared_ptr<HighScoreData>> m_highScoreDatas;
};

