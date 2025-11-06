#include "TutorialManager.h"
#include "../Actor/Character/Player/Player.h"
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/TextData.h"
#include "../UI/TalkUI.h"
#include "../UI/UIManager.h"
#include "../../General/Input.h"
#include <map>

namespace
{
	//パス
	const std::wstring kTextDataPath = L"Tutorial/TextData";
}

TutorialManager::TutorialManager(std::weak_ptr<Player> pPlayer):
	m_pPlayer(pPlayer),
	m_update(&TutorialManager::UpdateMoveStep)
{
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto datas = csvLoader.LoadCSV(kTextDataPath.c_str());
	//テキストデータ
	std::list<std::shared_ptr<TextData>> textDatas;
	//グループ分けでそのグループをすでに作ったかを確認するための変数
	std::map<std::wstring, bool> groupMake;

	//すべてのテキストを読み込む
	for (auto& data : datas)
	{
		auto textData = std::make_shared<TextData>(data);
		textDatas.push_back(textData);
		//グループ名登録
		groupMake[textData->GetGroup()] = false;
	}

	//UIマネージャー
	auto& uiManager = UIManager::GetInstance();

	//グループ分け
	std::list<std::shared_ptr<TextData>> groupTextDatas;
	for (auto& textData : textDatas)
	{
		//まだ作っていないグループなら
		if (!groupMake[textData->GetGroup()])
		{
			for (auto& textData2 : textDatas)
			{
				//同じグループなら
				if (textData->GetGroup() == textData2->GetGroup())
				{
					//保持
					groupTextDatas.push_back(textData2);
				}
			}
			//グループ分けが終わったら
			groupMake[textData->GetGroup()] = true;

			//UIの作成
			auto ui = std::make_shared<TalkUI>(groupTextDatas);

			//描画非表示
			ui->DisableDraw();

			//登録
			uiManager.Entry(ui);

			//参照
			m_talkUIs.push_back(ui);

			//解放
			groupTextDatas.clear();
		}
	}

	if (m_talkUIs.size() < 5)return;

	if (m_talkUIs.front().expired())return;
	//最初のチュートリアル表示
	m_talkUIs.front().lock()->EnableDraw();
}

TutorialManager::~TutorialManager()
{
}
void TutorialManager::Update()
{
	//もしもすべてのチュートリアルが終了したら更新しない
	if (m_talkUIs.empty())return;

	if (m_pPlayer.expired())return;
	auto player = m_pPlayer.lock();
	//状態ごとの更新処理
	(this->*m_update)(player);
}

void TutorialManager::UpdateMoveStep(std::shared_ptr<Player> player)
{
	//なくなったら
	if (m_talkUIs.front().expired())
	{
		//プレイヤーを動けるように
		player->Operate();

		//目標達成
		if (Input::GetInstance().IsTrigger("RB"))
		{
			//解放
			m_talkUIs.pop_front();

			//次のチュートリアル表示
			if (m_talkUIs.front().expired())return;
			m_talkUIs.front().lock()->EnableDraw();

			m_update = &TutorialManager::UpdateJumpStep;
			return;
		}
	}
	else
	{
		//待機
		player->Wait();
	}
}

void TutorialManager::UpdateJumpStep(std::shared_ptr<Player> player)
{
	//なくなったら
	if (m_talkUIs.front().expired())
	{
		//プレイヤーを動けるように
		player->Operate();

		//目標地点へ移動したら
		if (Input::GetInstance().IsTrigger("RB"))
		{
			//解放
			m_talkUIs.pop_front();

			//次のチュートリアル表示
			if (m_talkUIs.front().expired())return;
			m_talkUIs.front().lock()->EnableDraw();

			m_update = &TutorialManager::UpdateAttackStep;
			return;
		}
	}
	else
	{
		//待機
		player->Wait();
	}
}

void TutorialManager::UpdateAttackStep(std::shared_ptr<Player> player)
{
	//なくなったら
	if (m_talkUIs.front().expired())
	{
		//プレイヤーを動けるように
		player->Operate();

		//目標地点へ移動したら
		if (Input::GetInstance().IsTrigger("RB"))
		{
			//解放
			m_talkUIs.pop_front();

			//次のチュートリアル表示
			if (m_talkUIs.front().expired())return;
			m_talkUIs.front().lock()->EnableDraw();

			m_update = &TutorialManager::UpdateJustStep;
			return;
		}
	}
	else
	{
		//待機
		player->Wait();
	}
}

void TutorialManager::UpdateJustStep(std::shared_ptr<Player> player)
{
	//なくなったら
	if (m_talkUIs.front().expired())
	{
		//プレイヤーを動けるように
		player->Operate();

		//目標地点へ移動したら
		if (Input::GetInstance().IsTrigger("RB"))
		{
			//解放
			m_talkUIs.pop_front();

			//次のチュートリアル表示
			if (m_talkUIs.front().expired())return;
			m_talkUIs.front().lock()->EnableDraw();

			m_update = &TutorialManager::UpdateBattleStep;
			return;
		}
	}
	else
	{
		//待機
		player->Wait();
	}
}

void TutorialManager::UpdateBattleStep(std::shared_ptr<Player> player)
{
	//なくなったら
	if (m_talkUIs.front().expired())
	{
		//プレイヤーを動けるように
		player->Operate();

		//解放
		m_talkUIs.pop_front();
	}
	else
	{
		//待機
		player->Wait();
	}
}
