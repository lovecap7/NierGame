#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"
#include "PlayerStateAvoid.h"
#include "Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	//アニメーション
	const char* kAnimJump1 = "Player|Jump1";
	const char* kAnimJump2 = "Player|Jump2";
	//ジャンプ力
	constexpr float kJumpPower = 20.0f;
	//移動速度
	constexpr float kMoveSpeed = 2.0f;
	//上限
	constexpr float kMaxMoveSpeed = 10.0f;
}

PlayerStateJump::PlayerStateJump(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//ジャンプ回数カウント
	owner->AddJumpNum();
	if (owner->GetJumpNum() <= 1)
	{
		owner->GetModel()->SetAnim(kAnimJump1, false);
	}
	else
	{
		owner->GetModel()->SetAnim(kAnimJump2, false);
	}
	owner->SetCollState(CollisionState::Jump);
	//ジャンプ
	owner->GetRb()->SetVecY(kJumpPower);
}

PlayerStateJump::~PlayerStateJump()
{
}
void PlayerStateJump::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateJump::Update()
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
