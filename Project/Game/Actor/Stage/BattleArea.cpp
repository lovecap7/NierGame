#include "BattleArea.h"
#include "StageObject.h"
#include "../Character/Player/Player.h"
#include "../ActorManager.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"

BattleArea::BattleArea(std::weak_ptr<StageObject> start, std::weak_ptr<StageObject> end):
	m_startArea(start),
	m_endArea(end),
	m_isBattleStart(false),
	m_isNoEnemys(false)
{
}

BattleArea::~BattleArea()
{
}

void BattleArea::Init(const std::weak_ptr<EnemyManager> enemyManager)
{
	if (enemyManager.expired())return;
	//範囲内の敵を取得
	InitInEnemys(enemyManager.lock());
}

void BattleArea::Update(const std::shared_ptr<ActorManager> actorManager)
{
	if (!actorManager)return;
	if (actorManager->GetPlayer().expired())return;

	//まだプレイヤーが範囲内にいないなら
	if (!m_isBattleStart)
	{
		//プレイヤーが入ったかチェック
		CheckInPlayer(actorManager->GetPlayer().lock());
	}
	else
	{
		//敵が生きてるか
		CheckDeathEnemys();
	}
}

void BattleArea::InitInEnemys(const std::shared_ptr<EnemyManager> enemyManager)
{
	//範囲内のCollidableの参照を取得
	auto enemys = enemyManager->GetEnemys();
	for (auto& enemy : enemys)
	{
		//範囲内の敵の参照を取得
		if (IsInArea(enemy->GetPos()))
		{
			//敵をカウントしていく
			m_areaEnemies.emplace_back(enemy);
		}
	}
}

void BattleArea::CheckInPlayer(std::shared_ptr<Player> player)
{
	//座標から範囲に入ったかをチェック
	auto playerPos = player->GetPos();
	//範囲内にいるかチェック
	m_isBattleStart = IsInArea(playerPos);
}

void BattleArea::CheckDeathEnemys()
{
	if (m_isNoEnemys)return;
	//参照がすべて切れたか
	for (auto& enemy : m_areaEnemies)
	{
		//まだ生きてるのでリターン
		if (!enemy.expired())
		{
			return;
		}
	}
	//ここまで来たら全ての敵を倒している
	m_isNoEnemys = true;
}

bool BattleArea::IsInArea(const Vector3& pos) const
{
	if (m_startArea.expired() || m_endArea.expired())return false;
	auto startPos = m_startArea.lock()->GetPos();
	auto endPos = m_endArea.lock()->GetPos();

	//範囲内にいるかチェック
	return Vector2::IsPointInRect(startPos.XZ(), endPos.XZ(), pos.XZ());
}
void BattleArea::End()
{
	m_areaEnemies.clear();
	m_startArea.reset();
	m_endArea.reset();
}
