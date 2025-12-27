#include "OptionUI.h"
#include "../../../General/AssetManager.h"
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
}

OptionUI::~OptionUI()
{
}

void OptionUI::Update()
{
}

void OptionUI::Draw() const
{
	DrawRotaGraph(50, 50, 1.0f, 0.0f, m_bgmHandle, true);
	DrawRotaGraph(50, 150, 1.0f, 0.0f, m_seHandle, true);
	DrawRotaGraph(50, 250, 1.0f, 0.0f, m_voiceHandle, true);
	DrawRotaGraph(50, 350, 1.0f, 0.0f, m_screenModeHandle, true);
	DrawRotaGraph(50, 450, 1.0f, 0.0f, m_windowHandle, true);
	DrawRotaGraph(50, 550, 1.0f, 0.0f, m_fullScreenHandle, true);
	DrawRotaGraph(50, 650, 1.0f, 0.0f, m_cursorHandle, true);

}
