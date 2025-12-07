#include "PauseScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../General/Effect/EffekseerManager.h"

PauseScene::PauseScene(SceneController& controller):
	SceneBase(controller)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Init()
{
	//描画停止
	UIManager::GetInstance().SetIsDraw(false);
	//エフェクト停止
	EffekseerManager::GetInstance().StopEffect();
	//Physicsを止める
	Physics::GetInstance().StopUpdate();
}

void PauseScene::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTrigger("B"))
	{
		m_controller.PopScene();
	}
}

void PauseScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PauseScene::End()
{
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
	//エフェクト開始
	EffekseerManager::GetInstance().StartEffect();
	//描画停止
	UIManager::GetInstance().SetIsDraw(true);
}