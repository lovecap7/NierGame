#include "ResultScene.h"
#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include  "../General/Timer.h"
#include  "../General/Fader.h"
#include  "../General/Game.h"
#include "SceneController.h"
#include "GameScene.h"
#include "../Game/UI/Result/ResultUI.h"
#include "../Game/UI/UIManager.h"

ResultScene::ResultScene(std::wstring stageName, SceneController& controller, std::shared_ptr<Timer> timer) :
	SceneBase(controller),
	m_resultUI(),
	m_timer(timer),
	m_stageName(stageName)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
	//フェードイン
	Fader::GetInstance().FadeIn();
	
	//UIの削除
	UIManager::GetInstance().AllDeleteUI();

	//リザルトUI
	auto resultUI = std::make_shared<ResultUI>(m_stageName,m_timer);
	resultUI->Init();
	m_resultUI = resultUI;
}

void ResultScene::Update()
{
	auto& input = Input::GetInstance();
	auto& fader = Fader::GetInstance();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_unique<TitleScene>(m_controller));
		return;
	}
	//ボタンを押したら次
	if (input.IsTrigger("A") && !fader.IsFadeNow())
	{
		fader.FadeOut();
	}
}

void ResultScene::Draw()
{
}

void ResultScene::End()
{
}

void ResultScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Result Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
