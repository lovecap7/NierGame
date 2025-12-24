#include "EndingScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "TitleScene.h"
#include "DebugScene/DebugSelectScene.h"
#include "../General/Fader.h"
#include "../General/Game.h"
#include "../Game/UI/Ending/EndingTextUI.h"
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
	//タイトル
	const std::wstring kEndingPath = L"Ending";
	const std::wstring kCharacterDataPath = L"CharacterData";
	const std::wstring kStageDataPath = L"StageData";

	//BGM
	const std::wstring kBGMEndingPath = L"TitleScene";

	//スキップフレーム数
	constexpr int kSkipFrame = 60 * 2;
}

EndingScene::EndingScene(SceneController& controller) :
	SceneBase(controller),
	m_effectManager(EffekseerManager::GetInstance()),
	m_skipFrame(0.0f)
{
}

EndingScene::~EndingScene()
{
}

void EndingScene::Init()
{
	//アセットの削除
	AssetManager::GetInstance().AllDelete();
	//タイムスケール
	Application::GetInstance().SetTimeScale(1.0f);
	//UI削除
	UIManager::GetInstance().AllDeleteUI();
	//フェードイン
	Fader::GetInstance().FadeIn();
	//シェーダーリセット
	Application::GetInstance().GetPostProcess()->ResetPostEffectState();
	//エフェクトマネージャー初期化
	m_effectManager.Init();
	//音
	auto& soundManager = SoundManager::GetInstance();
	//サウンドデータのロード
	soundManager.LoadBGM(kBGMEndingPath);
	//BGM再生
	soundManager.PlayBGM(kBGMEndingPath);
	
	//カメラ
	auto camera = std::make_shared<TitleCamera>();
	auto& cameraController = CameraController::GetInstance();
	cameraController.Init();
	cameraController.ChangeCamera(camera);

	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>();
	m_actorManager->Init();
	m_actorManager->CreateActorCSV(kEndingPath.c_str(), kCharacterDataPath.c_str());
	m_actorManager->CreateActorCSV(kEndingPath.c_str(), kStageDataPath.c_str());

	//UI
	auto endingTextUI = std::make_shared<EndingTextUI>();
	endingTextUI->Init();
	m_endingTextUI = endingTextUI;

	//スキップフレーム
	m_skipFrame = 0;
}

void EndingScene::Update()
{
	auto& input = Input::GetInstance();

	//エフェクトの再生
	m_effectManager.Update();

	auto& fader = Fader::GetInstance();
	//フェードアウトしきったら
	if (fader.IsFinishFadeOut())
	{
		m_controller.ChangeScene(std::make_unique<TitleScene>(m_controller));
		return;
	}

	//カメラ
	auto& cameraController = CameraController::GetInstance();
	cameraController.Update();

	//全ての処理が終了したら
	if (!m_endingTextUI.expired())
	{
		if (m_endingTextUI.lock()->IsAllEnd() || input.IsPress("A"))
		{
			++m_skipFrame;
		}
		else
		{
			--m_skipFrame;
		}
		m_skipFrame = MathSub::ClampFloat(m_skipFrame, 0, kSkipFrame);
	}

	if (m_skipFrame >= kSkipFrame && !fader.IsFadeNow())
	{
		//フェード
		fader.FadeOut();
	}

	m_actorManager->Update();
}

void EndingScene::Draw()
{
	m_actorManager->Draw();
	m_effectManager.Draw();
}

void EndingScene::End()
{
	m_actorManager->End();
}

void EndingScene::DebugDraw() const
{
#if _DEBUG
	DrawString(0, 0, L"Ending Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
	DrawFormatString(0, 32, 0xffffff,L"Skip = %d", m_skipFrame);
#endif
}
