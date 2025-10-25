#include "EnemyStateAttack.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateHit.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../Attack/AOEAttack.h"
#include "../../../../General/Math/MyMath.h"
#include "../../../../General/Model.h"
#include "../../../../General/CSV/AttackData.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	const std::wstring kAttack1DataName = L"Attack1";
	const std::wstring kAttack2DataName = L"Attack2";
	const std::wstring kAttack3DataName = L"Attack3";
}

EnemyStateAttack::EnemyStateAttack(std::weak_ptr<Actor> enemy):
	EnemyStateBase(enemy),
	m_isAppearedAttack(false)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());

	int index = MyMath::GetRand(1, 3);
	std::wstring attackDataName;
	switch (index)
	{
	case 1:
		attackDataName = kAttack1DataName;
		break;
	case 2:
		attackDataName = kAttack2DataName;
		break;
	case 3:
		attackDataName = kAttack3DataName;
		break;
	default:
		attackDataName = kAttack1DataName;
		break;
	}
	//攻撃データ
	m_attackData = owner->GetAttackData(attackDataName);
	//アニメーションセット
	owner->GetModel()->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), true);

}

EnemyStateAttack::~EnemyStateAttack()
{
	//攻撃削除
	DeleteAttack();
	//攻撃クールタイム設定
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->SetAttackCoolTime(m_attackData->m_cancelFrame);
}

void EnemyStateAttack::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void EnemyStateAttack::Update()
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

	//フレームをカウント
	CountFrame();

	//モデル
	auto model = owner->GetModel();

	//発生フレームになったら
	UpdateStartAttack(owner);
	//攻撃移動更新
	UpdateMove(owner, model);
	//攻撃位置更新
	UpdateAttackPos(owner);

	//アニメーションが終了したら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<EnemyStateIdle>(m_pOwner));
		return;
	}
}

void EnemyStateAttack::UpdateStartAttack(std::shared_ptr<EnemyBase>& owner)
{
	if (m_frame >= m_attackData->m_startFrame)
	{
		//持続が切れたら
		if (m_isAppearedAttack && m_pAttack.expired())
		{
			//多段ヒット攻撃の処理
			if (m_attackData->m_isMultipleHit && m_attackData->m_nextAttackName != L"None")
			{
				//多段ヒット攻撃
				LoadNextMultipleHitAttack(owner);
			}
		}
		//まだ攻撃が発生していないなら発生
		if (!m_isAppearedAttack)
		{
			CreateAttack(owner);
		}
	}
}
void EnemyStateAttack::DeleteAttack()
{
	if (m_pAttack.expired())return;
	auto attack = m_pAttack.lock();
	attack->Delete();
}

void EnemyStateAttack::CreateAttack(std::shared_ptr<EnemyBase> owner)
{
	if (!m_attackData)return;
	std::shared_ptr<AttackBase> attack;
	//攻撃作成
	if (m_attackData->m_attackType == AttackData::AttackType::Sword)
	{
		attack = std::make_shared<SwordAttack>(m_attackData, owner);
	}
	else if (m_attackData->m_attackType == AttackData::AttackType::AOE)
	{
		attack = std::make_shared<AOEAttack>(m_attackData, owner);
	}
	owner->SetAttack(attack);
	m_pAttack = attack;
	m_isAppearedAttack = true;
}

void EnemyStateAttack::LoadNextMultipleHitAttack(std::shared_ptr<EnemyBase> owner)
{
	if (!m_attackData)return;
	// 多段ヒット処理
	m_attackData = owner->GetAttackData(m_attackData->m_nextAttackName);
	m_isAppearedAttack = false;
}

void EnemyStateAttack::UpdateMove(std::shared_ptr<EnemyBase> owner, std::shared_ptr<Model> model)
{
	if (!m_attackData)return;
	Vector3 moveVec = Vector3::Zero();
	if (m_frame < m_attackData->m_moveFrame)
	{
		//向き
		Vector3 dir = owner->GetToTargetVec();
		//モデルの向き
		model->SetDir(dir.XZ());
		moveVec = model->GetDir() * m_attackData->m_moveSpeed;
	}
	//移動
	owner->GetRb()->SetMoveVec(moveVec);
}

void EnemyStateAttack::UpdateAttackPos(std::shared_ptr<EnemyBase> owner)
{
	if (!m_attackData)return;
	if (m_pAttack.expired())return;

	//フレームインデックス取得
	int handle = owner->GetModel()->GetModelHandle();
	Vector3 pos1 = MV1GetFramePosition(handle,static_cast<int>(m_attackData->m_param1));

	//攻撃の位置更新
	if (m_attackData->m_attackType == AttackData::AttackType::Sword ||
		m_attackData->m_attackType == AttackData::AttackType::Throw)
	{
		auto attack = std::dynamic_pointer_cast<SwordAttack>(m_pAttack.lock());
		attack->SetStartPos(pos1);
		Vector3 pos2 = MV1GetFramePosition(handle, static_cast<int>(m_attackData->m_param1));
		Vector3 dir = (pos2 - pos1);
		if (dir.SqMagnitude() > 0.0f)
		{
			dir = dir.Normalize();
		}
		attack->SetEndPos(pos1 + (dir * m_attackData->m_length));
	}
	else if (m_attackData->m_attackType == AttackData::AttackType::AOE)
	{
		auto attack = std::dynamic_pointer_cast<AOEAttack>(m_pAttack.lock());
		attack->SetPos(pos1);
	}
}
