#include "EnemyStateDeath.h"
#include "EnemyBase.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../General/Effect/EffekseerManager.h"

namespace
{
	//アニメーション
	const std::wstring kDeath = L"Death";
	//エフェクト
	const std::wstring kDeadEff = L"Dead";
	//ふっとばす力
	constexpr float kMinSmashPower = 10.0f;
	constexpr float kMaxSmashPower = 20.0f;
	//死亡までのフレーム
	constexpr int kDeadFrame = 20;
}

EnemyStateDeath::EnemyStateDeath(std::weak_ptr<Actor> enemy) :
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

	++m_frame;
	//モデルのアニメーションが終わったら
	if ((m_frame > kDeadFrame && !owner->IsBoss()) ||
		owner->GetModel()->IsFinishAnim())
	{
		if (!owner->IsDelete())
		{
			//爆発
			EffekseerManager::GetInstance().CreateEffect(owner->GetEffectPath(kDeadEff), owner->GetPos());
		}
		owner->Delete();
	}
}
