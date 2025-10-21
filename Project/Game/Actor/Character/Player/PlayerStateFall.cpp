#include "PlayerStateFall.h"
#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateDeath.h"
#include "PlayerStateLightAttack.h"
#include "PlayerStateHeavyAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kFall = L"Fall";
	const std::wstring kGliding = L"PodFall";
}

PlayerStateFall::PlayerStateFall(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_isGliding(false),
	m_isGlided(false)
{
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kFall).c_str(), true);
	owner->SetCollState(CollisionState::Fall);
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
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());

	//ステータス
	auto status = owner->GetCharaStatus();
	//死亡
	if (status->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_pOwner));
		return;
	}
	//回避
	if (input.IsBuffered("B") && owner->IsAvoidable())
	{
		ChangeState(std::make_shared<PlayerStateAvoid>(m_pOwner));
		return;
	}
	//やられ
	if (status->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_pOwner));
		return;
	}
	//地面に付いたら
	if (owner->IsFloor())
	{
		ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
		return;
	}
	//ジャンプ
	if (owner->IsJumpable() && input.IsBuffered("A"))
	{
		ChangeState(std::make_shared<PlayerStateJump>(m_pOwner));
		return;
	}

	//空中攻撃可能なら
	if (!owner->IsAirAttacked())
	{
		//攻撃
		if (input.IsBuffered("X"))
		{
			ChangeState(std::make_shared<PlayerStateLightAttack>(m_pOwner));
			return;
		}
		if (input.IsBuffered("Y"))
		{
			ChangeState(std::make_shared<PlayerStateHeavyAttack>(m_pOwner));
			return;
		}
	}

	//落下中の移動
	MoveFall(input, owner, status);
}

void PlayerStateFall::MoveFall(Input& input, std::shared_ptr<Player> owner, std::shared_ptr<CharaStatus> status)
{
	auto rb = owner->GetRb();
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		Vector3 vec = InputMoveVec(owner, input);
		vec *= status->GetMS();
		//空中移動
		rb->SetMoveVec(vec);
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
	}
	if (input.IsPress("A") && !m_isGlided)
	{
		//滑空
		owner->GetModel()->SetAnim(owner->GetAnim(kGliding).c_str(), false);
		//落下速度を遅くする
		rb->SetVecY(rb->GetVec().y / 2.0f);
		//滑空中
		m_isGliding = true;
		
	}
	else
	{
		if (input.IsRelease("A"))
		{
			//滑空をした
			m_isGlided = true;
		}
		m_isGliding = false;
		//落下
		owner->GetModel()->SetAnim(owner->GetAnim(kFall).c_str(), true);
	}
}
