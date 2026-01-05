#pragma once
#include "CSVData.h"
#include "../StageIndex.h"
#include <memory>
#include <string>
class HighScoreData :
    public CSVData
{
public:
	HighScoreData();
	HighScoreData(std::shared_ptr<CSVData> data);
	~HighScoreData();

	//ステージ番号
	StageIndex GetStageIndex()const { return m_stageIndex; };
	//タイム
	std::wstring GetTimeText()const { return m_timeText; };
	//ランク
	std::wstring m_rankText;
private:
	//変換
	void Conversion() override;
private:
	//ステージ番号
	StageIndex m_stageIndex;
	//タイム
	std::wstring m_timeText;
	//ランク
	std::wstring m_rankText;
};

