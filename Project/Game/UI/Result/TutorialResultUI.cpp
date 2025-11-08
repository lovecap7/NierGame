#include "TutorialResultUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include <DxLib.h>

namespace
{
	//パス
	const std::wstring kMCPath = L"Result/MissionComplete";
	const std::wstring kBackPath = L"Back/BeigeBack";
	const std::wstring kNextTutorialPath = L"Result/NextTutorial";
	const std::wstring kSelectStagePath = L"Result/SelectStage";
	//終了フレーム
	constexpr int kFinishFrame = 45;
	//位置
	const Vector2 kMCFirstPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY);
	const Vector2 kMCSecondPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY - 200);
	const Vector2 kMenuSecondPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY);
	constexpr int kMenuOffsetY = 150;
	//Lerp
	constexpr float kLerpRate = 0.1f;
}

TutorialResultUI::TutorialResultUI(StageIndex tutorialIndex) :
	UIBase(),
	m_isFinish(false),
	m_mcHandle(-1),
	m_backHandle(-1),
	m_nextTutorialHandle(-1),
	m_selectStageHandle(-1),
	m_countFrame(0),
	m_mcPos{ -Game::kScreenWidth,Game::kScreenCenterY },
	m_menuPos{ -Game::kScreenWidth, Game::kScreenCenterY },
	m_menuIndex(Menu::Next)
{
	auto& assetManager = AssetManager::GetInstance();
	//ミッションコンプリート
	m_mcHandle = assetManager.GetImageHandle(kMCPath);
	//背景
	m_backHandle = assetManager.GetImageHandle(kBackPath);
	//セレクト
	m_nextTutorialHandle = assetManager.GetImageHandle(kNextTutorialPath);
	m_selectStageHandle = assetManager.GetImageHandle(kSelectStagePath);
}

TutorialResultUI::~TutorialResultUI()
{
}

void TutorialResultUI::Update()
{
	++m_countFrame;
	if (m_countFrame <= kFinishFrame)
	{
		m_mcPos = Vector2::Lerp(m_mcPos, kMCFirstPos, kLerpRate);
	}
	else
	{
		m_mcPos = Vector2::Lerp(m_mcPos, kMCSecondPos, kLerpRate);
		m_menuPos = Vector2::Lerp(m_menuPos, kMenuSecondPos, kLerpRate);
		m_isFinish = true;
	}
}

void TutorialResultUI::Draw() const
{	
	//背景
	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
	DrawGraph(0, 0, m_backHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ミッションコンプリート
	DrawRotaGraph(m_mcPos.x, m_mcPos.y, 1.0, 0.0, m_mcHandle, true);

	//セレクト
	DrawRotaGraph(m_menuPos.x, m_menuPos.y, 1.0, 0.0, m_nextTutorialHandle, true);
	DrawRotaGraph(m_menuPos.x, m_menuPos.y + kMenuOffsetY, 1.0, 0.0, m_selectStageHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	switch (m_menuIndex)
	{
	case TutorialResultUI::Menu::Next:
		DrawRotaGraph(m_menuPos.x, m_menuPos.y, 1.0, 0.0, m_nextTutorialHandle, true);
		break;
	case TutorialResultUI::Menu::Select:
		DrawRotaGraph(m_menuPos.x, m_menuPos.y + kMenuOffsetY, 1.0, 0.0, m_selectStageHandle, true);
		break;
	default:
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TutorialResultUI::SetMenuIndex(int index)
{
	m_menuIndex = static_cast<Menu>(MathSub::ClampInt(index, static_cast<int>(Menu::Next), static_cast<int>(Menu::Select)));
}
