#include "PlayerTestScene.h"
#include <Dxlib.h>
#include  "../../General/Input.h"
#include "../SceneController.h"
#include "../../Game/Actor/ActorManager.h"
#include "../../Game/Attack/AttackManager.h"
#include "../../Game/Camera/CameraController.h"
#include "../../Game/Camera/PlayerCamera.h"
#include "../../General/Collision/Physics.h"
#include "../../General/CSV/ActorData.h"
#include "../../General/CSV/CharaStatusData.h"

#include "../../Game/Actor/DebugActor/DebugAttack.h"
#include "../../Game/Actor/DebugActor/DebugEnemy.h"
#include "../../Game/Actor/DebugActor/DebugPlayer.h"

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

	m_attackManager = std::make_shared<AttackManager>();
	m_attackManager->Init();

	//アクター
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	m_actorManager->CreateActorCSV("DebugScene","CharacterData");
	m_actorManager->CreateActorCSV("DebugScene","StageData");
	//カメラセット
	m_actorManager->SetCamera(camera);
	//攻撃マネージャーセット
	m_actorManager->SetAttackManager(m_attackManager);

	//攻撃デバッグ
	auto atData = std::make_shared<ActorData>();
	atData->m_pos = Vector3(100,100,300);
	atData->m_collRadius = 100.0f;
	atData->m_gameTag = GameTag::Attack;
	atData->m_isTrough = false;
	atData->m_isGravity = false;
	atData->m_isTrigger = true;
	auto attack = std::make_shared<DebugAttack>(atData, m_actorManager);
	m_actorManager->Entry(attack);

	//敵デバッグ
	auto enemyData = std::make_shared<ActorData>();
	enemyData->m_pos = Vector3(500, 100, 300);
	enemyData->m_collRadius = 100.0f;
	enemyData->m_gameTag = GameTag::Enemy;
	enemyData->m_isTrough = false;
	enemyData->m_isGravity = false;
	enemyData->m_isTrigger = false;
	enemyData->m_priority = Priority::Static;
	auto charaData = std::make_shared<CharaStatusData>();
	charaData->m_hp = 1000000;

	auto enemy = std::make_shared<DebugEnemy>(enemyData, charaData, m_actorManager);
	m_actorManager->Entry(enemy);
}

void PlayerTestScene::Update()
{
	//更新
	m_actorManager->Update();
	m_attackManager->Update();
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
	m_attackManager->Draw();
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
