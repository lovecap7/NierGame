#include "EnemyAttackKeyData.h"

EnemyAttackKeyData::EnemyAttackKeyData():
	m_attackKeyName(L""),
	m_attackRangeType(AttackRangeType::Melee)
{
}

EnemyAttackKeyData::EnemyAttackKeyData(std::shared_ptr<CSVData> data):
	m_attackKeyName(L""),
	m_attackRangeType(AttackRangeType::Melee)
{
	//データを取得
	this->m_data = data->GetData();
	//変換
	Conversion();
}

EnemyAttackKeyData::~EnemyAttackKeyData()
{
}

void EnemyAttackKeyData::Conversion()
{
	if (m_data.size() <= 0)return;
	//名前
	if (m_data[0] == L"Melee")
	{
		m_attackRangeType = AttackRangeType::Melee;
	}
	else if (m_data[0] == L"LongRange")
	{
		m_attackRangeType = AttackRangeType::LongRange;
	}

	//攻撃のキー
	m_attackKeyName = m_data[1];
}
