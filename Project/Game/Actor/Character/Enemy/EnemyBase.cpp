#include "EnemyBase.h"
#include "../CharacterStateBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Input.h"
#include "../../ActorManager.h"
#include "../../SearchPlace.h"
#include "../../../../Shader/MyDraw.h"

namespace
{
	//プレイヤーとの距離が遠いときに当たり判定を停止する距離
	const float kStopCollisionDistance = 4000.0f;
	//最初のクールタイム
	constexpr int kAttackCoolTime = 60;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//落下したと判定するY座標
	constexpr float kStageFallY = -500.0f;
	//警戒している時の索敵距離の倍率
	constexpr float kWarningDistanceRate = 1.4f;
}

EnemyBase::EnemyBase(Shape shape, EnemyGrade grade) :
	CharacterBase(shape),
	m_attackCoolTime(kAttackCoolTime),
	m_enemyGrade(grade),
	m_isActive(true),
	m_isWarning(false),
	m_name{}
{
}

void EnemyBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//落下した際の処理
	if (m_rb->m_pos.y < kStageFallY)
	{
		m_isDelete = true;
	}
	//プレイヤーから遠いなら処理をしない
	if (IsStopActiveCollision(actorManager))return;
#if _DEBUG
	if (m_enemyGrade == EnemyGrade::Boss)
	{
		//ボスを死亡させる
		if (Input::GetInstance().IsTrigger("BossDead"))
		{
			m_hitPoints->SetIsNoDamege(false);
			m_hitPoints->Damage(999999);
		}
	}
#endif
	//アクティブ状態じゃないなら
	if (!m_isActive)
	{
		//減速
		m_rb->SpeedDown(kMoveDeceRate);
		//アニメーションの更新
		m_model->Update();
		return;
	}
	//攻撃のクールタイムを減らす
	UpdateAttackCoolTime();
	//ターゲットを発見できたかをチェック
	auto target = actorManager.lock()->GetPlayer();
	if (!target.expired())
	{
		//視野角
		float viewingAngle	 = m_viewingAngle;
		//索敵距離
		float searchDistance = m_searchDistance;
		//警戒状態なら
		if (m_isWarning)
		{
			//全方位
			viewingAngle = MyMath::TwoPI_F;
			//索敵距離が長くなる
			searchDistance *= kWarningDistanceRate;
		}
		//範囲と視野角からターゲット探す
		TargetSearch(searchDistance, viewingAngle, target.lock()->GetPos());
	}
	//状態に合わせた更新
	m_state->Update(camera, actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//体力クラスのフラグリセット
	m_hitPoints->ResetHitFlags();
}

void EnemyBase::Draw() const
{
#if _DEBUG
	//DrawCapsule3D(
	//	m_rb->GetPos().ToDxLibVector(),
	//	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
	//	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
	//	16,
	//	0xff0000,
	//	0xff0000,
	//	false
	//);
	// 
	//座標
	auto pos = m_rb->m_pos;
	//視野角
	float viewingAngle = m_viewingAngle;
	//索敵距離
	float searchDistance = m_searchDistance;
	//警戒状態なら
	if (m_isWarning)
	{
		//全方位
		viewingAngle = MyMath::TwoPI_F;
		//索敵距離が長くなる
		searchDistance *= kWarningDistanceRate;
	}
	//索敵範囲
	MyDrawUtils::Draw3DCircle(pos, searchDistance, 36, 0x0000ff);
	//見てる方向
	auto forward = m_model->GetDir();
	forward = forward * searchDistance;
	//視野角
	auto viewDir1 = Quaternion::AngleAxis(viewingAngle / 2.0f, Vector3::Up()) * forward;
	auto viewDir2 = Quaternion::AngleAxis(-viewingAngle / 2.0f, Vector3::Up()) * forward;
	//描画
	DrawLine3D(pos.ToDxLibVector(), (pos + forward).ToDxLibVector(), 0xff0000);
	DrawLine3D(pos.ToDxLibVector(), (pos + viewDir1).ToDxLibVector(), 0xff0000);
	DrawLine3D(pos.ToDxLibVector(), (pos + viewDir2).ToDxLibVector(), 0xff0000);
	if (m_searchPlace)
	{
		m_searchPlace->Draw();
	}
#endif
	m_model->Draw();
}

void EnemyBase::UpdateAttackCoolTime()
{
	//攻撃できないなら
	if (!m_canAttack)
	{
		//攻撃権が与えられた瞬間にすぐに攻撃されると困るので
		if (m_attackCoolTime < kAttackCoolTime)
		{
			m_attackCoolTime = kAttackCoolTime;
		}
		return;
	}
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}

void EnemyBase::SetActive(bool isActive)
{
	m_isActive = isActive;
	if (!m_isActive)
	{
		//無敵
		m_hitPoints->SetIsNoDamege(true);
	}
	else
	{
		//無敵解除
		m_hitPoints->SetIsNoDamege(false);
	}
}

bool EnemyBase::IsStopActiveCollision(const std::weak_ptr<ActorManager> actorManager)
{
	//停止したか
	bool isStop = false;
	//プレイヤーとの距離
	auto player = actorManager.lock()->GetPlayer();
	if (player.expired())
	{
		return true;
	}
	auto dis = (player.lock()->GetPos() - m_rb->m_pos).Magnitude();
	//プレイヤーとの距離が遠いときは当たり判定を停止する
	if (dis > kStopCollisionDistance)
	{
		m_isThrough = true;				//当たり判定を停止
		m_rb->m_isGravity = false;		//重力を無効化
		m_rb->SetVec(Vector3::Zero());	//動きを止める
		isStop = true;					//停止した
	}
	else
	{
		m_isThrough = false;			//当たり判定を有効化
		m_rb->m_isGravity = true;		//重力を有効化
	}
	return isStop;
}
