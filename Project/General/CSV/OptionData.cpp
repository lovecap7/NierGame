#include "OptionData.h"

OptionData::OptionData():
	m_bgmLv(0),
	m_seLv(0),
	m_voiceLv(0),
	m_isWindow(false)
{
}

OptionData::OptionData(std::shared_ptr<CSVData> data):
	m_bgmLv(0),
	m_seLv(0),
	m_voiceLv(0),
	m_isWindow(false)
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

OptionData::~OptionData()
{
}

void OptionData::Conversion()
{
	//音量
	m_bgmLv = std::stoi(m_data[0]);
	m_seLv = std::stoi(m_data[1]);
	m_voiceLv = std::stoi(m_data[2]);
	//スクリーンモード
	m_isWindow = static_cast<bool>(std::stoi(m_data[3]));
}
