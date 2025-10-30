#include "DebugSelectScene.h"
#include "PlayerTestScene.h"
#include "../SceneController.h"
#include "../TitleScene.h"
#include "../GameScene.h"
#include "../../General/Input.h"
#include <DxLib.h>
#include "../../General/StringUtil.h"
#include "../../General/Math/MyMath.h"


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
		case Menu::Game:
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Stage1)));
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
	menu = MathSub::ClampInt(menu, 0, 2);
	m_currentMenu = static_cast<Menu>(menu);
}

void DebugSelectScene::Draw()
{
	for (int i = 0;i < 3;i++)
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
		case Menu::Game:
			DrawString(100, 100 + i * 30, L"Game", color);
			break;
		case Menu::PlayerTest:
			DrawString(100, 100 + i * 30, L"PlayerTestScene", color);
			break;
		default:
			break;
		}
	}
	DrawString(100, 50, L"[A]ÉLÅ[Ç≈åàíË", 0xffffff);
}

void DebugSelectScene::End()
{
}

void DebugSelectScene::DebugDraw() const
{
}
