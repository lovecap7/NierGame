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
	m_currentMenu(Menu::Tutorial1)
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
	m_currentMenu = Menu::Tutorial1;
}

void SelectScene::Update()
{
	auto& input = Input::GetInstance();
	
	auto& fader = Fader::GetInstance();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		switch (m_currentMenu)
		{
		case Menu::Tutorial1:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial1)));
			break;
		case Menu::Tutorial2:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial2)));
			break;
		case Menu::Tutorial3:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial3)));
			break;
		case Menu::Stage1:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial)));
			break;
		case Menu::Stage2:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Stage2)));
			break;
		case Menu::Stage3:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Stage3)));
			break;
		case Menu::Title:
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
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
		}
		int menu = static_cast<int>(m_currentMenu);
		if (input.IsRepeate("Up"))menu--;
		if (input.IsRepeate("Down"))menu++;
		menu = MathSub::ClampInt(menu, 0, static_cast<int>(Menu::Title));
		m_currentMenu = static_cast<Menu>(menu);
	}
}

void SelectScene::Draw()
{
	for (int i = 0; i <= static_cast<int>(Menu::Title); i++)
	{
		int color = 0xffffff;
		if (m_currentMenu == static_cast<Menu>(i))
		{
			color = 0xffff00;
		}
		std::wstring text = L"Title";
		switch (static_cast<Menu>(i))
		{
		case Menu::Tutorial1:
			text = L"Tutorial1";
			break;
		case Menu::Tutorial2:
			text = L"Tutorial2";
			break;
		case Menu::Tutorial3:
			text = L"Tutorial3";
			break;
		case Menu::Stage1:
			text = L"Stage1";
			break;
		case Menu::Stage2:
			text = L"Stage2";
			break;
		case Menu::Stage3:
			text = L"Stage3";
			break;
		case Menu::Title:
			text = L"Title";
			break;
		default:
			break;
		}
		DxLib::DrawString(100, 100 + i * 24, text.c_str(), color);
	}
	DxLib::DrawString(100, 50, L"[A]キーで決定", 0xffffff);
}

void SelectScene::End()
{
}

void SelectScene::DebugDraw() const
{
}
