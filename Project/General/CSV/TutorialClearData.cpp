#include "TutorialClearData.h"

TutorialClearData::TutorialClearData():
	m_clearRequirement(ClearRequirement::AllDeadEnemy)
{
}

TutorialClearData::TutorialClearData(std::shared_ptr<CSVData> data):
	m_clearRequirement(ClearRequirement::AllDeadEnemy)
{
	//データを取得
	this->m_data = data->GetData();
	Conversion();
}

TutorialClearData::~TutorialClearData()
{
}

void TutorialClearData::Conversion()
{
	if (m_data.size() <= 0)return;
	//チュートリアル名
	m_tutorialName = m_data[0];
	//クリア条件
	m_clearRequirement = static_cast<ClearRequirement>(stoi(m_data[1]));
	//クリア条件テキスト
	m_clearRequirementText = m_data[2];
}
