#pragma once
#include "CSVData.h"
#include "../Math/MyMath.h"
#include "../Collision/Collidable.h"
#include <string>
#include "../../General/CharaStatus.h"
class AttackData :
	public CSVData
{
public:
	AttackData();
	AttackData(std::shared_ptr<CSVData> data);
	~AttackData();

	//攻撃の名前
	std::wstring m_name;

	//威力
	int m_attackPower;

	//攻撃の重さ
	CharaStatus::AttackWeight m_attackWeight;

	//ノックバックの大きさ
	float m_knockBackPower;

	//上方向の力
	float m_verticalPower;

	//発生フレーム
	int m_startFrame;

	//持続フレーム
	int m_keepFrame;

	//半径
	float m_radius;

	//長さ
	float m_length;
	
	//アニメーションの名前
	std::wstring m_animName;

	//次の攻撃の名前
	std::wstring m_nextAttackName;

	//キャンセル
	int m_cancelFrame;

private:
	//変換
	void Conversion() override;
};

