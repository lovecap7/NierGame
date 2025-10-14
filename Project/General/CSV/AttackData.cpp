#include "AttackData.h"
#include "../StringUtil.h"

AttackData::AttackData():
	m_name(),
	m_attackPower(0),
	m_knockBackPower(0.0f),
	m_verticalPower(0.0f),
	m_startFrame(0),
	m_keepFrame(0),
	m_nextAttackName()
{

}

AttackData::AttackData(std::shared_ptr<CSVData> data):
	m_name(),
	m_attackPower(0),
	m_knockBackPower(0.0f),
	m_verticalPower(0.0f),
	m_startFrame(0),
	m_keepFrame(0),
	m_nextAttackName()
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

AttackData::~AttackData()
{
}

void AttackData::Conversion()
{
	if (m_data.size() <= 0)return;
	//攻撃の名前
	m_name = m_data[0];

	//威力
	m_attackPower = stoi(m_data[1]);

	//アーマー
	m_attackWeight = static_cast<CharaStatus::AttackWeight>(stoi(m_data[2]));

	//ノックバックの大きさ
	m_knockBackPower = stof(m_data[3]);

	//上方向の力
	m_verticalPower = stof(m_data[4]);

	//発生フレーム
	m_startFrame = stoi(m_data[5]);

	//持続フレーム
	m_keepFrame = stoi(m_data[6]);

	//半径
	m_radius = stof(m_data[7]);

	//長さ
	m_radius = stof(m_data[8]);

	//アニメーションの名前
	m_animName = m_data[9];

	//次の攻撃の名前
	m_nextAttackName = m_data[10];

	//キャンセルフレーム
	m_cancelFrame = stoi(m_data[11]);
}
