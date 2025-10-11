#include "PlayerStateMoving.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateFall.h"
#include "PlayerStateHit.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	constexpr float kRunSpeedRata = 1.5f;
	const char* kAnimNameJog = "Player|Jog_N";
	const char* kAnimNameRun = "Player|Run";
}

PlayerStateMoving::PlayerStateMoving(std::weak_ptr<Actor> player, bool isDash) :
	PlayerStateBase(player),
	m_speed(0.0f)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//ステータス
	auto status = owner->GetCharaStatus();
	float speed = status->GetMS();
	//歩き
	if (!isDash)
	{
		owner->GetModel()->SetAnim(kAnimNameJog, true);
		m_speed = speed;
	}
	//走り
	else
	{
		owner->GetModel()->SetAnim(kAnimNameRun, true);
		m_speed = speed * kRunSpeedRata;
	}
	owner->SetCollState(CollisionState::Move);

	//地面に付いてるなら
	if (owner->IsFloor())
	{
		//回避可能
		owner->SetIsAvoidable(true);
	}
}

PlayerStateMoving::~PlayerStateMoving()
{
}

void PlayerStateMoving::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateMoving::Update()
{
	if (m_owner.expired())return;
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
	auto& input = Input::GetInstance();
	//回避
	if (input.IsTrigger("B") && owner->IsAvoidable())
	{
		//回避
		ChangeState(std::make_shared<PlayerStateAvoid>(m_owner));
		return;
	}
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//ジャンプ
	if (owner->IsJumpable() && input.IsTrigger("A"))
	{
		ChangeState(std::make_shared<PlayerStateJump>(m_owner));
		return;
	}
	//移動
	Vector3 vec = InputMoveVec(owner,input);
	vec *= m_speed;
	owner->GetRb()->SetMoveVec(vec);

	//モデルの向き
	owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
}
