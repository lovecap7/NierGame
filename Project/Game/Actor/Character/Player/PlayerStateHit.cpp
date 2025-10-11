#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateFall.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Hit2";
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 5;
}

PlayerStateHit::PlayerStateHit(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetModel()->SetAnim(kAnim, true);
	owner->SetCollState(CollisionState::Normal);
}

PlayerStateHit::~PlayerStateHit()
{
}
void PlayerStateHit::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateHit::Update()
{
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//ボスの体力がなくなった場合またはモデルのアニメーションが終わったら
	if (owner->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
}
