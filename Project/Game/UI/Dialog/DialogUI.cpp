#include "DialogUI.h"
#include "../../../General/AssetManager.h"
#include <string>

namespace
{
	const std::wstring kYesPath = L"Dialog/Yes";
	const std::wstring kNoPath = L"Dialog/No";
	const std::wstring kBackPath = L"Dialog/DialogBack";
}

DialogUI::DialogUI():
	UIBase(),
	m_yesHandle(-1),
	m_noHandle(-1),
	m_backHandle(-1)
{
	auto& assetManager = AssetManager::GetInstance();
	m_yesHandle = assetManager.GetImageHandle(kYesPath);
	m_noHandle = assetManager.GetImageHandle(kNoPath);
	m_backHandle = assetManager.GetImageHandle(kBackPath);
}

DialogUI::~DialogUI()
{
}

void DialogUI::Update()
{

}

void DialogUI::Draw() const
{
}
