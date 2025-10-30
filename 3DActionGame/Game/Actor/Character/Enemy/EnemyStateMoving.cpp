#include "EnemyStateMoving.h"
#include "EnemyStateIdle.h"
#include "EnemyStateHit.h"
#include "EnemyStateAttack.h"
#include "EnemyStateFall.h"
#include "EnemyStateDeath.h"
#include "EnemyBase.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	//アニメーション
	const std::wstring kWalk = L"Walk";
	//最低保証フレーム数
	constexpr int kMinFrame = 10;
}
EnemyStateMoving::EnemyStateMoving(std::weak_ptr<Actor> enemy):
	EnemyStateBase(enemy)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kWalk).c_str(), true);
	owner->SetCollState(CollisionState::Move);
	//速度
	auto status = owner->GetCharaStatus();
	m_speed = status->GetMS();
}

EnemyStateMoving::~EnemyStateMoving()
{
}

void EnemyStateMoving::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void EnemyStateMoving::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());

	//フレームカウント
	CountFrame();
	//ステータス
	auto status = owner->GetCharaStatus();
	//死亡
	if (status->IsDead())
	{
		ChangeState(std::make_shared<EnemyStateDeath>(m_pOwner));
		return;
	}
	//ヒット状態
	if (owner->GetCharaStatus()->IsHitReaction())
	{
		//ヒット状態ならヒットステートへ
		ChangeState(std::make_shared<EnemyStateHit>(m_pOwner));
		return;
	}
	//落下
	if (!owner->IsFloor())
	{
		ChangeState(std::make_shared<EnemyStateFall>(m_pOwner, false));
		return;
	}
	//攻撃可能なら
	if (owner->IsEnableAttack())
	{
		//攻撃データ取得(距離から)
		auto attackData = owner->GetAttackByDistance();
		if(attackData)
		{
			//攻撃状態へ
			ChangeState(std::make_shared<EnemyStateAttack>(m_pOwner, attackData));
			return;
		}
	}
	//待機状態へ
	if (!owner->GetTargetInfo().m_isFound || 
		(owner->IsInMeleeRange() && m_frame >= kMinFrame))
	{
		ChangeState(std::make_shared<EnemyStateIdle>(m_pOwner));
		return;
	}
	//移動
	Move(owner);
}

void EnemyStateMoving::Move(std::shared_ptr<EnemyBase> owner)
{
	//移動
	if (!owner->GetTargetInfo().m_isFound || owner->GetTargetInfo().m_pTarget.expired()) return;
	Vector3 vec = owner->GetToTargetVec();
	vec *= m_speed;
	owner->GetRb()->SetMoveVec(vec);

	//モデルの向き
	owner->GetModel()->SetDir(vec.XZ());
}
