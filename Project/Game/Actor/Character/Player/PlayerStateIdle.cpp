#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateFall.h"
#include "PlayerStateJump.h"
#include "PlayerStateHit.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Idle_N";
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 5;
}

PlayerStateIdle::PlayerStateIdle(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetModel()->SetAnim(kAnim, true);
	owner->SetCollState(CollisionState::Normal);

	//地面に付いてるなら
	if (owner->IsFloor())
	{
		//回避可能
		owner->SetIsAvoidable(true);
	}
}

PlayerStateIdle::~PlayerStateIdle()
{
}
void PlayerStateIdle::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateIdle::Update()
{
	auto& input = Input::GetInstance();
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//やられ
	if (owner->GetCharaStatus()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//落下
	if (owner->IsFall())
	{
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
		return;
	}
	//回避
	if (input.IsTrigger("B") && owner->IsAvoidable())
	{
		ChangeState(std::make_shared<PlayerStateAvoid>(m_owner));
		return;
	}
	//ジャンプ
	if (owner->IsJumpable() && input.IsTrigger("A"))
	{
		ChangeState(std::make_shared<PlayerStateJump>(m_owner));
		return;
	}
	//入力があるなら移動
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//歩き
		ChangeState(std::make_shared<PlayerStateMoving>(m_owner,false));
		return;
	}
	//移動量リセット
	owner->GetRb()->SetMoveVec(Vector3::Zero());
}
