#include "PlayerTestScene.h"
#include <Dxlib.h>
#include  "../../General/Input.h"
#include "../SceneController.h"

PlayerTestScene::PlayerTestScene(SceneController& controller) :
	SceneBase(controller)
{
}

PlayerTestScene::~PlayerTestScene()
{
}

void PlayerTestScene::Init()
{
	
}

void PlayerTestScene::Update()
{
	
}

void PlayerTestScene::Draw()
{
#if _DEBUG
	//グリッド線
	for (int i = -10; i <= 10; i++)
	{
		DrawLine3D(VGet(-1000, 0, i * 100), VGet(1000, 0, i * 100), 0xff0000);
		DrawLine3D(VGet(i * 100, 0, -1000), VGet(i * 100, 0, 1000), 0x00ffff);
	}
#endif
}

void PlayerTestScene::End()
{

}

void PlayerTestScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Player Test Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
