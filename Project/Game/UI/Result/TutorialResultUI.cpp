#include "TutorialResultUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include <DxLib.h>

namespace
{
	//パス
	const std::wstring kImagePath = L"Result/MissionComplete";
	//終了フレーム
	constexpr int kFinishFrame = 45;
	//位置
	const Vector2 kFirstPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY);
	const Vector2 kSecondPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY - 200);
	//Lerp
	constexpr float kLerpRate = 0.1f;
}

TutorialResultUI::TutorialResultUI(StageIndex tutorialIndex) :
	UIBase(),
	m_isFinish(false),
	m_mcHandle(-1),
	m_backHandle(-1),
	m_countFrame(0),
	m_pos{ -Game::kScreenWidth,Game::kScreenCenterY }
{
	m_mcHandle = AssetManager::GetInstance().GetImageHandle(kImagePath);
}

TutorialResultUI::~TutorialResultUI()
{
}

void TutorialResultUI::Update()
{
	++m_countFrame;
	if (m_countFrame <= kFinishFrame)
	{
		m_pos = Vector2::Lerp(m_pos, kFirstPos, kLerpRate);
	}
	else
	{
		m_pos = Vector2::Lerp(m_pos, kSecondPos, kLerpRate);
		m_isFinish = true;
	}
}

void TutorialResultUI::Draw() const
{
	DrawRotaGraph(m_pos.x, m_pos.y, 1.0, 0.0, m_mcHandle, true);
}
