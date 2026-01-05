#include "HighScoreData.h"

namespace
{
	constexpr int kDataNum = 2;
}

HighScoreData::HighScoreData():
	m_timeText(L"--:--:--"),
	m_rankText(L"-")
{
}

HighScoreData::HighScoreData(std::shared_ptr<CSVData> data) :
	m_timeText(L"--:--:--"),
	m_rankText(L"-")
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

HighScoreData::~HighScoreData()
{
}

void HighScoreData::Conversion()
{
	//要素が足りないなら早期リターン
	if (m_data.size() != kDataNum)return;

	int index = std::stoi(m_data[0]);
	switch (index)
	{
	case 1:
		m_stageIndex = StageIndex::Stage1;
		break;
	case 2:
		m_stageIndex = StageIndex::Stage2;
		break;
	case 3:
		m_stageIndex = StageIndex::Stage3;
		break;
	default:
		m_stageIndex = StageIndex::Stage1;
		break;
	}

	//タイム
	m_timeText = m_data[1];
	//ランク
	m_rankText = m_data[2];
}
