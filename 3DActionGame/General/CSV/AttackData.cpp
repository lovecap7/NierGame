#include "AttackData.h"
#include "../StringUtil.h"

AttackData::AttackData():
	m_name(),
	m_isMultipleHit(false),
	m_attackType(AttackData::AttackType::None),
	m_attackWeight(CharaStatus::AttackWeight::Light),
	m_cancelFrame(0),
	m_attackPower(0),
	m_knockBackPower(0.0f),
	m_verticalPower(0.0f),
	m_startFrame(0),
	m_keepFrame(0),
	m_nextAttackName(),
	m_radius(0.0f),
	m_length(0.0f),
	m_param1(0.0f),
	m_param2(0.0f),
	m_param3(0.0f),
	m_moveSpeed(0.0f),
	m_moveFrame(0),
	m_attackOriginPos(AttackOriginPos::Owner)
{

}

AttackData::AttackData(std::shared_ptr<CSVData> data):
	m_name(),
	m_isMultipleHit(false),
	m_attackType(AttackData::AttackType::None),
	m_attackWeight(CharaStatus::AttackWeight::Light),
	m_cancelFrame(0),
	m_attackPower(0),
	m_knockBackPower(0.0f),
	m_verticalPower(0.0f),
	m_startFrame(0),
	m_keepFrame(0),
	m_nextAttackName(),
	m_radius(0.0f),
	m_length(0.0f),
	m_param1(0.0f),
	m_param2(0.0f),
	m_param3(0.0f),
	m_attackOriginPos(AttackOriginPos::Owner)
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

	//ヒット番号
	m_isMultipleHit = m_data[1] == L"TRUE";

	//攻撃タイプ
	m_attackType = static_cast<AttackType>(stoi(m_data[2]));

	//威力
	m_attackPower = stoi(m_data[3]);

	//攻撃の重さ
	m_attackWeight = static_cast<CharaStatus::AttackWeight>(stoi(m_data[4]));

	//アーマー
	m_armor = static_cast<CharaStatus::Armor>(stoi(m_data[5]));

	//ノックバックの大きさ
	m_knockBackPower = stof(m_data[6]);

	//上方向の力
	m_verticalPower = stof(m_data[7]);

	//発生フレーム
	m_startFrame = stoi(m_data[8]);

	//持続フレーム
	m_keepFrame = stoi(m_data[9]);

	//半径
	m_radius = stof(m_data[10]);

	//長さ
	m_length = stof(m_data[11]);

	//前進速度
	m_moveSpeed = stof(m_data[12]);

	//前進フレーム
	m_moveFrame = stoi(m_data[13]);

	//アニメーションの名前
	m_animName = m_data[14];

	//次の攻撃の名前
	m_nextAttackName = m_data[15];

	//攻撃の発生位置
	AttackOriginPos oriPos = AttackOriginPos::Owner;
	if (m_data[16] == L"Owner")oriPos = AttackOriginPos::Owner;
	else if (m_data[16] == L"Center")oriPos = AttackOriginPos::Center;

	m_attackOriginPos = oriPos;

	//キャンセルフレーム
	m_cancelFrame = stoi(m_data[17]);

	//パラメータ
	m_param1 = stof(m_data[18]);
	m_param2 = stof(m_data[19]);
	m_param3 = stof(m_data[20]);
}
