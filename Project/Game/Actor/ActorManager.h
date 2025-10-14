#pragma once
#include <list>
#include <memory>
#include <map>
#include "../../General/Collision/Collidable.h"

class Actor;
class ActorData;
class Player;
class PlayerCamera;
class CharaStatusData;
class CharacterBase;
class AttackManager;
class AttackBase;
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
	void CreateActorCSV(const char* folderName,const char* fileName);

	//カメラのセット
	void SetCamera(std::weak_ptr<PlayerCamera> pCamera) { m_pCamera = pCamera; };
	//カメラの参照
	std::weak_ptr<PlayerCamera> GetCamera()const { return m_pCamera; };

	//攻撃マネージャーのセット
	void SetAttackManager(std::weak_ptr<AttackManager> pAttackManager) { m_pAttackManager = pAttackManager; };
	//攻撃マネージャー参照
	std::weak_ptr<AttackManager> GetAttackManager()const { return m_pAttackManager; };

private:
	//アクター
	std::list<std::shared_ptr<Actor>> m_actors;
	//カメラの参照
	std::weak_ptr<PlayerCamera> m_pCamera;
	//攻撃マネージャーの参照
	std::weak_ptr<AttackManager> m_pAttackManager;
private:
	//キャラクターの作成
	std::shared_ptr<CharacterBase> CreateChara(GameTag tag, std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> data);

	//プレイヤーの準備
	void SetUpPlayer(std::shared_ptr<Player> player);
};

