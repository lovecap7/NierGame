#include "Boss4StateDeath.h"
#include "Boss4StateIdle.h"
#include "Boss4.h"
#include "../EnemyBase.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kDeath = L"Death";
	//ふっとばす力
	constexpr float kMinSmashPower = 10.0f;
	constexpr float kMaxSmashPower = 20.0f;
}

Boss4StateDeath::Boss4StateDeath(std::weak_ptr<Actor> enemy) :
	EnemyStateBase(enemy)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kDeath).c_str(), false);
	owner->SetCollState(CollisionState::Dead);

	//ボスではないなら
	if (!owner->IsBoss())
	{
		//ふっとばされる
		owner->GetRb()->SetVecY(MyMath::GetRandF(kMinSmashPower, kMaxSmashPower));
		owner->GetRb()->AddVec(owner->GetToTargetVec() * -MyMath::GetRandF(kMinSmashPower, kMaxSmashPower));
	}
	else
	{
		owner->GetRb()->ResetVec();
	}

	//無敵に
	owner->GetCharaStatus()->SetIsNoDamage(true);
}

Boss4StateDeath::~Boss4StateDeath()
{
}

void Boss4StateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void Boss4StateDeath::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Boss4>(m_pOwner.lock());

	//モデルのアニメーションが終わったら
	if (owner->GetModel()->IsFinishAnim())
	{
		if (owner->IsSecondPhase())
		{
			//第二形態なら志望
			owner->Delete();
		}
		else
		{
			//第一形態なら第二形態へ移行
			owner->ChangeSecondPhase();
			ChangeState(std::make_shared<Boss4StateIdle>(m_pOwner));
			return;
		}
	}
}
