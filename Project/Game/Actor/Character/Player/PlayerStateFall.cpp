#include "PlayerStateFall.h"
#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateAvoid.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	//アニメーション
	const char* kAnimFall = "Player|Fall";
	//移動速度
	constexpr float kMoveSpeed = 2.0f;
	//上限
	constexpr float kMaxMoveSpeed = 10.0f;
}

PlayerStateFall::PlayerStateFall(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetModel()->SetAnim(kAnimFall, true);
	owner->SetCollState(CollisionState::Fall);
	//Y成分のリセット
	owner->GetRb()->SetVecY(0.0f);
}

PlayerStateFall::~PlayerStateFall()
{
}
void PlayerStateFall::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateFall::Update()
{
	auto& input = Input::GetInstance();
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//地面に付いたら
	if (owner->IsFloor())
	{
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//ジャンプ
	if (owner->IsJumpable() && input.IsTrigger("A"))
	{
		ChangeState(std::make_shared<PlayerStateJump>(m_owner));
		return;
	}
	//回避
	if (input.IsTrigger("B"))
	{
		ChangeState(std::make_shared<PlayerStateAvoid>(m_owner));
		return;
	}
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		auto rb = owner->GetRb();
		//移動
		Vector3 vec = InputMoveVec(owner, input);
		vec *= kMoveSpeed;
		//空中移動
		rb->AddVec(vec);
		//横移動速度に上限をつける
		float speed = rb->GetMoveVec().Magnitude();
		if (speed > 0.0f)
		{
			speed = MathSub::ClampFloat(speed, 0.0f, kMaxMoveSpeed);
			Vector3 moveVec = rb->GetMoveVec();
			if (moveVec.Magnitude() > 0.0f)
			{
				moveVec = moveVec.Normalize();
			}
			rb->SetMoveVec(moveVec * speed);
		}
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
	}
}
