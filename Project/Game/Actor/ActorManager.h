#pragma once
#include <list>
#include <memory>
#include <map>

class Actor;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager();
	~ActorManager();
	//登録
	void Entry(std::shared_ptr<Actor> actor);
	//解除
	void Exit(std::shared_ptr<Actor> actor);
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw() const;
	//終了処理
	void End();
private:
	//アクター
	std::list<std::shared_ptr<Actor>> m_actors;
	//ID
	int m_actorId;//割り振る番号
};

