#include "ResultRankData.h"

ResultRankData::ResultRankData() :
	m_rank(),
	m_min(0),
	m_sec(0)
{
}

ResultRankData::ResultRankData(std::shared_ptr<CSVData> data) :
	m_rank(),
	m_min(0),
	m_sec(0)
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

ResultRankData::~ResultRankData()
{
}

void ResultRankData::Conversion()
{
	if (m_data.size() <= 0)return;

	//ランク
	m_rank = m_data[0];

	//分と秒
	m_min = stoi(m_data[1]);
	m_sec = stoi(m_data[2]);
}
