#pragma once
#include "CSVData.h"
#include "../CharaStatus.h"
#include <memory>
class EnemyAttackKeyData :
	public CSVData
{
public:
	EnemyAttackKeyData();
	EnemyAttackKeyData(std::shared_ptr<CSVData> data);
	~EnemyAttackKeyData();
	
	//UŒ‚‚Ì‹——£
	enum class AttackRangeType
	{
		Melee,		//‹ßÚUŒ‚
		LongRange,	//‰“‹——£UŒ‚
	};
	AttackRangeType	m_attackRangeType;	//UŒ‚‚Ì‹——£
	//UŒ‚‚ÌƒL[–¼
	std::wstring	m_attackKeyName;	//UŒ‚‚ÌƒL[–¼
private:
	//•ÏŠ·
	void Conversion() override;
};

