#include "GameScene.h"
#include "TitleScene.h"
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

GameScene::GameScene(SceneController& controller, std::wstring stageName) :
	SceneBase(controller),
	m_stageName(stageName)
{

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	auto stageName = m_stageName.c_str();

	//カメラ
	auto camera = std::make_shared<PlayerCamera>();
	m_cameraController = std::make_shared<CameraController>();
	m_cameraController->Init();
	m_cameraController->ChangeCamera(camera);

	m_attackManager = std::make_shared<AttackManager>();
	m_attackManager->Init();

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
}

void GameScene::Update()
{
	//更新
	m_actorManager->Update();
	m_attackManager->Update();
	m_cameraController->Update();
	m_battleAreaManager->Update(m_actorManager);
}

void GameScene::Draw()
{
	m_actorManager->Draw();
	m_attackManager->Draw();
}

void GameScene::End()
{
	m_actorManager->End();
	m_attackManager->End();
	m_battleAreaManager->End();
}

void GameScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Game Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
