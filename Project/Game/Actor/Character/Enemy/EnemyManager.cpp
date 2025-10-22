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


	//カメラロックオン
	if (m_pActorManager.expired())return;
	auto actorM = m_pActorManager.lock();
	if (actorM->GetPlayerCamera().expired())return;
	auto camera = actorM->GetPlayerCamera().lock();

	//LBでロックオン開始
	auto& input = Input::GetInstance();
	if (input.IsTrigger("LB"))
	{
		//ロックオン中なら解除
		if (camera->IsLockOn())
		{
			camera->EndLockOn();
		}
		//ロックオンしていないなら開始
		else
		{
			//プレイヤーの座標
			if (actorM->GetPlayer().expired())return;
			auto player = actorM->GetPlayer().lock();
			Vector3 playerPos = player->GetNextPos();

			//最も近い敵を探す
			std::shared_ptr<EnemyBase> nearestEnemy = nullptr;
			float minDis = 5000.0f; //索敵範囲
			bool isFind = false;
			for (auto enemy : m_enemys)
			{
				Vector3 enemyPos = enemy->GetNextPos();
				Vector3 toEnemy = enemyPos - playerPos;
				float distance = toEnemy.Magnitude();
				if (distance < minDis)
				{
					nearestEnemy = enemy;
					//発見
					isFind = true;
				}
			}
			//ロックオン開始
			if (isFind)
			{
				camera->StartLockOn(nearestEnemy);
			}
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
