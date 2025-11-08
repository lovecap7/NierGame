#include "TutorialResultScene.h"
#include "SceneController.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "DebugScene/DebugSelectScene.h"
#include <DxLib.h>
#include "../General/Fader.h"
#include "../General/Input.h"
#include "../General/Game.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/Result/TutorialResultUI.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Camera/CameraController.h"

TutorialResultScene::TutorialResultScene(SceneController& controller, StageIndex index):
	SceneBase(controller),
	m_tutorialIndex(index),
	m_menuIndex(Menu::Next)
{

}

TutorialResultScene::~TutorialResultScene()
{
}

void TutorialResultScene::Init()
{
	//UI削除
	UIManager::GetInstance().AllDeleteUI();
	//Phisicsを停止
	Physics::GetInstance().StopUpdate();
	auto ui = std::make_shared<TutorialResultUI>(m_tutorialIndex);
	ui->Init();
	m_mcUI = ui;
}

void TutorialResultScene::Update()
{
	auto& input = Input::GetInstance();
	auto& fader = Fader::GetInstance();
	CameraController::GetInstance().Update();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		if (m_tutorialIndex != StageIndex::Tutorial3)
		{
			int index = static_cast<int>(m_tutorialIndex) + 1;
			m_tutorialIndex = static_cast<StageIndex>(index);
		}
		else
		{
			m_menuIndex = Menu::Select;
		}

		switch (m_menuIndex)
		{
		case TutorialResultScene::Menu::Next:
			m_controller.ChangeBaseScene(std::make_unique<TutorialScene>(m_controller,GetStageNameByIndex(m_tutorialIndex)));
			break;
		case TutorialResultScene::Menu::Select:
			m_controller.ChangeBaseScene(std::make_unique<DebugSelectScene>(m_controller));
			break;
		default:
			m_controller.ChangeBaseScene(std::make_unique<DebugSelectScene>(m_controller));
			break;
		}
		m_controller.PopScene();
		return;
	}
	//ボタンを押したら次
	if (m_mcUI.expired())return;
	auto ui = m_mcUI.lock();
	if (!fader.IsFadeNow() && ui->IsFinish())
	{
		if (input.IsTrigger("A"))
		{
			fader.FadeOut();
		}
		//カーソル
		int index = static_cast<int>(m_menuIndex);
		if (input.IsTrigger("Up"))--index;
		if (input.IsTrigger("Down"))++index;
		index = MathSub::ClampInt(index, static_cast<int>(Menu::Next), static_cast<int>(Menu::Select));
		m_menuIndex = static_cast<Menu>(index);
		ui->SetMenuIndex(index);
	}

}

void TutorialResultScene::Draw()
{
}

void TutorialResultScene::End()
{
	//Phisicsを開始
	Physics::GetInstance().StartUpdate();
}

void TutorialResultScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"TutorialResult Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
