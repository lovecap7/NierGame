#pragma once
#include "CSVData.h"
#include "../Math/MyMath.h"
#include "../Collision/Collidable.h"
#include <string>
class PlayerAnimData :
	public CSVData
{
public:
	PlayerAnimData();
	PlayerAnimData(std::shared_ptr<CSVData> data);
	~PlayerAnimData();
	//武器
	enum class WeaponType
	{
		None,
		LightSword,
		BigSword,
	};
	//状態
	std::wstring m_stateName;
	//武器
	WeaponType m_weaponType;
	//アニメーション
	std::string m_animName;
private:
	//変換
	void Conversion() override;
};

