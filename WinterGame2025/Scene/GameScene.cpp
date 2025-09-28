#include "GameScene.h"
#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "../Game/Actor/ActorManager.h"
#include "../Game/Actor/DebugPlayer.h"
#include "../Game/Camera/Camera.h"

GameScene::GameScene(SceneController& controller) :
	SceneBase(controller)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	//デバッグ用プレイヤー
	auto debugPlayer = std::make_shared<DebugPlayer>();
	m_actorManager->Entry(debugPlayer);
	//カメラ
	m_camera = std::make_shared<Camera>();
	m_camera->Init();
	//デバッグ用プレイヤーにカメラをセット
	debugPlayer->SetCamera(m_camera);
}

void GameScene::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTrigger("A"))
	{
		m_controller.ChangeScene(std::make_unique<TitleScene>(m_controller));
		return;
	}
	//アクターマネージャー更新
	m_actorManager->Update();
	//ターゲット
	Vector3 targetPos = Vector3::Zero();
	if (!m_actorManager->GetPlayer().expired())
	{
		targetPos = m_actorManager->GetPlayer().lock()->GetPos();
	}
	//カメラ更新
	m_camera->Update(targetPos);
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
	//アクターマネージャー描画
	m_actorManager->Draw();
}

void GameScene::End()
{

}

void GameScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Game Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
	auto cPos = m_camera->GetPos();
	printf("cPos {%.2f,%.2f,%.2f}\n", cPos.x, cPos.y, cPos.z);
#endif
}
