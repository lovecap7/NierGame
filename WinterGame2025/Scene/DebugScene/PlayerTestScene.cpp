#include "PlayerTestScene.h"
#include <Dxlib.h>
#include  "../../General/Input.h"
#include "../SceneController.h"
#include "../../Game/Actor/ActorManager.h"
#include "../../Game/Actor/DebugActor/DebugPlayer.h"
#include "../../Game/Actor/DebugActor/DebugEnemy.h"
#include "../../Game/Actor/DebugActor/DebugWeapon.h"
#include "../../Game/Camera/CameraController.h"
#include "../../Game/Camera/PlayerCamera.h"

PlayerTestScene::PlayerTestScene(SceneController& controller) :
	SceneBase(controller)
{
}

PlayerTestScene::~PlayerTestScene()
{
}

void PlayerTestScene::Init()
{
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	//デバッグ用プレイヤー
	auto debugPlayer = std::make_shared<DebugPlayer>();
	m_actorManager->Entry(debugPlayer);
	//デバッグ用敵
	auto debugEnemy = std::make_shared<DebugEnemy>();
	m_actorManager->Entry(debugEnemy);
	debugPlayer->SetEnemy(debugEnemy);
	//カメラ
	m_cameraController = std::make_shared<CameraController>();
	m_cameraController->Init();
	//プレイヤーカメラ
	auto playerCamera = std::make_shared<PlayerCamera>();
	m_cameraController->ChangeCamera(playerCamera);
	//デバッグ用プレイヤーにカメラをセット
	debugPlayer->SetCamera(playerCamera);
	//武器の生成
	auto weapon = std::make_shared<DebugWeapon>(MV1LoadModel(L"Data/Model/Weapon/SwordZ.mv1"));
	m_actorManager->Entry(weapon);
	debugPlayer->SetWeapon(weapon);
}

void PlayerTestScene::Update()
{
	auto& input = Input::GetInstance();
	//アクターマネージャー更新
	m_actorManager->Update();
	//ターゲット
	Vector3 targetPos = Vector3::Zero();
	if (!m_actorManager->GetPlayer().expired())
	{
		targetPos = m_actorManager->GetPlayer().lock()->GetPos();
	}
	//カメラ更新
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
	//アクターマネージャー描画
	m_actorManager->Draw();
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
