#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateFall.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kHit = L"Hit";
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 5;
}

PlayerStateHit::PlayerStateHit(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kHit).c_str(), false);
	owner->SetCollState(CollisionState::Normal);

	auto status = owner->GetCharaStatus();
	//無敵
	status->SetIsNoDamage(true);
}

PlayerStateHit::~PlayerStateHit()
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto status = owner->GetCharaStatus();
	//無敵
	status->SetIsNoDamage(false);
}
void PlayerStateHit::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateHit::Update()
{
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//モデルのアニメーションが終わったら
	if (owner->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
}
