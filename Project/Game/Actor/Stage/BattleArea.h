#pragma once
#include <memory>
#include <list>
#include "../../../General/Math/MyMath.h"
class StageObject;
class EnemyBase;
class EnemyManager;
class ActorManager;
class Player;
class BattleArea
{
public:
	BattleArea(std::weak_ptr<StageObject> start, std::weak_ptr<StageObject> end);
	~BattleArea();
	//初期化
	void Init(const std::weak_ptr<EnemyManager> enemyManager);
	//更新
	void Update(const std::shared_ptr<ActorManager> actorManager);
	//終了処理
	void End();
	//敵がいるか
	bool IsNoEnemys()const { return m_isNoEnemys; };
private:
	//範囲内の敵
	std::list<std::weak_ptr<EnemyBase>> m_areaEnemies;
	//エリア始点
	std::weak_ptr<StageObject> m_startArea;
	//エリア終点
	std::weak_ptr<StageObject> m_endArea;
	//エリア内にプレイヤーが入ったか
	bool m_isBattleStart;
	//全ての敵を倒した
	bool m_isNoEnemys;
private:
	//範囲内の敵を取得
	void InitInEnemys(const std::shared_ptr<EnemyManager> enemyManager);
	//プレイヤーが範囲に入ったか
	void CheckInPlayer(std::shared_ptr<Player> player);
	//範囲内の敵がすべて倒されたか
	void CheckDeathEnemys();
	//範囲内に座標があるかチェック
	bool IsInArea(const Vector3& pos) const;
};

