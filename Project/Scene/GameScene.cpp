#include "GameScene.h"
#include "ResultScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "../Game/Actor/ActorManager.h"
#include "../Game/Attack/AttackManager.h"
#include "../Game/Actor/Stage/BattleAreaManager.h"
#include "../Game/Camera/CameraController.h"
#include "../Game/Camera/PlayerCamera.h"
#include "../General/Collision/Physics.h"
#include "../General/CSV/ActorData.h"
#include "../General/CSV/CharaStatusData.h"
#include "../General/AssetManager.h"
#include "../General/Fader.h"
#include "../General/ShaderPostProcess.h"
#include "../General/Timer.h"
#include "../Main/Application.h"
#include "../General/Effect/EffekseerManager.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Tutorial/TutorialManager.h"


GameScene::GameScene(SceneController& controller, std::wstring stageName) :
	SceneBase(controller),
	m_stageName(stageName),
	m_effectManager(EffekseerManager::GetInstance())
{

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//アセットを削除
	AssetManager::GetInstance().DeleteModelHandle();
	//ポストエフェクトを解除
	Application::GetInstance().GetPostProcess()->ResetPostEffectState();
	//UI削除
	UIManager::GetInstance().AllDeleteUI();

	//ステージインデックス
	auto stageName = m_stageName.c_str();

	//カメラ
	auto camera = std::make_shared<PlayerCamera>();
	auto& cameraController = CameraController::GetInstance();
	cameraController.Init();
	cameraController.ChangeCamera(camera);

	m_attackManager = std::make_shared<AttackManager>();
	m_attackManager->Init();
	m_attackManager->SetPlayerCamera(camera);

	//アクター
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	m_actorManager->CreateActorCSV(stageName, L"CharacterData");
	m_actorManager->CreateActorCSV(stageName, L"StageData");
	//カメラセット
	m_actorManager->SetCamera(camera);
	//攻撃マネージャーセット
	m_actorManager->SetAttackManager(m_attackManager);

	//エリアマネージャー
	m_battleAreaManager = std::make_shared<BattleAreaManager>();
	m_battleAreaManager->Init(stageName, m_actorManager);

	//エフェクト
	m_effectManager.Init();

	//フェードイン
	Fader::GetInstance().FadeIn();

	//タイマー
	m_timer = std::make_shared<Timer>();
	m_timer->Init();
}

void GameScene::Update()
{
	//更新
	m_actorManager->Update();
	m_attackManager->Update();
	CameraController::GetInstance().Update();
	m_battleAreaManager->Update(m_actorManager);
	m_effectManager.Update();
	m_timer->Update();

	auto& input = Input::GetInstance();

	auto& fader = Fader::GetInstance();
	//フェードアウトしたら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_unique<ResultScene>(m_stageName, m_controller, m_timer));
		return;
	}
	//もしもすべてのエリアを突破したら
	if ((m_battleAreaManager->IsEndAllArea() || input.IsTrigger("GameClear")) && !fader.IsFadeNow())
	{
		//フェードアウト
		fader.FadeOut();
	}
}

void GameScene::Draw()
{
	m_actorManager->Draw();
	m_attackManager->Draw();
	m_effectManager.Draw();
}

void GameScene::End()
{
	m_actorManager->End();
	m_attackManager->End();
	m_battleAreaManager->End();
	m_effectManager.End();
}

void GameScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Game Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
