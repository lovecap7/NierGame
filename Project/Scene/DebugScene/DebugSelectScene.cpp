#include "DebugSelectScene.h"
#include "PlayerTestScene.h"
#include "../SceneController.h"
#include "../TitleScene.h"
#include "../GameScene.h"
#include "../ResultScene.h"
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
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Stage1)));
			break;
		case Menu::Stage2:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial)));
			break;
		case Menu::Stage3:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial)));
			break;
		case Menu::Result:
			m_controller.ChangeScene(std::make_shared<ResultScene>(GetStageNameByIndex(StageIndex::Tutorial),m_controller,std::make_shared<Timer>()));
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
		switch (static_cast<Menu>(i))
		{
		case Menu::Title:
			DrawString(100, 100 + i * 30, L"Title", color);
			break;
		case Menu::Tutorial:
			DrawString(100, 100 + i * 30, L"Tutorial", color);
			break;
		case Menu::Stage1:
			DrawString(100, 100 + i * 30, L"Stage1", color);
			break;
		case Menu::Stage2:
			DrawString(100, 100 + i * 30, L"Stage2", color);
			break;
		case Menu::Stage3:
			DrawString(100, 100 + i * 30, L"Stage3", color);
			break;
		case Menu::Result:
			DrawString(100, 100 + i * 30, L"Result", color);
			break;
		case Menu::PlayerTest:
			DrawString(100, 100 + i * 30, L"PlayerTestScene", color);
			break;
		default:
			break;
		}
	}
	DrawString(100, 50, L"[A]キーで決定", 0xffffff);
}

void DebugSelectScene::End()
{
}

void DebugSelectScene::DebugDraw() const
{
}
