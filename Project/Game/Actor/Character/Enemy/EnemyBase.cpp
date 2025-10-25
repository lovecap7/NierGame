#include "EnemyBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/Model.h"
#include "../Player/Player.h"

EnemyBase::EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager):
	CharacterBase(actorData, charaStatusData, Shape::Capsule, pActorManager),
	m_attackCoolTime(0.0f),
	m_isActive(true),
	m_isAlerted(true)
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::IsEnableAttack() const
{
	return m_targetInfo.m_isFound && m_attackCoolTime <= 0.0f;
}

void EnemyBase::UpdateLockOnViewPos()
{
	//ロックオン座標
	Vector3 start = m_rb->GetNextPos();
	Vector3 end = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetNextEndPos(m_rb->GetVecWithTS());
	m_lockOnViewPos = (start + end) * 0.5f;
}

void EnemyBase::SearchTarget(std::shared_ptr<Player> player)
{
	//前回の発見状態を保存
	m_isAlerted = m_targetInfo.m_isFound;
	//視野角内にいるかどうか
	m_targetInfo.m_isFound = false;

	//プレイヤーの位置
	auto targetPos = player->GetRb()->GetPos();

	//距離を確認
	auto myPos = m_rb->GetPos();
	auto dir = targetPos.XZ() - myPos.XZ();
	if (dir.Magnitude() <= m_charaStatus->GetSearchRange())
	{
		//警戒状態なら無条件で発見
		if (m_isAlerted)
		{
			m_targetInfo.m_pTarget = player;
			m_targetInfo.m_isFound = true;
			return;
		}

		//視野角内にターゲットがいるか
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir));
		if (angle <= (m_charaStatus->GetSearchAngle() / 2.0f))
		{
			m_targetInfo.m_pTarget = player;
			m_targetInfo.m_isFound = true;
			m_isAlerted = true;
		}
	}
}

Vector3 EnemyBase::GetToTargetVec() const
{
	{
		if (!m_targetInfo.m_isFound || m_targetInfo.m_pTarget.expired())
		{
			return Vector3::Zero();
		}
		auto target = m_targetInfo.m_pTarget.lock();
		Vector3 toTarget = target->GetRb()->GetPos() - this->GetRb()->GetPos();
		if (toTarget.SqMagnitude() > 0.0f)
		{
			toTarget = toTarget.Normalize();
		}
		return toTarget;
	}
}

bool EnemyBase::IsInMeleeRange() const
{
	if (!m_targetInfo.m_isFound || m_targetInfo.m_pTarget.expired())
	{
		return false;
	}
	auto target = m_targetInfo.m_pTarget.lock();
	Vector3 toTarget = target->GetRb()->GetPos() - this->GetRb()->GetPos();
	float distance = toTarget.Magnitude();
	if (distance < m_charaStatus->GetMeleeAttackRange())
	{
		return true;
	}
	return false;
}

void EnemyBase::CountAttackCoolTime()
{
	if (m_attackCoolTime <= 0.0f)return;
	m_attackCoolTime -= GetTimeScale();
}
