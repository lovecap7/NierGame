#include "OptionUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include <string>
#include <DxLib.h>

namespace
{
	//パス
	const std::wstring kBGMPath = L"Option/Option_BGM";
	const std::wstring kSEPath = L"Option/Option_SE";
	const std::wstring kVoicePath = L"Option/Option_Voice";
	const std::wstring kScreenModePath = L"Option/Option_ScreenMode";
	const std::wstring kWindowModePath = L"Option/Option_Window";
	const std::wstring kFullScreenModePath = L"Option/Option_FullScreen";
	const std::wstring kCursorPath = L"Mark/Cursor";

	//座標
	constexpr float kMenuPosX = Game::kScreenCenterX - 200.0f;
	constexpr float kBasePosY = 100.0f;
	constexpr float kOffsetPosY = 100.0f;
}

OptionUI::OptionUI() :
	m_bgmHandle(-1),
	m_seHandle(-1),
	m_voiceHandle(-1),
	m_screenModeHandle(-1),
	m_windowHandle(-1),
	m_fullScreenHandle(-1),
	m_cursorHandle(-1)
{
	auto& assetManager = AssetManager::GetInstance();
	//ハンドル取得
	m_bgmHandle = assetManager.GetImageHandle(kBGMPath);
	m_seHandle = assetManager.GetImageHandle(kSEPath);
	m_voiceHandle = assetManager.GetImageHandle(kVoicePath);
	m_screenModeHandle = assetManager.GetImageHandle(kScreenModePath);
	m_windowHandle = assetManager.GetImageHandle(kWindowModePath);
	m_fullScreenHandle = assetManager.GetImageHandle(kFullScreenModePath);
	m_cursorHandle = assetManager.GetImageHandle(kCursorPath);

	//選択中メニュー初期化
	m_selectMenuIndex = static_cast<int>(OptionScene::OptionMenu::BGM);

	//メニュー座標初期化
	m_menuPos.resize(static_cast<int>(OptionScene::OptionMenu::Max));
	for(int i = 0; i < static_cast<int>(OptionScene::OptionMenu::Max); i++)
	{
		m_menuPos[i] = { kMenuPosX , kBasePosY + i * kOffsetPosY };
	}
}

OptionUI::~OptionUI()
{
}

void OptionUI::Update()
{
}

void OptionUI::Draw() const
{
	int bgmIndex = static_cast<int>(OptionScene::OptionMenu::BGM);
	int seIndex = static_cast<int>(OptionScene::OptionMenu::SE);
	int voiceIndex = static_cast<int>(OptionScene::OptionMenu::Voice);
	int screenModeIndex = static_cast<int>(OptionScene::OptionMenu::ScreenMode);
	//描画
	DrawRotaGraph(m_menuPos[bgmIndex].x, m_menuPos[bgmIndex].y, 1.0f, 0.0f, m_bgmHandle, true);
	DrawRotaGraph(m_menuPos[seIndex].x, m_menuPos[seIndex].y, 1.0f, 0.0f, m_seHandle, true);
	DrawRotaGraph(m_menuPos[voiceIndex].x, m_menuPos[voiceIndex].y, 1.0f, 0.0f, m_voiceHandle, true);
	DrawRotaGraph(m_menuPos[screenModeIndex].x, m_menuPos[screenModeIndex].y, 1.0f, 0.0f, m_screenModeHandle, true);

}
