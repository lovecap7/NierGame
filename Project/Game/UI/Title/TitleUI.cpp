#include "TitleUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Math/MyMath.h"
#include "../../../General/Game.h"
#include <DxLib.h>

namespace
{
	//ハンドル
	const std::wstring kTitleHandlePath = L"Title/Title";
	const std::wstring kTitleBackHandlePath = L"Title/TitleBack";
}

TitleUI::TitleUI() :
	UIBase(),
	m_titleLogoHandle(-1),
	m_titleBackHandle(-1)
{
	//ハンドル
	m_titleLogoHandle = AssetManager::GetInstance().GetImageHandle(kTitleHandlePath);
	m_titleBackHandle = AssetManager::GetInstance().GetImageHandle(kTitleBackHandlePath);
	//前描画
	m_isFrontDraw = true;
}

TitleUI::~TitleUI()
{
}

void TitleUI::Update()
{

}

void TitleUI::Draw() const
{
	//タイトル背景
	DrawRotaGraphF(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_titleBackHandle, true);
	//タイトルロゴ
	DrawRotaGraphF(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_titleLogoHandle, true);
}
