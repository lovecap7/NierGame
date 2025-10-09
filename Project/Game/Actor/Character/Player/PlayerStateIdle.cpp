#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateFall.h"
#include "PlayerStateJump.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
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
	//落下
	if (owner->IsFall())
	{
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
		return;
	}
	//回避
	if (input.IsTrigger("B"))
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
	//だんだん減速
	owner->GetRb()->SpeedDown(kMoveDeceRate);
}
