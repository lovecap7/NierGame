#include "EnemyBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/Rigidbody.h"

EnemyBase::EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager):
	CharacterBase(actorData, charaStatusData, Shape::Capsule, pActorManager),
	m_attackCoolTime(0.0f)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::UpdateLockOnViewPos()
{
	//ロックオン座標
	Vector3 start = m_rb->GetNextPos();
	Vector3 end = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetNextEndPos(m_rb->GetVecWithTS());
	m_lockOnViewPos = (start + end) * 0.5f;
}

void EnemyBase::CountAttackCoolTime()
{
	if (m_attackCoolTime <= 0.0f)return;
	m_attackCoolTime -= GetTimeScale();
}
