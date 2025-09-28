#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "GameScene.h"


TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	
}

void TitleScene::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTrigger("A"))
	{
		m_controller.ChangeScene(std::make_unique<GameScene>(m_controller));
		return;
	}
}

void TitleScene::Draw()
{

}

void TitleScene::End()
{

}

void TitleScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]ÉLÅ[Ç≈ Debug Scene", 0xffffff);
#endif
}
