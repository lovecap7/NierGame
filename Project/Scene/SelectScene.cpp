#include "SelectScene.h"
#include "SceneController.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "TutorialScene.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/StringUtil.h"
#include "../General/Math/MyMath.h"
#include "../General/AssetManager.h"
#include "../General/ShaderPostProcess.h"
#include "../General/Timer.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/TalkUI.h"
#include "../Main/Application.h"
#include "../General/CSV/CSVDataLoader.h"
#include "../General/CSV/TextData.h"
#include "../General/Fader.h"


SelectScene::SelectScene(SceneController& controller) :
	SceneBase(controller),
	m_currentMainMenu(MainMenu::Tutorial),
	m_currentStageMenu(StageMenu::Stage1),
	m_currentTutorialMenu(TutorialMenu::Tutorial1),
	m_update(&SelectScene::UpdateMainMenu)
{
}

SelectScene::~SelectScene()
{
}

void SelectScene::Init()
{
	//アセットを削除
	AssetManager::GetInstance().DeleteModelHandle();
	//ポストエフェクトを解除
	Application::GetInstance().GetPostProcess()->ResetPostEffectState();
	//UI削除
	UIManager::GetInstance().AllDeleteUI();
	//フェードイン
	Fader::GetInstance().FadeIn();

	auto& csvLoader = CSVDataLoader::GetInstance();
	auto datas = csvLoader.LoadCSV(L"DebugScene/TextData");
	std::list<std::shared_ptr<TextData>> textDatas;
	for (auto& data : datas)
	{
		auto textData = std::make_shared<TextData>(data);
		textDatas.push_back(textData);
	}

	UIManager::GetInstance().Entry(std::make_shared<TalkUI>(textDatas));

	//一番上の項目から
	m_currentMainMenu = MainMenu::Tutorial;
	m_currentStageMenu = StageMenu::Stage1;
	m_currentTutorialMenu = TutorialMenu::Tutorial1;
}

void SelectScene::Update()
{
	auto& input = Input::GetInstance();
	
	auto& fader = Fader::GetInstance();

	//状態ごとの更新処理
	(this->*m_update)(input, fader);
}

void SelectScene::Draw()
{
	DrawString(100, 100, L"Tutorial Menu", GetColor(255, 255, 255));
	DrawString(100, 180, L"Stage Menu", GetColor(255, 255, 255));
	DrawString(100, 260, L"Title Menu", GetColor(255, 255, 255));
	switch (m_currentMainMenu)
	{
	case SelectScene::MainMenu::Tutorial:
		DrawString(100, 100, L"Tutorial Menu", GetColor(0, 255, 0));
		break;
	case SelectScene::MainMenu::Stage:
		DrawString(100, 180, L"Stage Menu", GetColor(0, 255, 0));
		break;
	case SelectScene::MainMenu::Title:
		DrawString(100, 260, L"Title Menu", GetColor(0, 255, 0));
		break;
	default:
		break;
	}

	DrawString(300, 120, L"1", GetColor(255, 255, 255));
	DrawString(300, 140, L"2", GetColor(255, 255, 255));
	DrawString(300, 160, L"3", GetColor(255, 255, 255));
	switch (m_currentTutorialMenu)
	{
	case SelectScene::TutorialMenu::Tutorial1:
		DrawString(300, 120, L"1", GetColor(255, 0, 0));
		break;
	case SelectScene::TutorialMenu::Tutorial2:
		DrawString(300, 140, L"2", GetColor(255, 0, 0));
		break;
	case SelectScene::TutorialMenu::Tutorial3:
		DrawString(300, 160, L"3", GetColor(255, 0, 0));
		break;
	default:
		break;
	}

	DrawString(300, 200, L"1", GetColor(255, 255, 255));
	DrawString(300, 220, L"2", GetColor(255, 255, 255));
	DrawString(300, 240, L"3", GetColor(255, 255, 255));
	switch (m_currentStageMenu)
	{
	case SelectScene::StageMenu::Stage1:
		DrawString(300, 200, L"1", GetColor(255, 0, 0));
		break;
	case SelectScene::StageMenu::Stage2:
		DrawString(300, 220, L"2", GetColor(255, 0, 0));
		break;
	case SelectScene::StageMenu::Stage3:
		DrawString(300, 240, L"3", GetColor(255, 0, 0));
		break;
	default:
		break;
	}


}

void SelectScene::End()
{
}

void SelectScene::DebugDraw() const
{
}

void SelectScene::UpdateMainMenu(Input& input, Fader& fader)
{
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}

	//フェード中は操作できない
	if (!fader.IsFadeNow())
	{
		if (input.IsTrigger("A"))
		{
			switch (m_currentMainMenu)
			{
			case SelectScene::MainMenu::Tutorial:
				//チュートリアルセレクトへ遷移
				m_update = &SelectScene::UpdateTutorialMenu;
				break;
			case SelectScene::MainMenu::Stage:
				//ステージセレクトへ遷移
				m_update = &SelectScene::UpdateStageMenu;
				break;
			case SelectScene::MainMenu::Title:
				//タイトルへ
				fader.FadeOut();
				break;
			default:
				break;
			}
			return;
		}

		int menu = static_cast<int>(m_currentMainMenu);
		if (input.IsRepeate("Up"))menu--;
		if (input.IsRepeate("Down"))menu++;

		//範囲外ならループ
		if (menu < static_cast<int>(MainMenu::Tutorial))
		{
			menu = static_cast<int>(MainMenu::Title);
		}
		if (menu > static_cast<int>(MainMenu::Title))
		{
			menu = static_cast<int>(MainMenu::Tutorial);
		}

		m_currentMainMenu = static_cast<MainMenu>(menu);
	}
}

void SelectScene::UpdateTutorialMenu(Input& input, Fader& fader)
{
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		switch (m_currentTutorialMenu)
		{
		case SelectScene::TutorialMenu::Tutorial1:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial1)));
			break;
		case SelectScene::TutorialMenu::Tutorial2:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial2)));
			break;
		case SelectScene::TutorialMenu::Tutorial3:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial3)));
			break;
		default:
			break;
		}
		return;
	}

	//フェード中は操作できない
	if (!fader.IsFadeNow())
	{
		if(input.IsTrigger("A"))
		{
			fader.FadeOut();
			return;
		}
		if(input.IsTrigger("B"))
		{
			//メインメニューへ戻る
			m_update = &SelectScene::UpdateMainMenu;
			return;
		}

		int menu = static_cast<int>(m_currentTutorialMenu);
		if (input.IsRepeate("Up"))menu--;
		if (input.IsRepeate("Down"))menu++;

		//範囲外ならループ
		if (menu < static_cast<int>(TutorialMenu::Tutorial1))
		{
			menu = static_cast<int>(TutorialMenu::Tutorial3);
		}
		if (menu > static_cast<int>(TutorialMenu::Tutorial3))
		{
			menu = static_cast<int>(TutorialMenu::Tutorial1);
		}

		m_currentTutorialMenu = static_cast<TutorialMenu>(menu);
	}
}

void SelectScene::UpdateStageMenu(Input& input, Fader& fader)
{
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		switch (m_currentStageMenu)
		{
		case SelectScene::StageMenu::Stage1:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller,GetStageNameByIndex(StageIndex::Stage1)));
			break;
		case SelectScene::StageMenu::Stage2:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller,GetStageNameByIndex(StageIndex::Stage2)));
			break;
		case SelectScene::StageMenu::Stage3:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller,GetStageNameByIndex(StageIndex::Stage3)));
			break;
		default:
			break;
		}
		return;
	}

	//フェード中は操作できない
	if (!fader.IsFadeNow())
	{
		if (input.IsTrigger("A"))
		{
			fader.FadeOut();
			return;
		}
		if (input.IsTrigger("B"))
		{
			//メインメニューへ戻る
			m_update = &SelectScene::UpdateMainMenu;
			return;
		}

		int menu = static_cast<int>(m_currentStageMenu);
		if (input.IsRepeate("Up"))menu--;
		if (input.IsRepeate("Down"))menu++;

		//範囲外ならループ
		if (menu < static_cast<int>(StageMenu::Stage1))
		{
			menu = static_cast<int>(StageMenu::Stage3);
		}
		if (menu > static_cast<int>(StageMenu::Stage3))
		{
			menu = static_cast<int>(StageMenu::Stage1);
		}

		m_currentStageMenu = static_cast<StageMenu>(menu);
	}
}
