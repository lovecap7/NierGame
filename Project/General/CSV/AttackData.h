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

	//多段ヒットか
	bool m_isMultipleHit;

	enum class AttackType : int
	{
		None = 0,
		Melee = 1,
		Bullet = 2,
		Throw = 3
	};
	//攻撃タイプ
	AttackType m_attackType;

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

	//前進速度
	float m_moveSpeed;

	//前進フレーム
	int m_moveFrame;
	
	//アニメーションの名前
	std::wstring m_animName;

	//次の攻撃の名前
	std::wstring m_nextAttackName;

	//キャンセル
	int m_cancelFrame;

	//パラメータ
	float m_param1;
	float m_param2;
	float m_param3;

private:
	//変換
	void Conversion() override;
};

