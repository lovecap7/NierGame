#include "EnemyStateAttack.h"
#include "EnemyBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateHit.h"
#include "EnemyStateDeath.h"
#include "../Player/Player.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../Attack/AOEAttack.h"
#include "../../../Attack/EnemyBulletAttack.h"
#include "../../../../General/Math/MyMath.h"
#include "../../../../General/Model.h"
#include "../../../../General/CSV/AttackData.h"
#include "../../../../General/Collision/Rigidbody.h"


EnemyStateAttack::EnemyStateAttack(std::weak_ptr<Actor> enemy, std::shared_ptr<AttackData> attackData):
	EnemyStateBase(enemy),
	m_isAppearedAttack(false),
	m_isShotBullet(false)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);
	//攻撃データ
	m_attackData = attackData;
	//アニメーションセット
	owner->GetModel()->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), true);


}

EnemyStateAttack::~EnemyStateAttack()
{
	//弾は消さない
	if (!m_isShotBullet)
	{
		//攻撃削除
		DeleteAttack();
	}
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

void EnemyStateAttack::UpdateStartAttack(std::shared_ptr<EnemyBase> owner)
{
	if (m_frame >= m_attackData->m_startFrame)
	{
		//持続が切れたら
		if (m_isAppearedAttack && (m_pAttack.expired() || m_attackData->m_attackType == AttackData::AttackType::Bullet))
		{
			
			//多段ヒット攻撃の処理
			if (m_attackData->m_isMultipleHit && m_attackData->m_nextAttackName != L"None")
			{
				//多段ヒット攻撃
				LoadNextMultipleHitAttack(owner);
				return;
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
	else if(m_attackData->m_attackType == AttackData::AttackType::Bullet)
	{
		attack = std::make_shared<EnemyBulletAttack>(m_attackData, owner);
		//弾の初期位置と方向を設定
		auto bulletAttack = std::dynamic_pointer_cast<EnemyBulletAttack>(attack);
		auto model = owner->GetModel();

		//発射座標
		Vector3 bulletPos = MV1GetFramePosition(model->GetModelHandle(), static_cast<int>(m_attackData->m_param1));
		bulletAttack->SetPos(bulletPos);

		//XZ方向はモデルの向き
		Vector3 bulletDir = model->GetDir();

		//ターゲットがいるとき
		if (owner->GetTargetInfo().m_isFound)
		{
			auto player = std::dynamic_pointer_cast<Player>(owner->GetTargetInfo().m_pTarget.lock());
			Vector3 targetDir = player->GetCenterPos() - bulletPos;
			if (targetDir.SqMagnitude() > 0.0f)
			{
				targetDir = targetDir.Normalize();
			}
			bulletDir.y = targetDir.y;
		}


		//正規化
		if (bulletDir.SqMagnitude() > 0.0f)
		{
			bulletDir = bulletDir.Normalize();
		}
		//移動
		bulletAttack->SetMoveVec(bulletDir * m_attackData->m_param2);

		//壊れるか
		bulletAttack->SetIsDestructible(m_attackData->m_param3 != 0.0f);

		//弾を打った
		m_isShotBullet = true;
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
	//攻撃作成
	if (m_frame >= m_attackData->m_startFrame)
	{
		CreateAttack(owner);
	}
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
