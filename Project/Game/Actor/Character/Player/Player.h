#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>
#include <map>
#include <string>

class ActorManager;
class CharacterStateBase;
class Input;
class ActorData;
class PlayerCamera;
class Player :
	public CharacterBase
{
public:
	Player(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~Player();
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
	//コリジョン
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//コリジョンの状態を設定
	void SetCollState(CollisionState collState) { m_collState = collState; }
	CollisionState GetCollState()const { return m_collState; };
	//カメラの回転量
	Quaternion GetCameraRot()const;
private:
	//カメラ
	std::weak_ptr<PlayerCamera> GetPlayerCamera()const;
};

