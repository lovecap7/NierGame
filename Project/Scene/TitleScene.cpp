#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include "../General/Fader.h"
#include "../General/Game.h"

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	Fader::GetInstance().FadeIn();
}

void TitleScene::Update()
{
	auto& input = Input::GetInstance();

	auto& fader = Fader::GetInstance();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_unique<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial)));
		return;
	}

	if (input.IsTrigger("A") && !fader.IsFadeNow())
	{
		fader.FadeOut();
	}
}

void TitleScene::Draw()
{
	DrawCircle(Game::kScreenCenterX, Game::kScreenCenterY, 100, 0xff0000, true);
}

void TitleScene::End()
{

}

void TitleScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
