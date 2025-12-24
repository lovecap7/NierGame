#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "DebugScene/DebugSelectScene.h"
#include "../General/Fader.h"
#include "../General/Game.h"
#include "../Game/UI/Title/TitleUI.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Actor/ActorManager.h"
#include "../General/AssetManager.h"
#include "../Game/Camera/CameraController.h"
#include "../Game/Camera/TitleCamera.h"
#include "../Main/Application.h"
#include "../General/ShaderPostProcess.h"
#include "../General/Math/MyMath.h"
#include "../General/Effect/EffekseerManager.h"
#include "../General/Sound/SoundManager.h"
#include "../General/Sound/SE.h"

namespace
{
	//最初のグリッジフレーム
	constexpr int kStartHardShakingFrame = 100;
	//強いグリッジ
	constexpr float kBlockScele = 10.0f;
	constexpr float kNoiseSpeed = 10.0f;
	constexpr float kShakeStrength = 10.0f;

	//タイトル
	const std::wstring kTitlePath = L"Title";
	const std::wstring kCharacterDataPath = L"CharacterData";
	const std::wstring kStageDataPath = L"StageData";

	//BGM
	const std::wstring kBGMTitlePath = L"TitleScene";
	//SE
	const std::wstring kSENoisePath = L"Noise";
	const std::wstring kSEGameStartPath = L"GameStart";
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_hardShakingCountFrame(kStartHardShakingFrame),
	m_blockScele(kBlockScele),
	m_noiseSpeed(kNoiseSpeed),
	m_shakeStrength(kShakeStrength),
	m_effectManager(EffekseerManager::GetInstance()),
	m_noiseSE()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	//アセットの削除
	AssetManager::GetInstance().AllDelete();
	//タイムスケール
	Application::GetInstance().SetTimeScale(1.0f);
	//UI削除
	UIManager::GetInstance().AllDeleteUI();
	//フェードイン
	Fader::GetInstance().FadeIn();
	//エフェクトリセット
	m_effectManager.Reset();

	//エフェクトマネージャー初期化
	m_effectManager.Init();
	//音
	auto& soundManager = SoundManager::GetInstance();
	//サウンドデータのロード
	soundManager.LoadBGM(kBGMTitlePath);
	soundManager.LoadSE(kSENoisePath);
	soundManager.LoadBGM(kSEGameStartPath);

	//BGM再生
	soundManager.PlayBGM(kBGMTitlePath);
	//ノイズ再生
	m_noiseSE = soundManager.PlayLoopSE(kSENoisePath);

	//カメラ
	auto camera = std::make_shared<TitleCamera>();
	auto& cameraController = CameraController::GetInstance();
	cameraController.Init();
	cameraController.ChangeCamera(camera);

	//タイトルロゴ
	auto titleLogo = std::make_shared<TitleUI>();
	titleLogo->Init();

	//グリッジ
	auto& postPrecess = Application::GetInstance().GetPostProcess();
	postPrecess->AddPostEffectState(ShaderPostProcess::PostEffectState::NoColorGlitch);
	postPrecess->SetBlockScele(m_blockScele);
	postPrecess->SetNoiseSpeed(m_noiseSpeed);
	postPrecess->SetShakeStrength(m_shakeStrength);

	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	m_actorManager->CreateActorCSV(kTitlePath.c_str(), kCharacterDataPath.c_str());
	m_actorManager->CreateActorCSV(kTitlePath.c_str(), kStageDataPath.c_str());
	
}

void TitleScene::Update()
{
	auto& input = Input::GetInstance();

	//エフェクトの再生
	m_effectManager.Update();

	auto& fader = Fader::GetInstance();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_unique<SelectScene>(m_controller));
		return;
	}

	//グリッジの更新
	UpdateGlitch();

	//カメラ
	auto& cameraController = CameraController::GetInstance();
	cameraController.Update();

	if (input.IsTrigger("A") && !fader.IsFadeNow())
	{
		//フェード
		fader.FadeOut();
		//クリックSE
		auto& soundManager = SoundManager::GetInstance();
		soundManager.PlayOnceSE(kSEGameStartPath);
	}

	m_actorManager->Update();
}

void TitleScene::Draw()
{
	m_actorManager->Draw();
	m_effectManager.Draw();
}

void TitleScene::End()
{
	//グリッジ削除
	auto& postPrecess = Application::GetInstance().GetPostProcess();
	postPrecess->SubPostEffectState(ShaderPostProcess::PostEffectState::NoColorGlitch);

	m_actorManager->End();
}

void TitleScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
}


void TitleScene::UpdateGlitch()
{
	auto& postPrecess = Application::GetInstance().GetPostProcess();

	//最初の数フレームは激しくグリッジ
	if (m_hardShakingCountFrame > 0)
	{
		//激しく
		--m_hardShakingCountFrame;
		//グリッジの各値の設定
		postPrecess->SetBlockScele(m_blockScele);
		postPrecess->SetNoiseSpeed(m_noiseSpeed);
		postPrecess->SetShakeStrength(m_shakeStrength);
	}
	else
	{
		//ノイズSE停止
		if (!m_noiseSE.expired())
		{
			m_noiseSE.lock()->Delete();
		}
		//ノイズ終了
		postPrecess->SubPostEffectState(ShaderPostProcess::PostEffectState::NoColorGlitch);
	}
}
