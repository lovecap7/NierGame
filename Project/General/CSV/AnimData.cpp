#include "AnimData.h"
#include "../StringUtil.h"

AnimData::AnimData():
	m_stateName(),
	m_weaponType(WeaponType::None),
	m_animName()
{
	
}

AnimData::AnimData(std::shared_ptr<CSVData> data):
	m_stateName(),
	m_weaponType(WeaponType::None),
	m_animName()
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

AnimData::~AnimData()
{
}

void AnimData::Conversion()
{
	if (m_data.size() <= 0)return;
	//状態
	m_stateName = m_data[0];

	//武器
	auto weaponType = m_data[1];
	if (weaponType == L"None")
	{
		m_weaponType = WeaponType::None;
	}
	else if (weaponType == L"LightSword")
	{
		m_weaponType = WeaponType::LightSword;
	}
	else if (weaponType == L"BigSword")
	{
		m_weaponType = WeaponType::BigSword;
	}

	//アニメーション
	m_animName = StringUtil::WstringToString(m_data[2]);
}
