#include "UIManager.h"
#include <DxLib.h>
#include <cassert>
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/ActorData.h"
#include "UIBase.h"

void UIManager::Entry(std::shared_ptr<UIBase> ui)
{
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚ç‚µ‚È‚¢
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//UI‚Ì’Ç‰Á
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚È‚ç‚µ‚È‚¢
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it == m_uis.end())return;
	m_uis.erase(it);
}


void UIManager::Update()
{
	//UI‚ÌXV
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::Draw() const
{
	//•`‰æ
	for (auto& ui : m_uis)
	{
		ui->Draw();
	}
}

void UIManager::AllDeleteUI()
{
	//ˆê‹C‚É“o˜^‰ğœ
	m_uis.clear();
}
