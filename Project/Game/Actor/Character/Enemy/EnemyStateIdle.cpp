#include "EnemyStateIdle.h"
#include "EnemyStateHit.h"
#include "EnemyBase.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kIdle = L"Idle";
}

EnemyStateIdle::EnemyStateIdle(std::weak_ptr<Actor> enemy) :
	EnemyStateBase(enemy)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kIdle).c_str(), true);
}

EnemyStateIdle::~EnemyStateIdle()
{
}
void EnemyStateIdle::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void EnemyStateIdle::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());	
	//ヒット状態
	if (owner->GetCharaStatus()->IsHitReaction())
	{
		//ヒット状態ならヒットステートへ
		ChangeState(std::make_shared<EnemyStateHit>(m_pOwner));
		return;
	}

	//移動量リセット
	owner->GetRb()->SetMoveVec(Vector3::Zero());
}