#include "TextData.h"

TextData::TextData():
	m_id(),
	m_text(),
	m_speaker(),
	m_nextID()
{
}

TextData::TextData(std::shared_ptr<CSVData> data):
	m_id(),
	m_text(),
	m_speaker(),
	m_nextID()
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

TextData::~TextData()
{
}

void TextData::Conversion()
{
	if (m_data.size() <= 0)return;

	//ID
	m_id = m_data[0];
	//テキスト
	m_text = m_data[1];
	//話してる人
	m_speaker = m_data[2];
	//次のID
	m_nextID = m_data[3];
	//グループ
	m_group = m_data[4];
	//画像パス
	m_imagePath = m_data[5];
}
