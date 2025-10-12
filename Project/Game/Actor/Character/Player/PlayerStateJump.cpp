#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const char* kAnimJump1 = "Player|Jump1";
	const char* kAnimJump2 = "Player|Jump2";
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

	//ステータス
	auto status = owner->GetCharaStatus();

	//ジャンプ
	owner->GetRb()->SetVecY(status->GetJP());

	//回避可能
	owner->SetIsAvoidable(true);
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
	//ステータス
	auto status = owner->GetCharaStatus();
	//死亡
	if (status->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//回避
	if (input.IsBuffered("B") && owner->IsAvoidable())
	{
		ChangeState(std::make_shared<PlayerStateAvoid>(m_owner));
		return;
	}
	//やられ
	if (status->IsHitReaction())
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

	//ジャンプ中の移動
	MoveJump(input, owner, status);
}

void PlayerStateJump::MoveJump(Input& input, std::shared_ptr<Player> owner, std::shared_ptr<CharaStatus> status)
{
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		auto rb = owner->GetRb();
		//移動
		Vector3 vec = InputMoveVec(owner, input);
		vec *= status->GetMS();
		//空中移動
		rb->SetMoveVec(vec);
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
	}
}
