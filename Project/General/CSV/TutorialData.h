#pragma once
#include "CSVData.h"
#include <memory>
#include <string>
#include "../Math/MyMath.h"
class TutorialData :
	public CSVData
{
public:
	TutorialData();
	TutorialData(std::shared_ptr<CSVData> data);
	~TutorialData();
	
	//クリア条件
	enum class ClearRequirement : int
	{
		AllDeadEnemy = 0,
		Goal = 1,
		JustAvoid = 2
	};

	//クリア条件
	ClearRequirement GetClearRequirement()const { return m_clearRequirement; };
	//クリア条件テキスト
	std::wstring GetClearRequirementText()const { return m_clearRequirementText; };
private:
	//変換
	void Conversion() override;
private:
	//クリア条件
	ClearRequirement m_clearRequirement;
	//クリア条件テキスト
	std::wstring m_clearRequirementText;
};

