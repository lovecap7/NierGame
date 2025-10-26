#pragma once
#include <memory>
#include <string>
#include <vector>

class ActorManager;
class BattleArea;
class BattleAreaManager
{
public:
	BattleAreaManager();
	~BattleAreaManager();
	//初期化
	void Init(std::wstring stageName,std::shared_ptr<ActorManager> actorManager);
	//更新
	void Update(std::shared_ptr<ActorManager> actorManager);
	//終了処理
	void End();
private:
	//エリア
	std::vector<std::shared_ptr<BattleArea>> m_areas;

private:
	//終了したエリアの削除
	void CheckDeleteArea();
};

