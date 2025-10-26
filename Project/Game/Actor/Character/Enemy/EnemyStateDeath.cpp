#include "EnemyStateDeath.h"
#include "EnemyBase.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kDeath = L"Death";
	//ふっとばす力
	constexpr float kMinSmashPower = 10.0f;
	constexpr float kMaxSmashPower = 20.0f;
}

EnemyStateDeath::EnemyStateDeath(std::weak_ptr<Actor> enemy) :
	EnemyStateBase(enemy)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kDeath).c_str(), false);
	owner->SetCollState(CollisionState::Dead);

	//ふっとばされる
	owner->GetRb()->SetVecY(MyMath::GetRandF(kMinSmashPower, kMaxSmashPower));
	owner->GetRb()->AddVec(owner->GetToTargetVec() * -MyMath::GetRandF(kMinSmashPower, kMaxSmashPower));

	//無敵に
	owner->GetCharaStatus()->SetIsNoDamage(true);
}

EnemyStateDeath::~EnemyStateDeath()
{
}

void EnemyStateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void EnemyStateDeath::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());

	//モデルのアニメーションが終わったら
	if (owner->GetModel()->IsFinishAnim())
	{
		owner->Delete();
	}
}
