#include "PlayerAnimData.h"
#include "../StringUtil.h"

PlayerAnimData::PlayerAnimData():
	m_stateName(),
	m_weaponType(WeaponType::None),
	m_animName()
{
	
}

PlayerAnimData::PlayerAnimData(std::shared_ptr<CSVData> data):
	m_stateName(),
	m_weaponType(WeaponType::None),
	m_animName()
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

PlayerAnimData::~PlayerAnimData()
{
}

void PlayerAnimData::Conversion()
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
