#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"

namespace
{
	//アニメーション
	const char* kAnim = "rig_deform|Idle_N";
}

PlayerStateIdle::PlayerStateIdle(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
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
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	
}
