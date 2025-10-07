#include "PlayerTestScene.h"
#include <Dxlib.h>
#include  "../../General/Input.h"
#include "../SceneController.h"
#include "../../Game/Actor/ActorManager.h"
#include "../../Game/Camera/CameraController.h"
#include "../../Game/Camera/PlayerCamera.h"
#include "../../General/Collision/Physics.h"

PlayerTestScene::PlayerTestScene(SceneController& controller) :
	SceneBase(controller)
{
}

PlayerTestScene::~PlayerTestScene()
{
}

void PlayerTestScene::Init()
{
	//カメラ
	auto camera = std::make_shared<PlayerCamera>();
	m_cameraController = std::make_shared<CameraController>();
	m_cameraController->Init();
	m_cameraController->ChangeCamera(camera);
	//アクター
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	m_actorManager->CreateActorCSV("DebugScene/CharacterData");
	m_actorManager->CreateActorCSV("DebugScene/StageData");
	m_actorManager->SetCamera(camera);
}

void PlayerTestScene::Update()
{
	m_actorManager->Update();
	m_cameraController->Update();
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
	m_actorManager->Draw();
}

void PlayerTestScene::End()
{
	m_actorManager->End();
}

void PlayerTestScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Player Test Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
