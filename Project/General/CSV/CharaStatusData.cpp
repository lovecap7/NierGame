#include "CharaStatusData.h"

CharaStatusData::CharaStatusData(std::shared_ptr<CSVData> data) :
	m_hp(0),
	m_at(0),
	m_df(0),
	m_ms(0.0f),
	m_ar(CharaStatus::Armor::Light)
{
	//データを取得
	this->m_data = data->GetData();
	//変換
	Conversion();
}

CharaStatusData::~CharaStatusData()
{
}

void CharaStatusData::Conversion()
{
	if (m_data.size() <= 0)return;
	//名前はスキップ
	//1スタート
	//ID
	m_id = stoi(m_data[1]);
	//体力
	m_hp = stoi(m_data[2]);
	//攻撃力
	m_at = stoi(m_data[3]);
	//防御力
	m_df = stoi(m_data[4]);
	//移動速度
	m_ms = stof(m_data[5]);
	//ジャンプ力
	m_jp = stof(m_data[6]);
	//アーマー
	m_ar = static_cast<CharaStatus::Armor>(stoi(m_data[7]));
}