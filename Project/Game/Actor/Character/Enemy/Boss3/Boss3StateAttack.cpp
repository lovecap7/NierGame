#include "Boss3StateAttack.h"
#include "Boss3.h"
#include "Boss3StateIdle.h"
#include "Boss3StateHit.h"
#include "../EnemyStateDeath.h"
#include "../../Player/Player.h"
#include "../../../../Attack/AttackBase.h"
#include "../../../../Attack/SwordAttack.h"
#include "../../../../Attack/AOEAttack.h"
#include "../../../../Attack/EnemyBulletAttack.h"
#include "../../../../Attack/MissileAttack.h"
#include "../../../../../General/Math/MyMath.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/CSV/AttackData.h"
#include "../../../../../General/Collision/Rigidbody.h"

namespace
{
	//弾のばらつき
	constexpr int kBulletAngle = 10;
	//トリガー判定に変換
	constexpr int kTriggerFrame = 280;
}

Boss3StateAttack::Boss3StateAttack(std::weak_ptr<Actor> enemy, std::shared_ptr<AttackData> attackData) :
	EnemyStateAttack(enemy, attackData)
{
}

void Boss3StateAttack::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Boss3>(m_pOwner.lock());

	//強制待機状態へ
	if (m_isWait)
	{
		ChangeState(std::make_shared<Boss3StateIdle>(m_pOwner));
		return;
	}

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
		ChangeState(std::make_shared<Boss3StateHit>(m_pOwner));
		return;
	}

	//フレームをカウント
	CountFrame();

	//モデル
	auto model = owner->GetModel();

	//フレームによる攻撃の処理
	UpdateAttackFrame(owner);
	//攻撃移動更新
	UpdateMove(owner, model);
	//攻撃位置更新
	UpdateAttackPos(owner);

	
	//アニメーションが終了したら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<Boss3StateIdle>(m_pOwner));
		return;
	}
}

void Boss3StateAttack::UpdateMove(std::shared_ptr<EnemyBase> owner, std::shared_ptr<Model> model)
{
	if (!m_attackData)return;
	Vector3 moveVec = Vector3::Zero();

	//移動フレーム中は向きを更新
	if (m_frame < m_attackData->GetMoveFrame())
	{
		//向き
		Vector3 dir = owner->GetToTargetVec();
		//モデルの向き
		model->SetDir(dir.XZ());
		
	}
	//攻撃が出ている間移動
	if (!m_pAttack.expired())
	{
		//モデルの向きに移動
		moveVec = model->GetDir() * m_attackData->GetMoveSpeed();
	}

	//移動
	owner->GetRb()->SetMoveVec(moveVec);
}
