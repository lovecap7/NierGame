#include "GameScene.h"
#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"

GameScene::GameScene(SceneController& controller) :
	SceneBase(controller)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	
}

void GameScene::Update()
{
	
}

void GameScene::Draw()
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

void GameScene::End()
{

}

void GameScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Game Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
