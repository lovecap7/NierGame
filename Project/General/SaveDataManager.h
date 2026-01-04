#pragma once
#include <memory>
class OptionData;
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
	

	//オプションデータ
	std::shared_ptr<OptionData> GetOptionData()const { return m_optionData; };
private:
	//オプションデータ
	std::shared_ptr<OptionData> m_optionData;
};

