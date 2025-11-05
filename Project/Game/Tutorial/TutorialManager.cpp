#include "TutorialManager.h"

TutorialManager::TutorialManager(std::weak_ptr<Player> pPlayer):
	m_pPlayer(pPlayer)
{
}

TutorialManager::~TutorialManager()
{
}
