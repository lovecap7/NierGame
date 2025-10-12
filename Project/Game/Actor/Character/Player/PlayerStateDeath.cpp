#include "PlayerStateDeath.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Death";
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 5;
}

PlayerStateDeath::PlayerStateDeath(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetModel()->SetAnim(kAnim, false);
	owner->SetCollState(CollisionState::Dead);
	auto status = owner->GetCharaStatus();
	//無敵
	status->SetIsNoDamage(true);
}

PlayerStateDeath::~PlayerStateDeath()
{
}
void PlayerStateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update()
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto status = owner->GetCharaStatus();
	//復活したら
	if (!status->IsDead())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}

	//移動量リセット
	owner->GetRb()->SetMoveVec(Vector3::Zero());
}
