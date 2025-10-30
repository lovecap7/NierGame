#include "AttackManager.h"
#include <DxLib.h>
#include <cassert>
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/ActorData.h"
#include "AttackBase.h"

AttackManager::AttackManager()
{

}

AttackManager::~AttackManager()
{
}

//UŒ‚‚ğ’Ç‰Á
void AttackManager::Entry(std::shared_ptr<AttackBase> attack)
{
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚ç‚µ‚È‚¢
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it != m_attacks.end())return;
	//UŒ‚‚Ì‰Šú‰»
	attack->Init();
	//UŒ‚‚ğ’Ç‰Á
	m_attacks.emplace_back(attack);
}

void AttackManager::Exit(std::shared_ptr<AttackBase> attack)
{
	//“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚È‚ç‚µ‚È‚¢
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it == m_attacks.end())return;
	attack->End();
	m_attacks.erase(it);
}


void AttackManager::Init()
{
}

void AttackManager::Update()
{
	//UŒ‚‚ÌXV
	for (auto& attack : m_attacks)
	{
		attack->Update();
	}
	//íœƒ`ƒFƒbƒN
	CheckDelete();
}

void AttackManager::Draw() const
{
	//•`‰æ
	for (auto& attack : m_attacks)
	{
		attack->Draw();
	}
}

void AttackManager::End()
{
}

//Á–Åƒtƒ‰ƒO‚ğƒ`ƒFƒbƒN‚µ‚Äíœ
void AttackManager::CheckDelete()
{
	//Á–Åƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é‚à‚Ì‚ğíœ
	m_attacks.remove_if([](const std::shared_ptr<AttackBase>& attack) {
		if (attack->IsDelete()) {
			attack->End();
			return true;
		}
		return false;
		});
}