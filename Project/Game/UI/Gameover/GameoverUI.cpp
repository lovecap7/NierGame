#include "GameoverUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include <DxLib.h>

namespace
{
	//パス
	const std::wstring kBackPath = L"Back/BlackBack1";
	const std::wstring kGameoverPath = L"Gameover/Gameover";
	const std::wstring kContiuePath = L"Gameover/Continue";
	const std::wstring kRestartPath = L"Gameover/Restart";
	const std::wstring kStageSelectPath = L"Result/SelectStage";

	//ゲームオーバー位置
	const Vector2 kGameoverFirstPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY);
	const Vector2 kGameoverSecondPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY - 200);
	//メニュー位置
	const Vector2 kMenuFirstPos = Vector2(-Game::kScreenCenterX, Game::kScreenCenterY - 60);
	const Vector2 kMenuSecondPos = Vector2(Game::kScreenCenterX, Game::kScreenCenterY - 60);

	//メニューテキストをずらす
	constexpr int kMenuOffsetY = 140;

	//フレーム
	constexpr int kGameoverMoveFrame = 60;
	constexpr int kMenuMoveFrame = 80;

	//Lerp率
	constexpr float kGameoverLerpRate = 0.1f;
	constexpr float kMenuLerpRate = 0.1f;
}

GameoverUI::GameoverUI() :
	UIBase(),
	m_backHandle(-1),
	m_gameoverHandle(-1),
	m_countFrame(0),
	m_gameoverPos(kGameoverFirstPos),
	m_menuPos(kMenuFirstPos),
	m_continueHandle(-1),
	m_restartHandle(-1),
	m_stageSelectHandle(-1),
	m_menuIndex(Menu::Continue)
{
	auto& assetManager = AssetManager::GetInstance();
	//背景
	m_backHandle = assetManager.GetImageHandle(kBackPath);
	//ゲームオーバー
	m_gameoverHandle = assetManager.GetImageHandle(kGameoverPath);
	//コンティニュー
	m_continueHandle = assetManager.GetImageHandle(kContiuePath);
	//リスタート
	m_restartHandle = assetManager.GetImageHandle(kRestartPath);
	//ステージセレクト
	m_stageSelectHandle = assetManager.GetImageHandle(kStageSelectPath);
}

GameoverUI::~GameoverUI()
{
}

void GameoverUI::Update()
{
	++m_countFrame;
	if (m_countFrame > kMenuMoveFrame)
	{
		m_menuPos = Vector2::Lerp(m_menuPos, kMenuSecondPos, kMenuLerpRate);
	}
	if (m_countFrame > kGameoverMoveFrame)
	{
		m_gameoverPos = Vector2::Lerp(m_gameoverPos, kGameoverSecondPos, kGameoverLerpRate);
	}
}

void GameoverUI::Draw() const
{
	//背景
	DrawGraph(0, 0, m_backHandle, true);
	//ゲームオーバー
	DrawRotaGraph(m_gameoverPos.x, m_gameoverPos.y, 1.0, 0.0, m_gameoverHandle, true);
	//コンティニュー
	DrawRotaGraph(m_menuPos.x, m_menuPos.y, 1.0, 0.0, m_continueHandle, true);
	//リスタート
	DrawRotaGraph(m_menuPos.x, m_menuPos.y + kMenuOffsetY, 1.0, 0.0, m_restartHandle, true);
	//ステージセレクト
	DrawRotaGraph(m_menuPos.x, m_menuPos.y + kMenuOffsetY * 2, 1.0, 0.0, m_stageSelectHandle, true);
}
