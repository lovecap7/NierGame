#include "EnemyManager.h"
#include "EnemyBase.h"
#include "../../ActorManager.h"
#include "../Player/Player.h"
#include "../../../Camera/PlayerCamera.h"
#include "../../../../General/Input.h"
#include "../../../../General/Math/MyMath.h"

namespace
{
	//活動範囲
	constexpr float kActiveRange = 5000.0f;
}

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
	//すでに登録されているならしない
	auto it = std::find(m_enemys.begin(), m_enemys.end(), enemy);
	if (it != m_enemys.end())return;
	//敵を追加
	m_enemys.emplace_back(enemy);
}

void EnemyManager::Exit(std::shared_ptr<EnemyBase> enemy)
{
	//登録されていないならしない
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
	//プレイヤーとの距離で活動範囲を判定する
	if (m_pActorManager.expired())return;
	auto actorManager = m_pActorManager.lock();
	if (actorManager->GetPlayer().expired())return;
	auto player = actorManager->GetPlayer().lock();
	Vector3 playerPos = player->GetRb()->GetNextPos();
	for (auto& enemy : m_enemys)
	{
		//活動範囲内か
		bool isActive = false;

		//距離を計算
		Vector3 enemyPos = enemy->GetRb()->GetNextPos();
		float distance = (playerPos - enemyPos).Magnitude();
		//活動範囲内なら活動
		isActive = distance <= kActiveRange;
		enemy->SetIsActive(isActive);

		//プレイヤーを発見
		if (isActive)
		{
			enemy->SearchTarget(player);
		}
	}
}

void EnemyManager::End()
{
	//アクターの終了処理
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
