#include "TutorialManager.h"
#include "../Actor/Character/Player/Player.h"
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/TextData.h"
#include "../UI/TalkUI.h"
#include "../UI/UIManager.h"
#include "../../General/Input.h"
#include "../../General/CSV/TutorialData.h"
#include <map>

namespace
{
	//パス
	const std::wstring kTextDataPath = L"Tutorial/TextData";
	const std::wstring kTutorialDataPath = L"/ TutorialData";
}

TutorialManager::TutorialManager(std::weak_ptr<Player> pPlayer, std::wstring stageName):
	m_pPlayer(pPlayer),
	m_isClear(false)
{
	auto& csvLoader = CSVDataLoader::GetInstance();

	//チュートリアル情報
	/*auto path = stageName + kTutorialDataPath;
	auto tutorialDatas = csvLoader.LoadCSV(path.c_str());
	auto tutorialData = std::make_shared<TutorialData>(tutorialDatas.front());*/


	

	//テキストデータ
	auto datas = csvLoader.LoadCSV(kTextDataPath.c_str());
	std::list<std::shared_ptr<TextData>> allTextDatas;
	
	//すべてのテキストを読み込む
	for (auto& data : datas)
	{
		auto textData = std::make_shared<TextData>(data);
		allTextDatas.push_back(textData);
	}

	//UIマネージャー
	auto& uiManager = UIManager::GetInstance();

	//UI作成の準備
	std::list<std::shared_ptr<TextData>> uiTextDatas;

	//グループ
	for (auto& textData : allTextDatas)
	{
		//メニューとあるテキストを取得
		if (textData->GetGroup() == stageName)
		{
			uiTextDatas.push_back(textData);
		}
	}

	//UI作成
	auto talkUI = std::make_shared<TalkUI>(uiTextDatas);
	talkUI->Init();
}

TutorialManager::~TutorialManager()
{
}
void TutorialManager::Update()
{
	
}
