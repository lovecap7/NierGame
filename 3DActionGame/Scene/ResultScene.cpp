#include "ResultScene.h"
#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include  "../General/Fader.h"
#include  "../General/Game.h"
#include "SceneController.h"
#include "GameScene.h"

ResultScene::ResultScene(SceneController& controller) :
	SceneBase(controller)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
	Fader::GetInstance().FadeIn();
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
	DrawCircle(Game::kScreenCenterX, Game::kScreenCenterY, 100, 0x00ff00, true);
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
