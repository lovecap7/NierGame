#pragma once
#include <memory>
#include <list>
class EnemyBase;
class ActorManager;
class EnemyManager : 
	public std::enable_shared_from_this<EnemyManager>
{
public:
	EnemyManager(std::weak_ptr<ActorManager> actorM);
	~EnemyManager();
	//登録
	void Entry(std::shared_ptr<EnemyBase> enemy);
	//解除
	void Exit(std::shared_ptr<EnemyBase> enemy);
	//初期化
	void Init();
	//更新
	void Update();
	//終了処理
	void End();

	//敵のリスト取得
	const std::list<std::shared_ptr<EnemyBase>>& GetEnemys() const { return m_enemys; }
private:
	//敵
	std::list<std::shared_ptr<EnemyBase>> m_enemys;
	//アクターマネージャーの参照
	std::weak_ptr<ActorManager> m_pActorManager;
};

