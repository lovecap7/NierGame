#include "BattleArea.h"
#include "StageObject.h"
#include "../Character/Player/Player.h"
#include "../ActorManager.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../../General/Collision/CapsuleCollider.h"

BattleArea::BattleArea(std::weak_ptr<StageObject> start, std::weak_ptr<StageObject> end):
	m_startArea(start),
	m_endArea(end),
	m_isBattleStart(false),
	m_isNoEnemys(false),
	m_update(&BattleArea::UpdatePlayerSearvh)
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
	//壁をオフ
	OffWall();
}

void BattleArea::Update(const std::shared_ptr<ActorManager> actorManager)
{
	if (!actorManager)return;
	if (actorManager->GetPlayer().expired())return;
	//状態ごとの更新処理
	(this->*m_update)(actorManager);
}

void BattleArea::InitInEnemys(const std::shared_ptr<EnemyManager> enemyManager)
{
	//範囲内のCollidableの参照を取得
	auto enemys = enemyManager->GetEnemys();
	for (auto& enemy : enemys)
	{
		//範囲内の敵の参照を取得
		if (IsInArea(enemy->GetPos(),0.0f))
		{
			//エリア内である
			enemy->SetIsInArea(true);
			m_areaEnemies.emplace_back(enemy);
		}
	}
}

void BattleArea::CheckInPlayer(std::shared_ptr<Player> player)
{
	//座標から範囲に入ったかをチェック
	Vector3 playerPos = player->GetPos();
	float playerRadius = std::dynamic_pointer_cast<CapsuleCollider>(player->GetColl())->GetRadius();
	//範囲内にいるかチェック
	m_isBattleStart = IsInArea(playerPos, playerRadius);
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

bool BattleArea::IsInArea(const Vector3& pos,float radius) const
{
	if (m_startArea.expired() || m_endArea.expired())return false;
	auto startWall = m_startArea.lock();
	auto endWall = m_endArea.lock();

	//始点
	Vector3 startPos = startWall->GetPos();
	float radiusS = startWall->GetScaleZ() * 100.0f;
	Vector3 startDir = startWall->GetDir();
	//終点
	Vector3 endPos = endWall->GetPos();
	float radiusE = endWall->GetScaleZ() * 100.0f;
	Vector3 endDir = endWall->GetDir();

	startPos += startDir * (radiusS + radius);
	endPos += endDir * (radiusE + radius);

	//範囲内にいるかチェック
	return Vector2::IsPointInRect(startPos.XZ(), endPos.XZ(), pos.XZ());
}

void BattleArea::OnWall()
{
	if (m_startArea.expired() || m_endArea.expired())return;
	//壁を展開
	m_startArea.lock()->SetIsThrough(false);
	m_endArea.lock()->SetIsThrough(false);
}

void BattleArea::OffWall()
{
	if (m_startArea.expired() || m_endArea.expired())return;
	//壁を貫通
	m_startArea.lock()->SetIsThrough(true);
	m_endArea.lock()->SetIsThrough(true);
}

void BattleArea::End()
{
	m_areaEnemies.clear();
	m_startArea.reset();
	m_endArea.reset();
}

std::list<std::shared_ptr<EnemyBase>> BattleArea::GetAreaEnemys()
{
	std::list<std::shared_ptr<EnemyBase>> activeEnemys;

	for (auto enemy : m_areaEnemies)
	{
		if (enemy.expired())continue;
		activeEnemys.emplace_back(enemy.lock());
	}
	return activeEnemys;
}

void BattleArea::UpdatePlayerSearvh(const std::shared_ptr<ActorManager> actorManager)
{
	//プレイヤーが入ったかチェック
	CheckInPlayer(actorManager->GetPlayer().lock());
	if (m_isBattleStart)
	{	
		//壁を展開
		OnWall();
		m_update = &BattleArea::UpdateBattle;
		return;
	}
}

void BattleArea::UpdateBattle(const std::shared_ptr<ActorManager> actorManager)
{
	//敵が生きてるか
	CheckDeathEnemys();
	//敵をすべて倒したら
	if (m_isNoEnemys)
	{
		//壁をなくす
		OffWall();
	}
}