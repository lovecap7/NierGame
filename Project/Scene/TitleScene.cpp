#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include "../General/Fader.h"
#include "../General/Game.h"
#include "../Game/UI/Title/TitleUI.h"
#include "../Game/UI/UIManager.h"
#include "../General/AssetManager.h"
#include "../Game/Camera/CameraController.h"
#include "../Game/Camera/TitleCamera.h"
#include "../Main/Application.h"
#include "../General/ShaderPostProcess.h"

namespace
{
	//激しくグリッジ
	constexpr int kHardShakingFrame = 100;
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_hardShakingCountFrame(kHardShakingFrame)
{
	//カメラ
	auto camera = std::make_shared<TitleCamera>();
	m_cameraController = std::make_shared<CameraController>();
	m_cameraController->Init();
	m_cameraController->ChangeCamera(camera);

	//タイトルロゴ
	auto titleLogo = std::make_shared<TitleUI>();
	titleLogo->Init();

	//グリッジ
	auto& postPrecess = Application::GetInstance().GetPostProcess();
	postPrecess->AddPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
	postPrecess->SetBlockScele(10.0f);
	postPrecess->SetNoiseSpeed(10.0f);
	postPrecess->SetShakeStrength(10.0f);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	Fader::GetInstance().FadeIn();
}

void TitleScene::Update()
{
	auto& input = Input::GetInstance();

	auto& fader = Fader::GetInstance();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_unique<GameScene>(m_controller, GetStageNameByIndex(StageIndex::Tutorial)));
		return;
	}

	//最初の数フレームは激しくグリッジ
	if (m_hardShakingCountFrame > 0)
	{
		auto& postPrecess = Application::GetInstance().GetPostProcess();
		postPrecess->AddPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
		postPrecess->SetBlockScele(0.1f);
		postPrecess->SetNoiseSpeed(0.1f);
		postPrecess->SetShakeStrength(0.1f);
		//激しく
		--m_hardShakingCountFrame;
	}

	//カメラ
	m_cameraController->Update();

	if (input.IsTrigger("A") && !fader.IsFadeNow())
	{
		fader.FadeOut();
	}
}

void TitleScene::Draw()
{
}

void TitleScene::End()
{
	//UI削除
	UIManager::GetInstance().AllDeleteUI();
	//削除
	AssetManager::GetInstance().AllDelete();
}

void TitleScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}
