#include "EnemyStateHit.h"
#include "EnemyStateIdle.h"
#include "EnemyBase.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kHit = L"Hit";
}

EnemyStateHit::EnemyStateHit(std::weak_ptr<Actor> enemy) :
	EnemyStateBase(enemy)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kHit).c_str(), false);
}

EnemyStateHit::~EnemyStateHit()
{
}
void EnemyStateHit::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void EnemyStateHit::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	
	//ヒット中に攻撃を再度喰らったら最初から
	if (owner->GetCharaStatus()->IsHitReaction())
	{
		//最初から
		owner->GetModel()->ReplayAnim();
	}
	//モデルのアニメーションが終わったら
	if (owner->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<EnemyStateIdle>(m_pOwner));
		return;
	}
	//落下速度を落とす
	auto rb = owner->GetRb();
	if (rb->GetVec().y < 0.0f)
	{
		rb->SetVecY(rb->GetVec().y * 0.1f);
	}
}