#include "GameoverScene.h"
#include "../General/AssetManager.h"
#include "../General/Fader.h"
#include "../Game/UI/Gameover/GameoverUI.h"
#include "../Game/UI/UIManager.h"

GameoverScene::GameoverScene(SceneController& controller):
	SceneBase(controller)
{
}

GameoverScene::~GameoverScene()
{
}

void GameoverScene::Init()
{
	//フェードイン
	Fader::GetInstance().FadeIn();

	//UIの削除
	UIManager::GetInstance().AllDeleteUI();

	auto gameoverUI = std::make_shared<GameoverUI>();
	gameoverUI->Init();
	m_pGameoverUI = gameoverUI;
}

void GameoverScene::Update()
{
}

void GameoverScene::Draw()
{
}

void GameoverScene::End()
{
}

void GameoverScene::DebugDraw() const
{
}
