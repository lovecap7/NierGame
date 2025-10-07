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
	//追加予定のアクターを追加
	void AddNextActor(std::shared_ptr<Actor> actor);
	//プレイヤー
	std::weak_ptr<Actor> GetPlayer() const { return m_pPlayer; };
private:
	//追加予定のアクターを実装
	void CheckNextAddActors();
	//削除予定のアクターを削除
	void CheckDeleteActors();
private:
	//アクター
	std::list<std::shared_ptr<Actor>> m_actors;
	//追加予定のアクター
	std::list<std::shared_ptr<Actor>> m_nextAddActors;
	//ID
	int m_actorId;//割り振る番号
	//プレイヤー
	std::weak_ptr<Actor> m_pPlayer;
};

