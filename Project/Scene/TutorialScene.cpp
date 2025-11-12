#include "TutorialScene.h"
#include "ResultScene.h"
#include "TutorialResultScene.h"
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
#include "../Main/Application.h"
#include "../General/Effect/EffekseerManager.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Tutorial/TutorialManager.h"


TutorialScene::TutorialScene(SceneController& controller, std::wstring stageName) :
	SceneBase(controller),
	m_stageName(stageName),
	m_effectManager(EffekseerManager::GetInstance())
{

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Init()
{
	//アセットを削除
	AssetManager::GetInstance().DeleteModelHandle();
	//ポストエフェクトを解除
	Application::GetInstance().GetPostProcess()->ResetPostEffectState();
	//タイムスケール
	Application::GetInstance().SetTimeScale(1.0f);
	//UI削除
	UIManager::GetInstance().AllDeleteUI();
	//Inputの入力情報リセット
	Input::GetInstance().StopUpdate();

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
	m_actorManager->CreateActorCSV(stageName, L"CheckPointData");
	//カメラセット
	m_actorManager->SetPlayerCamera(camera);
	//攻撃マネージャーセット
	m_actorManager->SetAttackManager(m_attackManager);

	//エリアマネージャー
	m_battleAreaManager = std::make_shared<BattleAreaManager>();
	m_battleAreaManager->Init(stageName, m_actorManager);

	//エフェクト
	m_effectManager.Init();

	//フェードイン
	Fader::GetInstance().FadeIn();

	//チュートリアル
	m_tutorialManager = std::make_shared<TutorialManager>(m_actorManager->GetPlayer(), m_actorManager, stageName);
}

void TutorialScene::Update()
{
	//更新
	m_tutorialManager->Update();
	m_actorManager->Update();
	m_attackManager->Update();
	CameraController::GetInstance().Update();
	m_battleAreaManager->Update(m_actorManager);
	m_effectManager.Update();

	auto& input = Input::GetInstance();

	auto& fader = Fader::GetInstance();

	//もしもすべてのエリアを突破したら
	if ((m_tutorialManager->IsClear() || input.IsTrigger("GameClear")))
	{
		m_controller.PushScene(std::make_unique<TutorialResultScene>(m_controller, GetStageIndexByName(m_stageName)));
		return;
	}
	if (m_actorManager->IsGameover())
	{
		Restart();
	}
}

void TutorialScene::Draw()
{
	m_actorManager->Draw();
	m_attackManager->Draw();
	m_effectManager.Draw();
}

void TutorialScene::End()
{
	m_actorManager->End();
	m_attackManager->End();
	m_battleAreaManager->End();
	m_effectManager.End();
}

void TutorialScene::Restart()
{
	//カメラ
	auto camera = std::make_shared<PlayerCamera>();
	auto& cameraController = CameraController::GetInstance();
	cameraController.Init();
	cameraController.ChangeCamera(camera);

	//エフェクト
	m_effectManager.End();
	m_effectManager.Init();

	//ポストエフェクトを解除
	Application::GetInstance().GetPostProcess()->ResetPostEffectState();

	//タイムスケール
	Application::GetInstance().SetTimeScale(1.0f);

	//UI削除
	UIManager::GetInstance().AllDeleteUI();

	//Inputの入力情報リセット
	Input::GetInstance().StopUpdate();

	//アクターの再スタート
	m_actorManager->Restart();
	m_actorManager->SetPlayerCamera(camera);

	//攻撃マネージャー
	m_attackManager->Init();
	m_attackManager->SetPlayerCamera(camera);

	//エリアの再スタート
	m_battleAreaManager->Restart();

	//フェードイン
	Fader::GetInstance().FadeIn();
}

void TutorialScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Game Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
