#pragma once
#include <list>
#include <memory>
#include <map>
#include "../../General/Collision/Collidable.h"

class Actor;
class ActorData;
class PlayerCamera;
class CharacterBase;
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
	//CSVからアクターを作成
	void CreateActorCSV(const char* path);
	//カメラのセット
	void SetCamera(std::weak_ptr<PlayerCamera> pCamera) { m_pCamera = pCamera; };
	//カメラの参照
	std::weak_ptr<PlayerCamera> GetCamera()const { return m_pCamera; };
private:
	//アクター
	std::list<std::shared_ptr<Actor>> m_actors;
	//ID
	int m_actorId;//割り振る番号
	//カメラの参照
	std::weak_ptr<PlayerCamera> m_pCamera;
private:
	//キャラクターの作成
	std::shared_ptr<CharacterBase> CreateChara(GameTag tag, std::shared_ptr<ActorData> data);
};

