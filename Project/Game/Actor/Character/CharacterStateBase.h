#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
#include "../Actor.h"
namespace
{
	//レイキャストの許容値(自分とターゲット)
	constexpr int kRayCastBeforeMoveToleranceNum = 2;
	//移動後のレイキャストの許容値(ターゲットのみ)
	constexpr int kRayCastAfterMoveToleranceNum = 1;
}

class Input;
class GameCamera;
class ActorManager;
class CharacterStateBase abstract
{
public:
	CharacterStateBase(std::weak_ptr<Actor> owner);
	virtual ~CharacterStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<CharacterStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<CharacterStateBase> nextState);
	//持ち主の参照
	std::weak_ptr<Actor> GetOwner() const { return m_owner; }
private:
	//次の状態
	std::shared_ptr<CharacterStateBase> m_nextState;
protected:
	//自分のポインタ
	std::weak_ptr<Actor> m_owner;
	//次の移動ベクトルをレイキャストの結果をもとに取得
	Vector3 GetNextNomVecFromRayCast(std::shared_ptr<Actor> coll, Vector3 targetPos, Vector3 moveDir, float speed, 
		int beforeMoveToleranceNum = kRayCastBeforeMoveToleranceNum,int afterMoveToleranceNum = kRayCastAfterMoveToleranceNum);
};

