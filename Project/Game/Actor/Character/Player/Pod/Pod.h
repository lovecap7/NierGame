#pragma once
#include "../../CharacterBase.h"
#include <memory>
class ActorManager;
class Collidable;
class Player;
class Rigidbody;
class ActorData;
class CharaStatusData;

class Pod :
    public CharacterBase
{
public:
    Pod(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData,
		std::weak_ptr<ActorManager> pActorManager,std::weak_ptr<Player> pPlayer);
    ~Pod();
	//初期化処理
	void Init()override;
	//更新処理
	void Update() override;
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//終了処理
	void End()override;

	//ステートにアクセスさせる関数
	//リジッドボディ
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//Player座標
	Vector3 GetPlayerPos()const;
	//カメラの向き
	Vector3 GetCameraDir()const;
private:
	//プレイヤーの参照
	std::weak_ptr<Player> m_pPlayer;
};

