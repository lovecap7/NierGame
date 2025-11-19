#include "DebugSelectScene.h"
#include "PlayerTestScene.h"
#include "../SceneController.h"
#include "../TitleScene.h"
#include "../GameScene.h"
#include "../ResultScene.h"
#include "../TutorialScene.h"
#include "../../General/Input.h"
#include <DxLib.h>
#include "../../General/StringUtil.h"
#include "../../General/Math/MyMath.h"
#include "../../General/AssetManager.h"
#include "../../General/ShaderPostProcess.h"
#include "../../General/Timer.h"
#include "../../Game/UI/UIManager.h"
#include "../../Game/UI/TalkUI.h"
#include "../../Main/Application.h"
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/TextData.h"
#include "../../General/Fader.h"


DebugSelectScene::DebugSelectScene(SceneController& controller) :
	SceneBase(controller),
	m_currentMenu(Menu::Title)
{
}

DebugSelectScene::~DebugSelectScene()
{
}

void DebugSelectScene::Init()
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
	auto datas = csvLoader.LoadCSV(L"Tutorial/TextData");
	std::list<std::shared_ptr<TextData>> textDatas;
	for (auto& data : datas)
	{
		auto textData = std::make_shared<TextData>(data);
		textDatas.push_back(textData);
	}

	UIManager::GetInstance().Entry(std::make_shared<TalkUI>(textDatas));


	m_currentMenu = Menu::Title;
}

void DebugSelectScene::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTrigger("A"))
	{
		switch (m_currentMenu)
		{
		case Menu::Title:
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			break;
		case Menu::Tutorial:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial)));
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
		case Menu::Result:
			m_controller.ChangeScene(std::make_shared<ResultScene>(GetStageNameByIndex(StageIndex::Tutorial),m_controller,std::make_shared<Timer>()));
			break;
		case Menu::Tutorial1:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial1)));
			break;
		case Menu::Tutorial2:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial2)));
			break;
		case Menu::Tutorial3:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial3)));
			break;
		case Menu::Tutorial4:
			m_controller.ChangeScene(std::make_shared<TutorialScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial4)));
			break;
		case Menu::PlayerTest:
			m_controller.ChangeScene(std::make_shared<PlayerTestScene>(m_controller));
			break;
		default:
			break;
		}
		return;
	}
	int menu = static_cast<int>(m_currentMenu);
	if (input.IsTrigger("Up"))menu--;
	if (input.IsTrigger("Down"))menu++;
	menu = MathSub::ClampInt(menu, 0, static_cast<int>(Menu::PlayerTest));
	m_currentMenu = static_cast<Menu>(menu);
}

void DebugSelectScene::Draw()
{
	for (int i = 0;i <= static_cast<int>(Menu::PlayerTest);i++)
	{
		int color = 0xffffff;
		if (m_currentMenu == static_cast<Menu>(i))
		{
			color = 0xffff00;
		}
		std::wstring text = L"Title";
		switch (static_cast<Menu>(i))
		{
		case Menu::Title:
			text = L"Title";
			break;
		case Menu::Tutorial:
			text = L"Tutorial";
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
		case Menu::Result:
			text = L"Result";
			break;
		case Menu::Tutorial1:
			text = L"Tutorial1";
			break;
		case Menu::Tutorial2:
			text = L"Tutorial2";
			break;
		case Menu::Tutorial3:
			text = L"Tutorial3";
			break;
		case Menu::PlayerTest:
			text = L"PlayerTest";
			break;
		default:
			break;
		}
		DxLib::DrawString(100, 100 + i * 24, text.c_str(), color);
	}
	DxLib::DrawString(100, 50, L"[A]キーで決定", 0xffffff);
}

void DebugSelectScene::End()
{
}

void DebugSelectScene::DebugDraw() const
{
}
