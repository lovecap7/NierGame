#include "TutorialData.h"

TutorialData::TutorialData():
	m_clearRequirement(ClearRequirement::AllDeadEnemy)
{
}

TutorialData::TutorialData(std::shared_ptr<CSVData> data):
	m_clearRequirement(ClearRequirement::AllDeadEnemy)
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

TutorialData::~TutorialData()
{
}

void TutorialData::Conversion()
{
	if (m_data.size() <= 0)return;

	//クリア条件
	m_clearRequirement = static_cast<ClearRequirement>(stoi(m_data[1]));
	//クリア条件テキスト
	m_clearRequirementText = m_data[2];
}
