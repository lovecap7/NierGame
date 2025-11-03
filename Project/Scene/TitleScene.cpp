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
#include "../General/Math/MyMath.h"

namespace
{
	//最初のグリッジフレーム
	constexpr int kStartHardShakingFrame = 100;
	//強いグリッジ
	constexpr float kBlockScele = 10.0f;
	constexpr float kNoiseSpeed = 10.0f;
	constexpr float kShakeStrength = 10.0f;
	//揺れるフレーム
	constexpr int kIrregularFrameMin = 80;
	constexpr int kIrregularFrameMax = 150;
	//カウントリセット
	constexpr int kResetIrregularFrame = 10;
	//lerp率
	constexpr float kLerpRate = 0.1f;
	//0とみなす値
	constexpr float kEpsilon = 0.1f;
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_hardShakingCountFrame(kStartHardShakingFrame),
	m_IrregularCountFrame(0),
	m_IrregularFrame(kStartHardShakingFrame),
	m_blockScele(kBlockScele),
	m_noiseSpeed(kNoiseSpeed),
	m_shakeStrength(kShakeStrength)
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
	postPrecess->AddPostEffectState(ShaderPostProcess::PostEffectState::NoColorGlitch);
	postPrecess->SetBlockScele(m_blockScele);
	postPrecess->SetNoiseSpeed(m_noiseSpeed);
	postPrecess->SetShakeStrength(m_shakeStrength);
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

	//グリッジの更新
	UpdateGlitch();

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
	//グリッジ削除
	auto& postPrecess = Application::GetInstance().GetPostProcess();
	postPrecess->SubPostEffectState(ShaderPostProcess::PostEffectState::NoColorGlitch);
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
	}
	else
	{
		// 各値
		float scale, speed, strength;
		scale = speed = strength = 0.0f;

		//不定期に揺れる
		++m_IrregularCountFrame;
		if (m_IrregularCountFrame >= m_IrregularFrame + kResetIrregularFrame)
		{
			m_IrregularCountFrame = 0;
			m_IrregularFrame = MyMath::GetRand(kIrregularFrameMin, kIrregularFrameMax);
		}
		else if (m_IrregularCountFrame >= m_IrregularFrame)
		{
			scale = kBlockScele;
			speed = kNoiseSpeed;
			strength = kShakeStrength;
		}
		m_blockScele = MathSub::Lerp(m_blockScele, scale, kLerpRate);
		m_noiseSpeed = MathSub::Lerp(m_noiseSpeed, speed, kLerpRate);
		m_shakeStrength = MathSub::Lerp(m_shakeStrength, strength, kLerpRate);

		//小さい値の場合0にする
		if (m_blockScele < kEpsilon)m_blockScele = 0.0f;
		if (m_noiseSpeed < kEpsilon)m_noiseSpeed = 0.0f;
		if (m_shakeStrength < kEpsilon)m_shakeStrength = 0.0f;
	}
	//グリッジの各値の設定
	postPrecess->SetBlockScele(m_blockScele);
	postPrecess->SetNoiseSpeed(m_noiseSpeed);
	postPrecess->SetShakeStrength(m_shakeStrength);
}
