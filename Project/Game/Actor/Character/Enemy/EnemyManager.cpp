#include "EnemyManager.h"
#include "EnemyBase.h"
#include "../../ActorManager.h"
#include "../Player/Player.h"
#include "../../../Camera/PlayerCamera.h"
#include "../../../../General/Input.h"
#include "../../../../General/Math/MyMath.h"

EnemyManager::EnemyManager(std::weak_ptr<ActorManager> actorM):
	m_enemys(),
	m_pActorManager(actorM)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Entry(std::shared_ptr<EnemyBase> enemy)
{
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚ç‚µ‚È‚¢
	auto it = std::find(m_enemys.begin(), m_enemys.end(), enemy);
	if (it != m_enemys.end())return;
	//“G‚ğ’Ç‰Á
	m_enemys.emplace_back(enemy);
}

void EnemyManager::Exit(std::shared_ptr<EnemyBase> enemy)
{
	//“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚È‚ç‚µ‚È‚¢
	auto it = std::find(m_enemys.begin(), m_enemys.end(), enemy);
	if (it == m_enemys.end())return;
	m_enemys.erase(it);
}


void EnemyManager::Init()
{
}

void EnemyManager::Update()
{
	if (m_enemys.empty())return;
}

void EnemyManager::End()
{
	//ƒAƒNƒ^[‚ÌI—¹ˆ—
	std::list<std::shared_ptr<EnemyBase>> deleteEnemy;
	for (auto& enemy : m_enemys)
	{
		deleteEnemy.emplace_back(enemy);
	}
	for (auto& enemy : deleteEnemy)
	{
		Exit(enemy);
	}
	deleteEnemy.clear();
}
